#include "matrix.h"

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
  const Matrix m1(in);
  in.close();

  in.open(argv[2]);
  const Matrix m2(in);
  in.close();

  std::ofstream out;
  out.open(argv[3]);
  Matrix m3 = m1 * m2;
  m3.write(out);
  out.close();

  return EXIT_SUCCESS;
}
