#include "matrix.h"

#include <iostream>
#include <fstream>

#include <cstdlib>

using std::cout;
using std::cin;
using std::endl;
using std::ifstream;
using std::ofstream;

int main(int argc, char **argv)
{
     ifstream in;
     size_t size;
     int tmp;

     in.open(argv[1]);
     
     in >> size;

     Matrix r1(size);

     for(index_t i = 0; i < size; ++i)
          for(index_t j = 0; j < size; ++j) {
               in >> tmp;
               r1(i, j, tmp);
          }

     in.close();


     in.open(argv[2]);

     in >> size;

     Matrix r2(size);
     for(index_t i = 0; i < size; ++i)
          for(index_t j = 0; j < size; ++j) {
               in >> tmp;
               r2(i, j, tmp);
          }
     
     in.close();

     ofstream out;
     out.open(argv[3]);
     
     Matrix r3 = r1 * r2;
     
     out << r3.size() << endl;
     for(index_t i = 0; i < size; ++i) {
          for(index_t j = 0; j < size; ++j) {
               out << r3(i, j);
               if(j != size -1)
                    out << " ";
          }
          out << endl;
     }

     out.close();
     
     return EXIT_SUCCESS;
}
