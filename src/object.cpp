#include "object.h"

Face default_faces[NUM_TRIANGLES_IN_BOX] = {
    // forward
    Face(Vector(-1, -1, 1, 1, 0, 0), Vector(1, -1, 1, 1, TEXTURE_RES, 0),
         Vector(1, 1, 1, 1, TEXTURE_RES, TEXTURE_RES)),
    Face(Vector(-1, -1, 1, 1, 0, 0),
         Vector(1, 1, 1, 1, TEXTURE_RES, TEXTURE_RES),
         Vector(-1, 1, 1, 1, 0, TEXTURE_RES)),
    // right
    Face(Vector(1, -1, 1, 1, 0, TEXTURE_RES), Vector(1, -1, -1, 1, 0, 0),
         Vector(1, 1, -1, 1, TEXTURE_RES, 0)),
    Face(Vector(1, -1, 1, 1, 0, TEXTURE_RES),
         Vector(1, 1, -1, 1, TEXTURE_RES, 0),
         Vector(1, 1, 1, 1, TEXTURE_RES, TEXTURE_RES)),
    // beckward
    Face(Vector(1, -1, -1, 1, TEXTURE_RES, 0), Vector(-1, -1, -1, 1, 0, 0),
         Vector(-1, 1, -1, 1, 0, TEXTURE_RES)),
    Face(Vector(1, -1, -1, 1, TEXTURE_RES, 0),
         Vector(-1, 1, -1, 1, 0, TEXTURE_RES),
         Vector(1, 1, -1, 1, TEXTURE_RES, TEXTURE_RES)),
    // left
    Face(Vector(-1, -1, -1, 1, 0, 0), Vector(-1, -1, 1, 1, 0, TEXTURE_RES),
         Vector(-1, 1, 1, 1, TEXTURE_RES, TEXTURE_RES)),
    Face(Vector(-1, -1, -1, 1, 0, 0),
         Vector(-1, 1, 1, 1, TEXTURE_RES, TEXTURE_RES),
         Vector(-1, 1, -1, 1, TEXTURE_RES, 0)),
    // up
    Face(Vector(-1, 1, 1, 1, 0, TEXTURE_RES),
         Vector(1, 1, 1, 1, TEXTURE_RES, TEXTURE_RES),
         Vector(1, 1, -1, 1, TEXTURE_RES, 0)),
    Face(Vector(-1, 1, 1, 1, 0, TEXTURE_RES),
         Vector(1, 1, -1, 1, TEXTURE_RES, 0), Vector(-1, 1, -1, 1, 0, 0)),
    Face(Vector(1, -1, 1, 1, TEXTURE_RES, TEXTURE_RES),
         Vector(-1, -1, -1, 1, 0, 0), Vector(1, -1, -1, 1, TEXTURE_RES, 0)),
    // down
    Face(Vector(1, -1, 1, 1, TEXTURE_RES, TEXTURE_RES),
         Vector(-1, -1, 1, 1, 0, TEXTURE_RES), Vector(-1, -1, -1, 1, 0, 0))};

Object::Object(const Vector &_pos, double _vertical, double _horizontal) {
  pos = _pos;
  vertical = _vertical;
  horizontal = _horizontal;
  v = Vector(0, 0, 0, 0);
  faces_len = 12;
  faces = new Face[faces_len];
  update();
}

Object::Object(const Object &other) {
  pos = other.pos;
  vertical = other.vertical;
  horizontal = other.horizontal;
  v = other.v;
  faces_len = other.faces_len;
  faces = new Face[faces_len];
  update();
}

void Object::update() {
  update_world();
  update_faces();
}

void Object::update_world() {
  Matrix rotate_x = Matrix::rotate(X_AXIS, vertical);
  Matrix rotate_y = Matrix::rotate(Y_AXIS, horizontal);
  Matrix shift = Matrix::shift(pos);
  world = rotate_x * rotate_y;
  world = world * shift;
}

void Object::update_faces() {
  for (int i = 0; i < faces_len; i++) {
    faces[i] = default_faces[i] * world;
  }
}

Object::~Object() { delete[] faces; }

Object &Object::operator=(const Object &other) {
  pos = other.pos;
  vertical = other.vertical;
  horizontal = other.horizontal;
  v = other.v;
  faces_len = other.faces_len;
  faces = new Face[faces_len];
  for (int i = 0; i < faces_len; i++) {
    faces[i] = other.faces[i];
  }
  return *this;
}

double intersection(const Object &o1, const Object &o2, Vector &v) {
  double min_t = 1;
  double t;
  Vector inter;
  for (int i = 0; i < o1.faces_len; i++) {
    Face &face1 = o1.faces[i];
    for (int j = 0; j < o1.faces_len; j++) {
      Face &face2 = o2.faces[j];
      t = intersection(face1, face2, v);
      min_t = min(min_t, t);
    }
  }
  return min_t;
}

Face &Object::operator[](int index) { return faces[index]; }
