#include "face.h"

using namespace std;

void Face::update_normal() {
  Vector edge1 = vertices[1] - vertices[0];
  Vector edge2 = vertices[2] - vertices[1];
  normal = cross(edge1, edge2);
}

const Vector &Face::operator[](int index) const {
  const Vector &res = vertices[index % 3];
  return res;
}

Vector &Face::operator[](int index) { return vertices[index % 3]; }

Face operator+(const Face &face, const Vector &pos) {
  Face res;
  for (int i = 0; i < 3; i++) {
    res[i] = face[i] + pos;
    res[i].texture_x = face[i].texture_x;
    res[i].texture_y = face[i].texture_y;
  }
  res.normal = face.normal;
  return res;
}
