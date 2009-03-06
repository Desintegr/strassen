#include "matrix.h"

#include <iostream>
#include <fstream>

#include <cstdlib>

int main(int argc, char **argv)
{
     std::ifstream in;
     size_t size;
     double tmp;

     in.open(argv[1]);
     
     in >> size;

     Matrix m1(size);

     for(index_t i = 0; i < size; ++i)
          for(index_t j = 0; j < size; ++j) {
               in >> tmp;
               m1(i, j, tmp);
          }

     in.close();

     in.open(argv[2]);

     in >> size;

     Matrix m2(size);
     for(index_t i = 0; i < size; ++i)
          for(index_t j = 0; j < size; ++j) {
               in >> tmp;
               m2(i, j, tmp);
          }
     
     in.close();

     std::ofstream out;
     out.open(argv[3]);
     
     Matrix m3 = m1 * m2;
     
     out << size << std::endl;
     for(index_t i = 0; i < size; ++i) {
          for(index_t j = 0; j < size; ++j) {
               out << m3(i, j);
               if(j != size -1)
                    out << " ";
          }
          out << std::endl;
     }

     out.close();
     
     return EXIT_SUCCESS;
}
