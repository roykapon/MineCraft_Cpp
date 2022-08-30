#pragma once

#include <bits/stdc++.h>
#include <iomanip>
#include <iostream>
#include <limits.h>
#include <math.h>

// #define INT_MAX numeric_limits<int>::max()
// #define INT_MIN numeric_limits<int>::min()
#define DOUBLE_MAX numeric_limits<double>::max()
#define DOUBLE_MIN numeric_limits<double>::min()

#define X_AXIS 0
#define Y_AXIS 1
#define Z_AXIS 2
#define W 3
#define TEXTURE_X 2
#define TEXTURE_Y 3
#define AXES 3
#define PIXEL_AXES 4
#define EXTENDED_AXES 4
#define MAX_NUM_LENGTH 13
#define PI 3.14159265
#define EPSILON 0.0001

#define INTERPOLATE(X1, X2, RATIO)                                             \
  ((((double)X1) * RATIO) + (((double)X2) * (1.0f - RATIO)))

using namespace std;

class Vector {

public:
  double x, y, z, w;
  int texture_x, texture_y;

  Vector(double _x = 0, double _y = 0, double _z = 0, double _w = 1,
         int _texture_x = 0, int _texture_y = 0) {
    x = _x;
    y = _y;
    z = _z;
    w = _w;
    texture_x = _texture_x;
    texture_y = _texture_y;
  }

  /** Returns the value at the given index (or axis)*/
  double operator[](int index) const;

  /** Returns a mutable reference to the value at the given index (or axis) */
  double &operator[](int index);

  /** Returns true if the vectors have the same 3d position (does not compare
   * the w coordinate) */
  bool operator==(const Vector &b2) const;

  /** Returns true if the vectors so not have the same 3d position (does not
   * compare the w coordinate) */
  bool operator!=(const Vector &b2) const;
};
/** Prints the vector */
ostream &operator<<(ostream &os, const Vector &v);

/** Returns the sum of the vectors */
Vector operator+(const Vector &v1, const Vector &v2);

/** Returns the difference between the vectors (first-second) */
Vector operator-(const Vector &v1, const Vector &v2);

/** Returns the vector multiplied by a scalar */
Vector operator*(const Vector &v, double a);

/** Returns the vector multiplied by a scalar */
Vector operator*(double a, const Vector &v);

/** Returns the point between the vectors with the given ratio */
Vector interpolate(const Vector &v1, const Vector &v2, double ratio);

/** Returns the dot product of the vectors */
double operator*(const Vector &v1, const Vector &v2);

/** Returns the l2norm of the vector (its size)*/
double norm(const Vector &v);

/** Returns the normalized vector (same direction, with norm 1) */
Vector normalized(const Vector &v);

/** Returns the normalized vector in the x, z axes */
Vector normalized_horizontal(const Vector &v);

/** Returns the distance between the vectors */
double distance(const Vector &v1, const Vector &v2);

/** Returns the *normalized* cross-product between the vectors */
Vector cross(const Vector &v1, const Vector &v2);

/** Vector hash function */
struct Vector_hash {
  size_t operator()(const Vector &key) const;
};

/** returns the vector rounded to the closest block */
Vector round(Vector &v);

// ============================= Pixel ===================================

class Pixel {

public:
  int x;
  int y;
  Vector source;

  Pixel(int _x = 0, int _y = 0, Vector _source = Vector()) {
    x = _x;
    y = _y;
    source = _source;
  }

  /** Returns the value a the given axis*/
  int &operator[](int index);
};

/** Returns the sum of the pixels (sources also summed) */
Pixel operator+(const Pixel &p1, const Pixel &p2);

/** Returns the difference of the pixels (sources also substracted) */
Pixel operator-(const Pixel &p1, const Pixel &p2);

/** Returns the pixel multyplied by a scalar (source also multyplied) */
Pixel operator*(const Pixel &p, double a);

/** Prints the pixel */
ostream &operator<<(ostream &os, Pixel &p);

/** Returns an array in which the first value is the minimum of all y values
 * and the second value is the maximum of all y values */
void get_extremum(Pixel *pixels, int len, int *extremum,
                  int lower_bound = INT_MIN, int upper_bound = INT_MAX);

// ============================ Matrix ============================

class Matrix {

public:
  Vector vectors[4];

  Matrix(Vector v1 = Vector(0, 0, 0, 0), Vector v2 = Vector(0, 0, 0, 0),
         Vector v3 = Vector(0, 0, 0, 0), Vector v4 = Vector(0, 0, 0, 0)) {
    vectors[0] = v1;
    vectors[1] = v2;
    vectors[2] = v3;
    vectors[3] = v4;
  }

  /** Returns a mutable reference to the row at the given index */
  const Vector &operator[](int index) const;

  /** Returns the row at the given index */
  Vector &operator[](int index);

  /** Returns the rotation matrix around the given axis
   *  (when applied on the RIGHT)
   *  The rotation direction is clockwise when looking towards the given axis
   */
  static Matrix rotate(int axis, double angle);

  /** Returns the shifting matrix, assuming w=1
   *  (when applied on the RIGHT) */
  static Matrix shift(const Vector &offset);
};

/** Prints the matrix */
ostream &operator<<(ostream &os, const Matrix &M);

/** Returns the sum of the two matrices */
Matrix operator+(const Matrix &M1, const Matrix &M2);

/** Returns the difference of the matrices */
Matrix operator-(const Matrix &M1, const Matrix &M2);

/** Returns the vector multiplied by a matrix (from the LEFT) */
Vector operator*(const Matrix &M, const Vector &v);

/** Returns the vector multiplied by a matrix (from the RIGHT)*/
Vector operator*(const Vector &v, const Matrix &M);

/** Returns the product of the matrices */
Matrix operator*(const Matrix &M1, const Matrix &M2);

/** Returns the Identity matrix of size 3 by 3 (rest is 0) */
Matrix I();

/** Given a matrix M of size len by len, finds its inverse and returns whether
 * it is invertible or not*/
bool inverse(const Matrix &M, Matrix &res, int len = 3);
