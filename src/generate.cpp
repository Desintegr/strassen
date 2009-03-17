#include "matrix.h"

#include <fstream>
#include <iostream>

#include <cstdlib>

int main(int argc, char **argv)
{
     if(argc != 3) {
          std::cout << "Usage: " << argv[0] << " size outputfile" << std::endl;
          return EXIT_FAILURE;
     }

     srand(time(NULL) + getpid());

     const int size = atoi(argv[1]);
     if(size <= 0) {
          std::cout << "Error: size must be greater than 0" << std::endl;
          return EXIT_FAILURE;
     }

     const Matrix m(size, true);
     std::ofstream out;
     out.open(argv[2]);
     if(!out.is_open()) {
          std::cout << "Error: cannot write to file " << argv[2] << std::endl;
          return EXIT_FAILURE;
     }
     m.write(out);
     out.close();

     return EXIT_SUCCESS;
}
