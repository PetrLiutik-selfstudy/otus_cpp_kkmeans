#include "ver.h"


#include <iostream>
#include <vector>

#include <dlib/clustering.h>
#include <dlib/rand.h>

using namespace dlib;

int main(int argc, char const *argv[]) {
  std::cout << "kkmeans version: "
            << ver_major() << "."
            << ver_minor() << "."
            << ver_patch() << std::endl;

  if((argc != 2) && (argc != 3)) {
    std::cerr << "Wrong number of arguments (expected 1 or 2). Usage: kkmeans <n> [-g]. \n" << std::endl;
    return EXIT_FAILURE;
  }

  auto centers_num = std::strtoll(argv[1], nullptr, 0);
  if(centers_num <= 0) {
    std::cerr << "Parameter n must be integer greater than 0.\n";
    return EXIT_FAILURE;
  }

  bool make_plot = false;
  if(argc == 3) {
    std::string arg2(argv[2]);
    make_plot = (arg2 == "-g");
  }


  using sample_t = matrix<double,2,1> ;
  using kernel_t = radial_basis_kernel<sample_t> ;


  // Тестовая модель на основе kcentroid.
  kcentroid<kernel_t> kc(kernel_t(0.1),0.01, 8);
  kkmeans<kernel_t> test_model(kc);

  std::vector<sample_t> samples;
  std::vector<sample_t> initial_centers;


  // Чтение координат кластеризуемых точек.
  sample_t sample;
  for(std::string x_str{}, y_str{}; std::getline(std::cin, x_str, ';') && std::getline(std::cin, y_str);) {
    try {
      sample(0) = std::stod(x_str);
      sample(1) = std::stod(y_str);
      samples.emplace_back(sample);
    } catch (const std::exception& e) {
      std::cerr << e.what() << std::endl;
      return EXIT_FAILURE;
    }
  }

  // Установка количества центров кластеризации.
  test_model.set_number_of_centers(centers_num);
  pick_initial_centers(centers_num, initial_centers, samples, test_model.get_kernel());

  // Тренировка тестовой модели.
  test_model.train(samples, initial_centers);


  // Вывод кластеризованных данных.
  for(const auto& sample: samples) {
    auto cluster = test_model(sample);
    std::cout << sample(0) << ";" << sample(1) << ";cluster" << cluster << std::endl;
  }

  // Рисование
  if(make_plot) {
    std::cout << "Making a plot..." ;
    // TODO make a plot.

    std::cout << "OK" << std::endl;
  }

  return 0;
}
