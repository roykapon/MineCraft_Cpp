#include "camera.h"
#include <cmath>
#include <iomanip>
#include <iostream>

using namespace std;

// =========================== Camera ===========================

Matrix Camera::get_world() {
  Vector offset = pos * (-1);
  Matrix shift = Matrix::shift(offset);
  Matrix rotate_x = Matrix::rotate(X, -vertical);
  Matrix rotate_y = Matrix::rotate(Y, -horizontal);
  world = shift * rotate_x;
  world = world * rotate_y;
  return world;
}

void Camera::update() {
  world = get_world();
  direction = get_direction();
}

Pixel Camera::project(Vector &v) {
  Vector rotated = v * world;
  float ratio = ffd / rotated.z;
  float x = pixel_x(rotated.x * ratio);
  float y = pixel_y(rotated.y * ratio);
  Pixel res = Pixel(x, y, v);
  //   cout << "projected: " << res;
  //   cout << "horizontal: " << horizontal << " vertical: " << vertical <<
  //   endl;
  return res;
}

Vector Camera::get_direction() {
  float x = cos(vertical) * (-sin(horizontal));
  float y = sin(vertical);
  float z = cos(vertical) * cos(horizontal);
  return Vector(x, y, z);
}

Pixel *Camera::project(Face &face) {
  Pixel *projected = (Pixel *)malloc(sizeof(Pixel) * 3);
  for (int i = 0; i < 3; i++) {
    projected[i] = project(face[i]);
  }
  return projected;
}

int Camera::pixel_x(float x) { return round(width * x) + width / 2; }

int Camera::pixel_y(float y) { return round(height * y) + height / 2; }