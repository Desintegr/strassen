#include "matrix.h"

#include <fstream>
#include <iomanip>
#include <iostream>

#include <cassert>
#include <cstdlib>

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
     if(random) {
          for (index_t i = 0; i < m_real_size; ++i)
               for (index_t j = 0; j < m_real_size; ++j)
                    (*this)(i, j, rand() % 10000);
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

Matrix::Matrix(const Matrix &c11, const Matrix &c12, const Matrix &c21, const Matrix &c22):
     m_alloc_size(c11.allocSize() * 2),
     m_real_size(c11.size() * 2),
     m_data(new double[m_alloc_size * m_alloc_size])
{
     assert(c11.size() == c12.size()
            && c12.size() == c21.size()
            && c21.size() == c22.size());

     const size_t s = c11.size();

     for(index_t i = 0; i < s; ++i)
          for(index_t j = 0; j < s; ++j) {
               (*this)(i, j, c11(i, j));
               (*this)(i, j + s, c12(i, j));
               (*this)(i + s, j, c21(i, j));
               (*this)(i + s, j + s, c22(i, j));
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

Matrix Matrix::operator*(const Matrix &m) const
{
     assert(m_real_size == m.size());

     if (m_real_size == 1) {
          Matrix r(1);
          r(0, 0, (*this)(0, 0) * m(0, 0));
          return r;
     }
     else {
          const Matrix a11 = slice11();
          const Matrix a12 = slice12();
          const Matrix a21 = slice21();
          const Matrix a22 = slice22();

          const Matrix b11 = m.slice11();
          const Matrix b12 = m.slice12();
          const Matrix b21 = m.slice21();
          const Matrix b22 = m.slice22();

          const Matrix m1 = (a11 + a22) * (b11 + b22);
          const Matrix m2 = (a21 + a22) *  b11;
          const Matrix m3 =        a11  * (b12 - b22);
          const Matrix m4 =        a22  * (b21 - b11);
          const Matrix m5 = (a11 + a12) *  b22;
          const Matrix m6 = (a21 - a11) * (b11 + b12);
          const Matrix m7 = (a12 - a22) * (b21 + b22);

          const Matrix c11 = m1 + m4 - m5 + m7;
          const Matrix c12 = m3 + m5;
          const Matrix c21 = m2 + m4;
          const Matrix c22 = m1 - m2 + m3 + m6;

          Matrix r(c11, c12, c21, c22);
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

void Matrix::print(std::ostream &os) const
{
#ifdef DEBUG
     /*
      * Affiche la matrice complète (allouée) (avec les lignes et les
      * colonnes remplies de 0)
      */
     for(index_t i = 0; i < m_alloc_size; ++i) {
          os << "[";
          for(index_t j = 0; j < m_alloc_size; ++j) {
               os << (*this)(i, j);
               if(j != m_alloc_size - 1)
                    os << " ";
          }
          os << "]" << std::endl;
     }
#else
     /*
      * Affiche la matrice réelle (sans les lignes et les colonnes
      * remplies de 0)
      */
     for(index_t i = 0; i < m_real_size; ++i) {
          os << "[";
          for(index_t j = 0; j < m_real_size; ++j) {
               os << (*this)(i, j);
               if(j != m_real_size - 1)
                    os << " ";
          }
          os << "]" << std::endl;
     }
#endif
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
