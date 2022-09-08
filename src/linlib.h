#pragma once

#include <cstring>
#include <iomanip>
#include <iostream>
#include <limits>
#include <math.h>
#include <string>

typedef float SCALAR;
// #define INT_MAX numeric_limits<int>::max()
// #define INT_MIN numeric_limits<int>::min()
#define SCALAR_MAX numeric_limits<SCALAR>::max()
#define SCALAR_MIN numeric_limits<SCALAR>::min()

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
#define PI (SCALAR)3.14159265
#define EPSILON (SCALAR)0.00001

#define INTERPOLATE(X1, X2, RATIO)                                             \
  ((((SCALAR)X1) * RATIO) + (((SCALAR)X2) * (1.0f - RATIO)))

using namespace std;

/* vector of 4 elements (floating points)*/
class Vector {

public:
  SCALAR x, y, z, w;

  Vector() {}

  Vector(SCALAR _x, SCALAR _y, SCALAR _z, SCALAR _w = 1)
      : x(_x), y(_y), z(_z), w(_w) {}

  Vector(const Vector &v) : x(v.x), y(v.y), z(v.z), w(v.w) {}

  /** Returns the value at the given index (or axis)*/
  SCALAR operator[](int index) const;

  /** Returns a mutable reference to the value at the given index (or axis) */
  SCALAR &operator[](int index);

  /** Returns true if the vectors have the same 3d position (does not compare
   * the w coordinate) */
  bool operator==(const Vector &b2) const;

  /** Returns true if the vectors so not have the same 3d position (does not
   * compare the w coordinate) */
  bool operator!=(const Vector &b2) const;

  /** Adds the given vector in-place */
  Vector &operator+=(const Vector &v);

  /** Substructs the given vector in-place */
  Vector &operator-=(const Vector &v);

  /** Multiplies by the given scalar in-place */
  Vector &operator*=(SCALAR a);
};
/** Prints the vector */
ostream &operator<<(ostream &os, const Vector &v);

/** Returns the sum of the vectors */
Vector operator+(const Vector &v1, const Vector &v2);

/** Returns the difference between the vectors (first-second) */
Vector operator-(const Vector &v1, const Vector &v2);

/** Returns the vector multiplied by a scalar */
Vector operator*(const Vector &v, SCALAR a);

/** Returns the vector multiplied by a scalar */
Vector operator*(SCALAR a, const Vector &v);

/** Returns the point between the vectors with the given ratio */
Vector interpolate(const Vector &v1, const Vector &v2, SCALAR ratio);

/** same as interpolate, but stores the result in res */
void interpolate(const Vector &v1, const Vector &v2, SCALAR ratio, Vector &res);

/** Returns the dot product of the vectors */
SCALAR operator*(const Vector &v1, const Vector &v2);

/** Returns the l2norm of the vector (its size)*/
SCALAR norm(const Vector &v);

/** Returns the normalized vector (same direction, with norm 1) */
Vector normalized(const Vector &v);

/** Returns the normalized vector in the x, z axes */
Vector normalized_horizontal(const Vector &v);

/** Returns the distance between the vectors */
SCALAR distance(const Vector &v1, const Vector &v2);

/** Returns the *normalized* cross-product between the vectors */
Vector cross(const Vector &v1, const Vector &v2);

/** Vector hash function */
struct Vector_hash {
  size_t operator()(const Vector &key) const;
};

/** returns the vector rounded to the closest block */
// Vector round(Vector &v);

// ============================ Matrix ============================

class Matrix {

public:
  Vector vectors[4];

  Matrix(const Vector &v1 = Vector(0, 0, 0, 0),
         const Vector &v2 = Vector(0, 0, 0, 0),
         const Vector &v3 = Vector(0, 0, 0, 0),
         const Vector &v4 = Vector(0, 0, 0, 0))
      : vectors{v1, v2, v3, v4} {}

  /** Returns a mutable reference to the row at the given index */
  const Vector &operator[](int index) const;

  /** Returns the row at the given index */
  Vector &operator[](int index);
};
/** Returns the rotation matrix around the given axis
 *  (when applied on the RIGHT)
 *  The rotation direction is clockwise when looking towards the given axis
 */
Matrix rotate(int axis, SCALAR angle);

/** Returns the shifting matrix, assuming w=1
 *  (when applied on the RIGHT) */
Matrix shift(const Vector &offset);

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
