#ifndef LINLIB_H
#define LINLIB_H
#include <SDL2/SDL.h>
#include <iostream>
using namespace std;

#define X 0
#define Y 1
#define Z 2
#define W 3
#define AXES 3
#define EXTENDED_AXES 4
#define MAX_NUM_LENGTH 13
#define PI 3.14159265

class Vector {

public:
  float x, y, z, w;

  Vector(float _x = 0, float _y = 0, float _z = 0, float _w = 1) {
    x = _x;
    y = _y;
    z = _z;
    w = _w;
  }

  /** Returns the value at the given index (or axis)*/
  float &operator[](int index);
};
/** Prints the vector */
ostream &operator<<(ostream &os, Vector &v);

/** Returns the sum of the vectors */
Vector operator+(Vector &v1, Vector &v2);

/** Returns the difference between the vectors (first-second) */
Vector operator-(Vector &v1, Vector &v2);

/** Returns the vector multiplied by a scalar */
Vector operator*(Vector &v, float a);

/** Returns the dot product of the vectors */
float operator*(Vector &v1, Vector &v2);

class Matrix {

public:
  Vector vectors[4];

  Matrix(Vector v1 = Vector(), Vector v2 = Vector(), Vector v3 = Vector(),
         Vector v4 = Vector()) {
    vectors[0] = v1;
    vectors[1] = v2;
    vectors[2] = v3;
    vectors[3] = v4;
  }

  /** Returns the row at the given index */
  Vector &operator[](int index);

  /** Returns the rotation matrix around the given axis
   *  (when applied on the RIGHT)
   *  The rotation direction is clockwise when looking towards the given axis */
  static Matrix rotate(int axis, float angle);

  /** Returns the shifting matrix, assuming w=1
   *  (when applied on the RIGHT) */
  static Matrix shift(Vector &offset);
};

/** Prints the matrix */
ostream &operator<<(ostream &os, Matrix &M);

/** Returns the sum of the two matrices */
Matrix operator+(Matrix &M1, Matrix &M2);

/** Returns the difference of the matrices */
Matrix operator-(Matrix &M1, Matrix &M2);

/** Returns the vector multiplied by a matrix (from the LEFT) */
Vector operator*(Matrix &M, Vector &v);

/** Returns the vector multiplied by a matrix (from the RIGHT)*/
Vector operator*(Vector &v, Matrix &M);

/** Returns the product of the matrices */
Matrix operator*(Matrix &M1, Matrix &M2);

class Pixel : public SDL_Point {

public:
  Vector source;

  Pixel(float _x = 0, float _y = 0, Vector _source = Vector()) {
    x = _x;
    y = _y;
    source = _source;
  }

  /** Returns the value a the given axis*/
  int &operator[](int index);
};

/** Returns the sum of the pixels (sources also summed) */
Pixel operator+(Pixel &p1, Pixel &p2);

/** Returns the difference of the pixels (sources also substracted) */
Pixel operator-(Pixel &p1, Pixel &p2);

/** Returns the pixel multyplied by a scalar (source also multyplied) */
Pixel operator*(Pixel p, float a);

/** Prints the pixel */
ostream &operator<<(ostream &os, Pixel &p);

#endif