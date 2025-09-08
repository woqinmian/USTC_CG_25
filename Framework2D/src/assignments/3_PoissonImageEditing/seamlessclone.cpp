#include "seamlessclone.h"
#include <iostream>

namespace USTC_CG{

void SeamlessClone::initialize(){
    int width = src_select_mask_->width();
    int height = src_select_mask_->height();
    index_.resize(width, height);
    index_.setConstant(-1);
    num_ = 0;
    for(int i = 0; i < width; i++){
        for(int j = 0; j < height; j++){
            if(src_select_mask_->get_pixel(i, j)[0] > 0){
                index_(i, j) = num_;
                num_++;
            }
        }
    }
    A_.resize(num_, num_);
    A_.setZero();
    B_red_.resize(num_);
    B_red_.setZero();
    B_green_.resize(num_);
    B_green_.setZero();
    B_blue_.resize(num_);
    B_blue_.setZero();

    std::vector<Eigen::Triplet<double>> triplet_list;
    for(int i = 0; i < width; i++){
        for(int j = 0; j < height; j++){
            int index = index_(i, j);
            if(index >= 0){
                B_red_(index)   += src_img_->get_pixel(i, j)[0] * 4;
                B_green_(index) += src_img_->get_pixel(i, j)[1] * 4;
                B_blue_(index)  += src_img_->get_pixel(i, j)[2] * 4;
                if(i > 0){
                    if(index_(i - 1, j) >= 0)
                        triplet_list.push_back(Eigen::Triplet<double>(index, index_(i - 1, j), -1.0));
                    B_red_(index)   -= src_img_->get_pixel(i - 1, j)[0];
                    B_green_(index) -= src_img_->get_pixel(i - 1, j)[1];
                    B_blue_(index)  -= src_img_->get_pixel(i - 1, j)[2];
                }
                if(i < width-1){
                    if(index_(i + 1, j) >= 0)
                        triplet_list.push_back(Eigen::Triplet<double>(index, index_(i + 1, j), -1.0));
                    B_red_(index)   -= src_img_->get_pixel(i + 1, j)[0];
                    B_green_(index) -= src_img_->get_pixel(i + 1, j)[1];
                    B_blue_(index)  -= src_img_->get_pixel(i + 1, j)[2];
                }
                if(j > 0){
                    if(index_(i, j - 1) >= 0)
                        triplet_list.push_back(Eigen::Triplet<double>(index, index_(i, j - 1), -1.0));
                    B_red_(index)   -= src_img_->get_pixel(i, j - 1)[0];
                    B_green_(index) -= src_img_->get_pixel(i, j - 1)[1];
                    B_blue_(index)  -= src_img_->get_pixel(i, j - 1)[2];
                }
                if(j < height-1){
                    if(index_(i, j + 1) >= 0)
                        triplet_list.push_back(Eigen::Triplet<double>(index, index_(i, j + 1), -1.0));
                    B_red_(index)   -= src_img_->get_pixel(i, j + 1)[0];
                    B_green_(index) -= src_img_->get_pixel(i, j + 1)[1];
                    B_blue_(index)  -= src_img_->get_pixel(i, j + 1)[2];
                }
                int np = 4;
                np -= ((i == 0) + (i == (width - 1)) + (j == 0) + (j == (height - 1)));
                triplet_list.push_back(Eigen::Triplet<double>(index, index, np)); 
            }
        }
    }
    A_.setFromTriplets(triplet_list.begin(), triplet_list.end());
    solver_.compute(A_);
}

std::shared_ptr<Image> SeamlessClone::solve(int x, int y){
    int width = src_select_mask_->width();
    int height = src_select_mask_->height();
    for(int i = 0; i < width; i++){
        for(int j = 0; j < height; j++){
            int index = index_(i, j);
            if(index >= 0){
                if(i == 0 || (i > 0 && index_(i - 1, j) < 0)){
                    B_red_(index)   += tar_img_->get_pixel(x + i - 1, y + j)[0];
                    B_green_(index) += tar_img_->get_pixel(x + i - 1, y + j)[1];
                    B_blue_(index)  += tar_img_->get_pixel(x + i - 1, y + j)[2];
                }
                if(i == (width-1) || (i < width-1 && index_(i + 1, j) < 0)){
                    B_red_(index)   += tar_img_->get_pixel(x + i + 1, y + j)[0];
                    B_green_(index) += tar_img_->get_pixel(x + i + 1, y + j)[1];
                    B_blue_(index)  += tar_img_->get_pixel(x + i + 1, y + j)[2];
                }
                if(j == 0 || (j > 0 && index_(i, j - 1) < 0)){
                    B_red_(index)   += tar_img_->get_pixel(x + i, y + j - 1)[0];
                    B_green_(index) += tar_img_->get_pixel(x + i, y + j - 1)[1];
                    B_blue_(index)  += tar_img_->get_pixel(x + i, y + j - 1)[2];
                }
                if(j == (height-1) || (j < (height-1) && index_(i, j + 1) < 0)){
                    B_red_(index)   += tar_img_->get_pixel(x + i, y + j + 1)[0];
                    B_green_(index) += tar_img_->get_pixel(x + i, y + j + 1)[1];
                    B_blue_(index)  += tar_img_->get_pixel(x + i, y + j + 1)[2];
                }
            }
        }
    }

    r_red_.resize(num_);
    r_green_.resize(num_);
    r_blue_.resize(num_);
    r_red_ = solver_.solve(B_red_);
    r_green_ = solver_.solve(B_green_);
    r_blue_ = solver_.solve(B_blue_);

    res_img_ = std::make_shared<Image>(width, height, tar_img_->channels());
    for(int i = 0; i < width; i++){
        for(int j = 0; j < height; j++){
            int index = index_(i, j);
            if(index >= 0){
                std::vector<unsigned char> col;
                unsigned char r = (r_red_(index) > 255) ? 255 : ((r_red_(index) < 0) ? 0 : r_red_(index));
                unsigned char g = (r_green_(index) > 255) ? 255 : ((r_green_(index) < 0) ? 0 : r_green_(index));
                unsigned char b = (r_blue_(index) > 255) ? 255 : ((r_blue_(index) < 0) ? 0 : r_blue_(index));
                col.push_back(r);
                col.push_back(g);
                col.push_back(b);
                col.push_back(255);
                res_img_->set_pixel(i, j, col);
            }
            else {
                res_img_->set_pixel(i, j, tar_img_->get_pixel(i, j));
            }
        }
    }

    return res_img_;
}

}