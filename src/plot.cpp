#include "plot.h"

#include "../gnuplot/gnuplot-iostream.h"

void plot(const plot_data_t& plot_data) {
  Gnuplot gp("gnuplot -persist");

  gp << "set xrange [-100:100]\nset yrange [-100:100]\n";
  gp << "plot";
  for(const auto& it: plot_data)
    gp << gp.file1d(it.second) << "with points pointtype 7 ps 2 title 'cluster" << +it.first << "',";
  gp << std::endl;
}