#include "matrix.h"

#ifdef _OPENMP
#include <omp.h>
#endif

#include <iostream>

#include <cstdlib>

int main(int argc, char **argv)
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

#ifdef _OPENMP
     // on active les sections parallèles imbriquées
     omp_set_nested(true);
#endif

     for(index_t i = 0; i < (unsigned int) it; ++i) {
          const Matrix m1(size, true);
          const Matrix m2(size, true);
          const Matrix m3 = m1 * m2;
     }

     return EXIT_SUCCESS;
}
