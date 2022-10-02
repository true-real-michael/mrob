/* Copyright (c) 2022, Gonzalo Ferrer
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * factor1PosePoint2Point.cpp
 *
 *  Created on: Dec 29, 2020
 *      Author: Gonzalo Ferrer
 *              g.ferrer@skoltech.ru
 *              Mobile Robotics Lab.
 */


#include "mrob/factors/factor1PosePoint2Point.hpp"

#include <iostream>


using namespace mrob;

Factor1PosePoint2Point::Factor1PosePoint2Point(const Mat31 &z_point_x, const Mat31 &z_point_y,  std::shared_ptr<Node> &node,
            const Mat3 &obsInf, Factor::robustFactorType robust_type):
        Factor(3,6,robust_type), z_point_x_(z_point_x), z_point_y_(z_point_y), W_(obsInf)
{
    neighbourNodes_.push_back(node);
}

void Factor1PosePoint2Point::evaluate_residuals()
{
    // r = Tx - y
    Mat4 Tnode = get_neighbour_nodes()->at(0)->get_state();
    SE3 T = SE3(Tnode);
    Tx_ = T.transform(z_point_x_);
    r_ = Tx_ - z_point_y_;
}

void Factor1PosePoint2Point::evaluate_jacobians()
{
    // it assumes you already have evaluated residuals
    J_ << -hat3(Tx_) , Mat3::Identity();
}

void Factor1PosePoint2Point::evaluate_chi2()
{
    chi2_ = 0.5 * r_.dot(W_ * r_);
}

void Factor1PosePoint2Point::print() const
{
    std::cout << "Printing Factor: " << id_ << ", obs point x= \n" << z_point_x_
              << "\nobs point y =\n" << z_point_y_
              << "\n Residuals= \n" << r_
              << " \nand Information matrix\n" << W_
              << "\n Calculated Jacobian = \n" << J_
              << "\n Chi2 error = " << chi2_
              << " and neighbour Node ids: " << neighbourNodes_[0]->get_id()
              << std::endl;
}

