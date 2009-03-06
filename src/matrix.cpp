#include "matrix.h"

#include <iostream>
#include <cassert>
#include <cstring>

Matrix::Matrix(const size_t size):
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
     // for (index_t i = 0; i < m_alloc_size; ++i)
     //      for (index_t j = 0; j < m_alloc_size; ++j)
     //           (*this)(i, j, 0);
     //memset(m_data, 0, m_alloc_size * m_alloc_size * sizeof(double));
}

Matrix::Matrix(const double *data, const size_t size):
     m_alloc_size(nextPowerOf2(size)),
     m_real_size(size),
     m_data(new double[m_alloc_size * m_alloc_size])
{
     for (index_t i = 0; i < m_real_size; ++i)
          for (index_t j = 0; j < m_real_size; ++j)
               (*this)(i, j, data[i * m_real_size + j]);
     //memcpy(m_data, data, m_alloc_size * m_alloc_size * sizeof(double));
}

Matrix::Matrix(const Matrix &m):
     m_alloc_size(m.m_alloc_size),
     m_real_size(m.m_real_size),
     m_data(new double[m_alloc_size * m_alloc_size])
{
     // for (index_t i = 0; i < m_alloc_size; ++i)
     //      for (index_t j = 0; j < m_alloc_size; ++j)
     //           (*this)(i, j, m(i, j));
     memcpy(m_data, m.m_data, m_alloc_size * m_alloc_size * sizeof(double));
}

Matrix::Matrix(const Matrix &c11, const Matrix &c12, const Matrix &c21, const Matrix &c22)
{
     assert(c11.size() == c12.size()
            && c12.size() == c21.size()
            && c21.size() == c22.size());

     m_alloc_size = c11.m_alloc_size * 2;
     m_real_size = c11.m_real_size * 2;
     m_data = new double[m_alloc_size * m_alloc_size];

     //memset(m_data, 0, m_alloc_size * m_alloc_size * sizeof(double));

     const size_t s = c11.m_real_size;

     for(index_t i = 0; i < s; ++i)
          for(index_t j = 0; j < s; ++j) {
               (*this)(i, j, c11(i, j));
               (*this)(i, j + s, c12(i, j));
               (*this)(i + s, j, c21(i, j));
               (*this)(i + s, j + s, c22(i, j));
          }
}

Matrix::~Matrix()
{
     delete[] m_data;
}

Matrix Matrix::operator*(const Matrix &m) const
{
     assert(size() == m.size());

     if (size() == 1) {
          Matrix r(1);
          r(0, 0, (*this)(0, 0) * m(0, 0));
          return r;
     }
     else {
          Matrix m1 = (slice11() + slice22()) * (m.slice11() + m.slice22());
          Matrix m2 = (slice21() + slice22()) *  m.slice11();
          Matrix m3 =  slice11()              * (m.slice12() - m.slice22());
          Matrix m4 =  slice22()              * (m.slice21() - m.slice11());
          Matrix m5 = (slice11() + slice12()) *  m.slice22();
          Matrix m6 = (slice21() - slice11()) * (m.slice11() + m.slice12());
          Matrix m7 = (slice12() - slice22()) * (m.slice21() + m.slice22());

          Matrix c11 = m1 + m4 - m5 + m7;
          Matrix c12 = m3 + m5;
          Matrix c21 = m2 + m4;
          Matrix c22 = m1 - m2 + m3 + m6;

          Matrix r = Matrix(c11, c12, c21, c22);
          r.m_real_size = m.m_real_size; // on remet la taille correcte
          return r;
     }
}

Matrix Matrix::operator+(const Matrix &m) const
{
     assert(size() == m.size());

     Matrix r(m.size());

     for(index_t i = 0; i < m_real_size; ++i)
          for(index_t j = 0; j < m_real_size; ++j)
               r(i, j, (*this)(i, j) + m(i, j));

     return r;
}

Matrix Matrix::operator-(const Matrix &m) const
{
     assert(size() == m.size());

     Matrix r(m.size());

     for(index_t i = 0; i < m_real_size; ++i)
          for(index_t j = 0; j < m_real_size; ++j)
               r(i, j, (*this)(i, j) - m(i, j));

     return r;
}

Matrix Matrix::slice11() const
{
     const size_t s = m_alloc_size / 2;
     Matrix m(s);

     for(index_t i = 0; i < s; ++i)
          for(index_t j = 0; j < s; ++j)
               m(i, j, (*this)(i, j));

     return m;
}

Matrix Matrix::slice12() const
{
     const size_t s = m_alloc_size / 2;
     Matrix m(s);

     for(index_t i = 0; i < s; ++i)
          for(index_t j = 0; j < s; ++j)
               m(i, j, (*this)(i, j + s));

     return m;
}

Matrix Matrix::slice21() const
{
     const size_t s = m_alloc_size / 2;
     Matrix m(s);

     for(index_t i = 0; i < s; ++i)
          for(index_t j = 0; j < s; ++j)
               m(i, j, (*this)(i + s, j));

     return m;
}

Matrix Matrix::slice22() const
{
     const size_t s = m_alloc_size / 2;
     Matrix m(s);

     for(index_t i = 0; i < s; ++i)
          for(index_t j = 0; j < s; ++j)
               m(i, j, (*this)(i + s, j + s));

     return m;
}

std::ostream & operator<<(std::ostream &os, const Matrix &m)
{
#ifdef DEBUG
     for(index_t i = 0; i < m.m_alloc_size; ++i) {
          os << "[";
          for(index_t j = 0; j < m.m_alloc_size; ++j) {
               os << m(i, j);
               if(j != m.m_alloc_size - 1)
                    os << " ";
          }
          os << "]" << std::endl;
     }
#else
     for(index_t i = 0; i < m.size(); ++i) {
          os << "[";
          for(index_t j = 0; j < m.size(); ++j) {
               os << m(i, j);
               if(j != m.size() - 1)
                    os << " ";
          }
          os << "]" << std::endl;
     }
#endif

     return os;
}
