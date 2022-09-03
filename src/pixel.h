#pragma once

#include "linlib.h"

using namespace std;

class ColoredVector : public Vector {

public:
  int texture_x, texture_y;

  ColoredVector(SCALAR _x = 0, SCALAR _y = 0, SCALAR _z = 0, SCALAR _w = 1,
                int _texture_x = 0, int _texture_y = 0) {
    x = _x;
    y = _y;
    z = _z;
    w = _w;
    texture_x = _texture_x;
    texture_y = _texture_y;
  }

  /** Adds the given vector in-place */
  ColoredVector &operator+=(const ColoredVector &v);

  /** Substructs the given vector in-place */
  ColoredVector &operator-=(const ColoredVector &v);

  /** Multiplies by the given scalar in-place */
  ColoredVector &operator*=(SCALAR a);
};

/** Returns the sum of the vectors */
ColoredVector operator+(const ColoredVector &v1, const ColoredVector &v2);

/** Returns the difference between the vectors (first-second) */
ColoredVector operator-(const ColoredVector &v1, const ColoredVector &v2);

/** Returns the vector multiplied by a scalar */
ColoredVector operator*(const ColoredVector &v, SCALAR a);

/** Returns the vector multiplied by a scalar */
ColoredVector operator*(SCALAR a, const ColoredVector &v);

/** Returns the point between the vectors with the given ratio */
ColoredVector interpolate(const ColoredVector &v1, const ColoredVector &v2,
                          SCALAR ratio);

/** same as interpolate, but stores the result in res */
void interpolate(const ColoredVector &v1, const ColoredVector &v2, SCALAR ratio,
                 ColoredVector &res);

ColoredVector operator*(const ColoredVector &v, const Matrix &M);

ColoredVector operator*(const Matrix &M, const ColoredVector &v);

// ============================= Pixel ===================================

class Pixel {

public:
  int x;
  int y;
  ColoredVector source;

  Pixel(int _x = 0, int _y = 0, ColoredVector _source = ColoredVector()) {
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
Pixel operator*(const Pixel &p, SCALAR a);

/** Prints the pixel */
ostream &operator<<(ostream &os, Pixel &p);

/** Returns an array in which the first value is the minimum of all y values
 * and the second value is the maximum of all y values */
void get_extremum(Pixel *pixels, int len, int *extremum,
                  int lower_bound = INT_MIN, int upper_bound = INT_MAX);
