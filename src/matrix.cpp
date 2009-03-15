#include "matrix.h"

#include <fstream>
#include <iomanip>
#include <iostream>

#include <cassert>
#include <cstdlib>

#include <cmath>

Matrix::Matrix():
     m_alloc_size(0),
     m_real_size(0),
     m_data(NULL)
{
}

Matrix::Matrix(const size_t size, const bool random):
     // L’algorithme de Strassen fonctionne avec des matrices carrées
     // dont la taille des côtés est une puissance de 2. Si la taille
     // spécifiée n'est pas une puissance de 2, on alloue quand même
     // une matrice dont la taille du côté est la puissance de 2
     // supérieure à la taille spécifiée. Les colonnes et les lignes
     // inutilisées sont remplies de 0.
     m_alloc_size(nextPowerOf2(size)),
     m_real_size(size),
     m_data(new double[m_alloc_size * m_alloc_size])
{
     for (index_t i = 0; i < m_real_size; ++i)
          for (index_t j = 0; j < m_real_size; ++j) {
               if(random)
                    (*this)(i, j, rand() % 10000);
               else
                    (*this)(i, j, 0);
          }

}

Matrix::Matrix(const double *data, const size_t size):
     m_alloc_size(nextPowerOf2(size)),
     m_real_size(size),
     m_data(new double[m_alloc_size * m_alloc_size])
{
     for (index_t i = 0; i < m_real_size; ++i)
          for (index_t j = 0; j < m_real_size; ++j)
               (*this)(i, j, data[i * m_real_size + j]);
}

Matrix::Matrix(std::ifstream &ifs)
{
     size_t size;
     ifs >> size;

     m_alloc_size = nextPowerOf2(size);
     m_real_size = size;
     m_data = new double[m_alloc_size * m_alloc_size];

     double tmp;
     for(index_t i = 0; i < m_real_size; ++i)
          for(index_t j = 0; j < m_real_size; ++j) {
               ifs >> tmp;
               (*this)(i, j, tmp);
          }
}

Matrix::Matrix(const Matrix &c00, const Matrix &c01, const Matrix &c10, const Matrix &c11):
     m_alloc_size(c11.allocSize() * 2),
     m_real_size(c11.size() * 2),
     m_data(new double[m_alloc_size * m_alloc_size])
{
     assert(c00.size() == c01.size()
            && c01.size() == c10.size()
            && c10.size() == c11.size());

     const size_t s = c00.size();

     for(index_t i = 0; i < s; ++i)
          for(index_t j = 0; j < s; ++j) {
               (*this)(i, j, c00(i, j));
               (*this)(i, j + s, c01(i, j));
               (*this)(i + s, j, c10(i, j));
               (*this)(i + s, j + s, c11(i, j));
          }
}

Matrix::Matrix(const Matrix &m):
     m_alloc_size(m.allocSize()),
     m_real_size(m.size()),
     m_data(new double[m_alloc_size * m_alloc_size])
{
     for (index_t i = 0; i < m_alloc_size; ++i)
          for (index_t j = 0; j < m_alloc_size; ++j)
               (*this)(i, j, m(i, j));
}

Matrix::~Matrix()
{
     delete[] m_data;
}

Matrix& Matrix::operator=(const Matrix &m)
{
     m_alloc_size = m.m_alloc_size;
     m_real_size = m.m_real_size;
     m_data = new double[m_alloc_size * m_alloc_size];

     for (index_t i = 0; i < m_alloc_size; ++i)
          for (index_t j = 0; j < m_alloc_size; ++j)
               (*this)(i, j, m(i, j));

     return *this;
}

