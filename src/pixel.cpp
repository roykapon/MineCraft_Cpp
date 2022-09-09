
#include "pixel.h"

// =========================== ColoredVector ===========================

ColoredVector operator+(const ColoredVector &v1, const ColoredVector &v2) {
  // sets w to be the same as v1
  return ColoredVector(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w,
                       v1.texture_x + v2.texture_x,
                       v1.texture_y + v2.texture_y);
}

ColoredVector &ColoredVector::operator+=(const ColoredVector &v) {
  x += v.x;
  y += v.y;
  z += v.z;
  return *this;
}

ColoredVector operator-(const ColoredVector &v1, const ColoredVector &v2) {
  // sets w to be the same as v1
  return ColoredVector(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w,
                       v1.texture_x - v2.texture_x,
                       v1.texture_y - v2.texture_y);
}

ColoredVector &ColoredVector::operator-=(const ColoredVector &v) {
  x -= v.x;
  y -= v.y;
  z -= v.z;
  return *this;
}

ColoredVector operator*(const ColoredVector &v, SCALAR a) {
  // sets w to be the same as v1
  return ColoredVector(v.x * a, v.y * a, v.z * a, v.w, lround(v.texture_x * a),
                       lround(v.texture_y * a));
}

ColoredVector &ColoredVector::operator*=(SCALAR a) {
  x *= a;
  y *= a;
  z *= a;
  return *this;
}

ColoredVector operator*(SCALAR a, const ColoredVector &v) {
  // sets w to be the same as v1
  return ColoredVector(v.x * a, v.y * a, v.z * a, v.w, lround(v.texture_x * a),
                       lround(v.texture_y * a));
}

ColoredVector interpolate(const ColoredVector &v1, const ColoredVector &v2,
                          SCALAR ratio) {
  return ColoredVector(INTERPOLATE(v1.x, v2.x, ratio),
                       INTERPOLATE(v1.y, v2.y, ratio),
                       INTERPOLATE(v1.z, v2.z, ratio), v1.w,
                       lround(INTERPOLATE(v1.texture_x, v2.texture_x, ratio)),
                       lround(INTERPOLATE(v1.texture_y, v2.texture_y, ratio)));
}

void interpolate(const ColoredVector &v1, const ColoredVector &v2, SCALAR ratio,
                 ColoredVector &res) {
  res.x = INTERPOLATE(v1.x, v2.x, ratio);
  res.y = INTERPOLATE(v1.y, v2.y, ratio);
  res.z = INTERPOLATE(v1.z, v2.z, ratio);
  res.w = v1.w;
  res.texture_x = lround(INTERPOLATE(v1.texture_x, v2.texture_x, ratio));
  res.texture_y = lround(INTERPOLATE(v1.texture_y, v2.texture_y, ratio));
}

ColoredVector operator*(const Matrix &M, const ColoredVector &v) {
  ColoredVector res;
  for (int i = 0; i < EXTENDED_AXES; i++) {
    SCALAR sum = 0.0f;
    for (int j = 0; j < EXTENDED_AXES; j++) {
      sum += v[j] * M[i][j];
    }
    res[i] = sum;
  }
  res.texture_x = v.texture_x;
  res.texture_y = v.texture_y;
  return res;
}

ColoredVector operator*(const ColoredVector &v, const Matrix &M) {
  ColoredVector res;
  for (int i = 0; i < EXTENDED_AXES; i++) {
    SCALAR sum = 0.0f;
    for (int j = 0; j < EXTENDED_AXES; j++) {
      sum += M[j][i] * v[j];
    }
    res[i] = sum;
  }
  res.texture_x = v.texture_x;
  res.texture_y = v.texture_y;
  return res;
}

// =========================== Pixel ===========================

int &Pixel::operator[](int index) {
  switch (index) {
  case X_AXIS:
    return x;
  case Y_AXIS:
    return y;
  default:
    cout << "Invalid index: " << index;
    return x;
  }
}

Pixel operator+(const Pixel &p1, const Pixel &p2) {
  Pixel res;
  res.x = p1.x + p2.x;
  res.y = p1.y + p2.y;
  res.source = p1.source + p2.source;
  return res;
}

Pixel operator-(const Pixel &p1, const Pixel &p2) {
  Pixel res;
  res.x = p1.x - p2.x;
  res.y = p1.y - p2.y;
  res.source = p1.source - p2.source;
  return res;
}

Pixel operator*(const Pixel &p, SCALAR a) {
  Pixel res;
  res.x = lround(p.x * a);
  res.y = lround(p.y * a);
  res.source = p.source * a;
  return res;
}

ostream &operator<<(ostream &os, Pixel &p) {
  os << "x = " << p.x << ", y = " << p.y << ", source = " << p.source << '\n';
  return os;
}

void get_extremum(Pixel *pixels, int len, int extremum[2], int lower_bound,
                  int upper_bound) {
  extremum[0] = INT_MAX;
  extremum[1] = INT_MIN;
  Pixel *p;
  for (int i = 0; i < len; i++) {
    p = &pixels[i];
    extremum[0] = max(min(extremum[0], p->y), lower_bound);
    extremum[1] = min(max(extremum[1], p->y), upper_bound);
  }
}
