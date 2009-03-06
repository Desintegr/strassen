#include "matrix.h"

#include <iomanip>
#include <iostream>

#include <cstdlib>
#include <ctime>

double diff(clock_t start, clock_t stop){
     return (double)(stop - start) / (double)(CLOCKS_PER_SEC);
}

int main(int argc, char**argv)
{
     srand(time(NULL));

     size_t size = atoi(argv[1]);
     size_t size2 = size * size;
     double* d1 = new double[size2];
     double* d2 = new double[size2];

     clock_t start = clock();

     unsigned int nbit = atoi(argv[2]);
     for(index_t it = 0; it < nbit; ++it) {

          for(index_t i = 0; i < size2; ++i) {
               d1[i] = rand() % 100;
               d2[i] = rand() % 100;
          }

          Matrix m1(d1, size);

          // FIXME remove
          // for(int i=0;i<size*size;i++) {
          //      std::cout << d1[i] << " ";
          // }
          // std::cout << std::endl;

          Matrix m2(d2, size);
          Matrix m3 = m1 * m2;

          // FIMXE remove
          // std::cout << m1 << std::endl;
          // std::cout << m2 << std::endl;
          // std::cout << m3;

     }

     clock_t stop = clock();
     double dif = diff(start, stop);

     std::cout << std::setprecision(2) << std::fixed << dif << " secs" << " - "
               << std::setprecision(0) << std::fixed << nbit / dif << " m/secs" << std::endl;

     return EXIT_SUCCESS;
}
