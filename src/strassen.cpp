#include "matrix.h"

#ifdef OPENMP
#include <omp.h>
#endif

#include <fstream>
#include <iostream>

#include <cstdlib>

int main(int argc, char **argv)
{
     if(argc != 4) {
          std::cout << "Usage: " << argv[0] << " inputfile1 inputfile2 outputfile " << std::endl;
          return EXIT_FAILURE;
     }

     std::ifstream in;

     in.open(argv[1]);
     if(!in.is_open()) {
          std::cout << "Error: cannot read file " << argv[1] << std::endl;
          return EXIT_FAILURE;
     }
     const Matrix m1(in);
     in.close();

     in.open(argv[2]);
     if(!in.is_open()) {
          std::cout << "Error: cannot read file " << argv[2] << std::endl;
          return EXIT_FAILURE;
     }
     const Matrix m2(in);
     in.close();

     std::ofstream out;
     out.open(argv[3]);
     if(!out.is_open()) {
          std::cout << "Error: cannot write to file " << argv[3] << std::endl;
          return EXIT_FAILURE;
     }

#ifdef OPENMP
     // on active les sections parallèles imbriquées
     omp_set_nested(true);
#endif

     Matrix m3 = m1 * m2;
     m3.write(out);
     out.close();

     return EXIT_SUCCESS;
}