Matrix Matrix::strassen(const Matrix &m, unsigned int deep) const
{
     assert(m_real_size == m.size());

     if (m_real_size == 1) {
          Matrix r(1);
          r(0, 0, (*this)(0, 0) * m(0, 0));
          return r;
     }
     else {
          const Matrix a00 = slice(0, 0);
          const Matrix a01 = slice(0, 1);
          const Matrix a10 = slice(1, 0);
          const Matrix a11 = slice(1, 1);

          const Matrix b00 = m.slice(0, 0);
          const Matrix b01 = m.slice(0, 1);
          const Matrix b10 = m.slice(1, 0);
          const Matrix b11 = m.slice(1, 1);

#ifdef OPENMP
          Matrix m1;
          Matrix m2;
          Matrix m3;
          Matrix m4;
          Matrix m5;
          Matrix m6;
          Matrix m7;

#pragma omp parallel sections if (deep < (log2(m_alloc_size)) / 2 )
          {
#pragma omp section
               m1 = (a00 + a11).strassen((b00 + b11), deep + 1);
#pragma omp section
               m2 = (a10 + a11).strassen(b00, deep + 1);
#pragma omp section
               m3 = a00.strassen((b01 - b11), deep + 1);
#pragma omp section
               m4 = a11.strassen((b10 - b00), deep + 1);
#pragma omp section
               m5 = (a00 + a01).strassen(b11, deep + 1);
#pragma omp section
               m6 = (a10 - a00).strassen((b00 + b01), deep + 1);
#pragma omp section
               m7 = (a01 - a11).strassen((b10 + b11), deep + 1);
          }
#else
          const Matrix m1 = (a00 + a11) * (b00 + b11);
          const Matrix m2 = (a10 + a11) * b00;
          const Matrix m3 = a00 * (b01 - b11);
          const Matrix m4 = a11 * (b10 - b00);
          const Matrix m5 = (a00 + a01) * b11;
          const Matrix m6 = (a10 - a00) * (b00 + b01);
          const Matrix m7 = (a01 - a11) * (b10 + b11);
#endif
          const Matrix c00 = m1 + m4 - m5 + m7;
          const Matrix c01 = m3 + m5;
          const Matrix c10 = m2 + m4;
          const Matrix c11 = m1 - m2 + m3 + m6;

          Matrix r(c00, c01, c10, c11);
          r.m_real_size = m.size(); // on remet la taille correcte
          return r;
     }
}

Matrix Matrix::operator+(const Matrix &m) const
{
     assert(m_real_size == m.size());

     Matrix r(m.size());

     for(index_t i = 0; i < m_real_size; ++i)
          for(index_t j = 0; j < m_real_size; ++j)
               r(i, j, (*this)(i, j) + m(i, j));

     return r;
}

Matrix Matrix::operator-(const Matrix &m) const
{
     assert(m_real_size == m.size());

     Matrix r(m.size());

     for(index_t i = 0; i < m_real_size; ++i)
          for(index_t j = 0; j < m_real_size; ++j)
               r(i, j, (*this)(i, j) - m(i, j));

     return r;
}

Matrix Matrix::slice(const index_t i, const index_t j) const
{
     assert((i == 0 || i == 1)
            && (j == 0 || j ==1));

     const size_t s = m_alloc_size / 2;
     Matrix m(s);

     for(index_t k = 0; k < s; ++k)
          for(index_t l = 0; l < s; ++l)
               m(k, l, (*this)(k + i * s, l + j * s));

     return m;
}

void Matrix::print(std::ostream &os) const
{
#ifdef DEBUG
     /*
      * Affiche la matrice complète (allouée) (avec les lignes et les
      * colonnes remplies de 0)
      */
     const size_t size = m_alloc_size;
#else
     /*
      * Affiche la matrice réelle (sans les lignes et les colonnes
      * remplies de 0)
      */
     const size_t size = m_real_size;
#endif

     for(index_t i = 0; i < size; ++i) {
          os << "[";
          for(index_t j = 0; j < size; ++j) {
               os << (*this)(i, j);
               if(j != size - 1)
                    os << " ";
          }
          os << "]" << std::endl;
     }
}

void Matrix::write(std::ofstream &ofs) const
{
     ofs << m_real_size << std::endl;
     for(index_t i = 0; i < m_real_size; ++i) {
          for(index_t j = 0; j < m_real_size; ++j) {
               ofs << std::fixed << (*this)(i, j);
               if(j != m_real_size - 1)
                    ofs << std::fixed << " ";
          }
          ofs << std::endl;
     }
}
