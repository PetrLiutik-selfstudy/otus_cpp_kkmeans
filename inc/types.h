#pragma once

#include <dlib/clustering.h>

#include <vector>
#include <map>

using sample_t = dlib::matrix<double,2,1> ;
using kernel_t = dlib::radial_basis_kernel<sample_t>;
using plot_data_t = std::map<int, std::vector<std::pair<double, double>>>;