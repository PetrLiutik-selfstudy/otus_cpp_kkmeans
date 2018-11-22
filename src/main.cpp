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

  if(argc != 2) {
    std::cerr << "Wrong number of arguments (expected 1). Usage: kkmeans <n>. \n" << std::endl;
    return EXIT_FAILURE;
  }

  auto n = std::strtoll(argv[1], nullptr, 0);
  if(n <= 0) {
    std::cerr << "Parameter n must be integer greater than 0.\n";
    return EXIT_FAILURE;
  }


  typedef matrix<double,2,1> sample_type;

  typedef radial_basis_kernel<sample_type> kernel_type;

  kcentroid<kernel_type> kc(kernel_type(0.1),0.01, 8);

  kkmeans<kernel_type> test(kc);

  std::vector<sample_type> samples;
  std::vector<sample_type> initial_centers;


  sample_type m;

  dlib::rand rnd;



  // we will make 50 points from each class
  const long num = 50;

  // make some samples near the origin
  double radius = 0.5;
  for (long i = 0; i < num; ++i)
  {
    double sign = 1;
    if (rnd.get_random_double() < 0.5)
      sign = -1;
    m(0) = 2*radius*rnd.get_random_double()-radius;
    m(1) = sign*sqrt(radius*radius - m(0)*m(0));

    // add this sample to our set of samples we will run k-means
    samples.push_back(m);
  }

  // make some samples in a circle around the origin but far away
  radius = 10.0;
  for (long i = 0; i < num; ++i)
  {
    double sign = 1;
    if (rnd.get_random_double() < 0.5)
      sign = -1;
    m(0) = 2*radius*rnd.get_random_double()-radius;
    m(1) = sign*sqrt(radius*radius - m(0)*m(0));

    // add this sample to our set of samples we will run k-means
    samples.push_back(m);
  }

  // make some samples in a circle around the point (25,25)
  radius = 4.0;
  for (long i = 0; i < num; ++i)
  {
    double sign = 1;
    if (rnd.get_random_double() < 0.5)
      sign = -1;
    m(0) = 2*radius*rnd.get_random_double()-radius;
    m(1) = sign*sqrt(radius*radius - m(0)*m(0));

    // translate this point away from the origin
    m(0) += 25;
    m(1) += 25;

    // add this sample to our set of samples we will run k-means
    samples.push_back(m);
  }

  // tell the kkmeans object we made that we want to run k-means with k set to 3.
  // (i.e. we want 3 clusters)
  test.set_number_of_centers(3);

  // You need to pick some initial centers for the k-means algorithm.  So here
  // we will use the dlib::pick_initial_centers() function which tries to find
  // n points that are far apart (basically).
  pick_initial_centers(3, initial_centers, samples, test.get_kernel());

  // now run the k-means algorithm on our set of samples.
  test.train(samples,initial_centers);

  // now loop over all our samples and print out their predicted class.  In this example
  // all points are correctly identified.
  for (unsigned long i = 0; i < samples.size()/3; ++i)
  {
    std::cout << test(samples[i]) << " ";
    std::cout << test(samples[i+num]) << " ";
    std::cout << test(samples[i+2*num]) << "\n";
  }

  // Now print out how many dictionary vectors each center used.  Note that
  // the maximum number of 8 was reached.  If you went back to the kcentroid
  // constructor and changed the 8 to some bigger number you would see that these
  // numbers would go up.  However, 8 is all we need to correctly cluster this dataset.
  std::cout << "num dictionary vectors for center 0: " << test.get_kcentroid(0).dictionary_size() << std::endl;
  std::cout << "num dictionary vectors for center 1: " << test.get_kcentroid(1).dictionary_size() << std::endl;
  std::cout << "num dictionary vectors for center 2: " << test.get_kcentroid(2).dictionary_size() << std::endl;


  // Finally, we can also solve the same kind of non-linear clustering problem with
  // spectral_cluster().  The output is a vector that indicates which cluster each sample
  // belongs to.  Just like with kkmeans, it assigns each point to the correct cluster.
  std::vector<unsigned long> assignments = spectral_cluster(kernel_type(0.1), samples, 3);
  std::cout << mat(assignments) << std::endl;

  return 0;
}
