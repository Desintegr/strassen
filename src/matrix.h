#ifndef MATRIX_H
#define MATRIX_H

#include <fstream>
#include <iostream>

typedef unsigned int index_t;
typedef unsigned int size_t;

class Matrix
{
public:
     /*!
      * Constructeur de matrice carrée vide de taille size × size
      * @param random si vrai, remplit la matrice avec des données
      * aléatoires
      */
     Matrix(const size_t size, const bool random = false);

     /*!
      * Constructeur de matrice carrée de taille size × size à partir
      * des données contenues dans le tableau data
      */
     Matrix(const double *data, const size_t size);

     /*!
      * Constructeur de matrice à partir d'un flux de fichier
      */
     Matrix(std::ifstream &ifs);

     /*!
      * Constructeur de matrice à partir de 4 sous-matrices
      *
      * La nouvelle matrice construite est :
      * -------------
      * | c11 | c12 |
      * |-----------|
      * | c21 | c22 |
      * -------------
      */
     Matrix(const Matrix &c11, const Matrix &c12, const Matrix &c21, const Matrix &c22);

     /*!
      * Constructeur de recopie
      */
     Matrix(const Matrix &m);

     /*!
      * Desctructeur de matrice
      */
     ~Matrix();

     /*!
      * Multiplie par la matrice m en utilisant l'algorithme de
      * Strassen
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
      * Met la valeur v à la position i,j
      */
     inline void operator()(const size_t i, const size_t j, const double v)
     {
          m_data[i * m_alloc_size + j] = v;
     }

     /*!
      * Retourne la taille réelle de la matrice
      */
     inline size_t size() const
     {
          return m_real_size;
     }

     /*!
      * Permet d'afficher la matrice dans un flux
      */
     void print(std::ostream &os) const;

     /*!
      * Écrit la matrice dans un flux de fichier
      */
     void write(std::ofstream &ofs) const;

private:
     /*!
      * Retour la taille allouée de la matrice
      */
     inline size_t allocSize() const
     {
          return m_alloc_size;
     }

     /*
      * -----------
      * | 11 | 12 |
      * |---------|
      * | 21 | 22 |
      * -----------
      */

     /*!
      * Retourne la sous-matrice 11
      */
     Matrix slice11() const;

     /*!
      * Retourne la sous-matrice 12
      */
     Matrix slice12() const;

     /*!
      * Retourne la sous-matrice 21
      */
     Matrix slice21() const;

     /*!
      * Retourne la sous-matrice 22
      */
     Matrix slice22() const;

     /*!
      * La taille allouée
      */
     size_t m_alloc_size;

     /*!
      * La taille réelle
      */
     size_t m_real_size;

     /*!
      * Les données
      */
     double* m_data;
};

/*!
 * Retourne la puissance de 2 supérieure ou égale à v
 */
inline unsigned int nextPowerOf2(const size_t v)
{
     unsigned int powof2 = 1;
     while(powof2 < v)
          powof2 *= 2;

     return powof2;
}

#endif
