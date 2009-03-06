#ifndef MATRIX_H
#define MATRIX_H

#include <ostream>

typedef unsigned int index_t;
typedef unsigned int size_t;

class Matrix
{
     /*!
      * Permet d'afficher la matrice m via l'opérateur <<.
      */
     friend std::ostream & operator<<(std::ostream &os, const Matrix &m);

public:
     /*!
      * Constructeur de matrice carrée vide de taille size × size.
      */
     Matrix(const size_t size);

     /*!
      * Constructeur de matrice carrée de taille size × size à partir des données
      * contenues dans le tableau data.
      */
     Matrix(const double *data, const size_t size);

     /*!
      * Constructeur de recopie
      */
     Matrix(const Matrix &m);

     /*!
      * Constructeur de matrice à partir de 4 sous-matrices.
      * La nouvelle matrice construite est :
      * -------------
      * | c11 | c12 |
      * |-----------|
      * | c21 | c22 |
      * -------------
      */
     Matrix(const Matrix &c11, const Matrix &c12, const Matrix &c21, const Matrix &c22);

     /*!
      * Desctructeur de matrice
      */
     ~Matrix();

     /*!
      * Multiplie par la matrice m en utilisant l'algorithme de Strassen
      */
     Matrix operator*(const Matrix &m) const;

     /*!
      * Ajoute la matrice m
      */
     Matrix operator+(const Matrix &m) const;

     /*!
      * Soustrait la matrice m
      */
     Matrix operator-(const Matrix &m) const;

     /*!
      * Retourne l'élément à la position i,j
      */
     inline double operator()(const size_t i, const size_t j) const
     {
          return m_data[i * m_alloc_size + j];
     }

     /*!
      * Met la valeur b à la position i,j
      */
     inline void operator()(const size_t i, const size_t j, const double v)
     {
          m_data[i * m_alloc_size + j] = v;
     }

     /*!
      * Retourne la taille de la matrice
      */
     inline size_t size() const
     {
          return m_real_size;
     }

//private: FIXME
     /*
      * -------------
      * | c11 | c12 |
      * |-----------|
      * | c21 | c22 |
      * -------------
      */

     /*!
      * Retourne la sous-matrice c11
      */
     Matrix slice11() const;

     /*!
      * Retourne la sous-matrice c12
      */
     Matrix slice12() const;

     /*!
      * Retourne la sous-matrice c21
      */
     Matrix slice21() const;

     /*!
      * Retourne la sous-matrice c22
      */
     Matrix slice22() const;

     size_t m_alloc_size;
     size_t m_real_size;
     double* m_data;
};

/*!
 * Retourne la puissance de 2 supérieur ou égale à v
 */
inline unsigned int nextPowerOf2(const size_t v)
{
     unsigned int powof2 = 1;
     while(powof2 < v)
          powof2 *= 2;

     return powof2;
}

#endif
