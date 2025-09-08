#pragma once

#include "common/image.h"
#include <eigen/Eigen/Sparse>

namespace USTC_CG{

class SeamlessClone{
public:
    SeamlessClone() = default;
    SeamlessClone(std::shared_ptr<Image> src_img,
                std::shared_ptr<Image> tar_img,
                std::shared_ptr<Image> src_select_mask)
                : src_img_(src_img),
                  tar_img_(tar_img),
                  src_select_mask_(src_select_mask){
        initialize();
    }

    void initialize();
    std::shared_ptr<Image> solve(int x, int y);


private:
    std::shared_ptr<Image> src_img_;
    std::shared_ptr<Image> tar_img_;
    std::shared_ptr<Image> src_select_mask_;
    std::shared_ptr<Image> res_img_;
    int num_;
    Eigen::MatrixXi index_;
    Eigen::SparseMatrix<double> A_;
    Eigen::SimplicialLDLT<Eigen::SparseMatrix<double>> solver_;
    Eigen::VectorXd B_red_;
    Eigen::VectorXd B_green_;
    Eigen::VectorXd B_blue_;
    Eigen::VectorXd r_red_;
    Eigen::VectorXd r_green_;
    Eigen::VectorXd r_blue_;

};

}