#include "matrix.h"

#include <iomanip>
#include <iostream>

#include <cstdlib>

inline static double diff(clock_t start, clock_t stop) {
     return (double)(stop - start) / (double)(CLOCKS_PER_SEC);
}

int main(int argc, char**argv)
{
     if(argc != 3) {
          std::cout << "Usage: " << argv[0] << " size iteration" << std::endl;
          return EXIT_FAILURE;
     }

     srand(time(NULL) + getpid());

     const int size = atoi(argv[1]);
     if(size <= 0) {
          std::cout << "Error: size must be greater than 0" << std::endl;
          return EXIT_FAILURE;
     }

     const int it = atoi(argv[2]);
     if(it <= 0) {
          std::cout << "Error: number of iterations must be greater than 0" << std::endl;
          return EXIT_FAILURE;
     }

     const clock_t start = clock();

     for(index_t i = 0; i < (unsigned int) it; ++i) {
          Matrix m1(size, true);
          Matrix m2(size, true);
          Matrix m3 = m1 * m2;
     }

     const clock_t stop = clock();
     const double dif = diff(start, stop);

     std::cout << std::setprecision(2) << std::fixed << dif << " secs" << " - "
               << std::setprecision(0) << std::fixed << it / dif << " m/secs" << std::endl;

     return EXIT_SUCCESS;
}
