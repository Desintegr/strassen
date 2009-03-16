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
      * Constructeur de matrice par défaut (de taille 0)
      */
     Matrix();

     /*!
      * Constructeur de matrice carrée de taille size × size
      * @param random si vrai, remplit la matrice avec des données
      * aléatoires
      */
     Matrix(const size_t size, const bool random = false);

     /*!
      * Constructeur de matrice à partir d'un flux de fichier
      */
     Matrix(std::ifstream &ifs);

     /*!
      * Constructeur de recopie
      */
     Matrix(const Matrix &m);

     /*!
      * Constructeur par déplacement
      */
     Matrix(Matrix &&m);

     /*!
      * Destructeur de matrice
      */
     ~Matrix();

     /*!
      * Affectation par déplacement
      */
     Matrix& operator=(Matrix &&m);

     /*!
      * Multiplie par la matrice m
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
     double operator()(const size_t i, const size_t j) const;

     /*!
      * Met la valeur v à la position i,j
      */
     void operator()(const size_t i, const size_t j, const double v);

     /*!
      * Retourne la taille réelle de la matrice
      */
     size_t size() const;

     /*!
      * Permet d'afficher la matrice dans un flux
      */
     void print(std::ostream &os = std::cout) const;

     /*!
      * Écrit la matrice dans un flux de fichier
      */
     void write(std::ofstream &ofs) const;

private:
     /*!
      * Constructeur de matrice à partir de 4 sous-matrices
      *
      * La nouvelle matrice construite est :
      * -------------
      * | c00 | c01 |
      * |-----------|
      * | c10 | c11 |
      * -------------
      */
     Matrix(const Matrix &c00, const Matrix &c01, const Matrix &c10, const Matrix &c11);

     /*!
      * Multiplie par la matrice m en utilisant l'algorithme de
      * Strassen
      * @param profondeur de la récursion parallèle
      */
     Matrix strassen(const Matrix &m, unsigned int deep = 0) const;

     /*!
      * Retourne la sous-matrice ij
      * -----------
      * | 00 | 01 |
      * |---------|
      * | 10 | 11 |
      * -----------
      */
     Matrix slice(const index_t i, const index_t j) const;

     /*!
      * Retour la taille allouée de la matrice
      */
     size_t allocSize() const;

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
     double *m_data;
};

inline Matrix Matrix::operator*(const Matrix &m) const
{
     return strassen(m);
}

inline double Matrix::operator()(const size_t i, const size_t j) const
{
     return m_data[i * m_alloc_size + j];
}

inline void Matrix::operator()(const size_t i, const size_t j, const double v)
{
     m_data[i * m_alloc_size + j] = v;
}

inline size_t Matrix::size() const
{
     return m_real_size;
}

inline size_t Matrix::allocSize() const
{
     return m_alloc_size;
}

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
