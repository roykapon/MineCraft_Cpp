#include "object.h"

Face default_faces[NUM_TRIANGLES_IN_BOX] = {
    // forward
    Face(ColoredVector(-1, -1, 1, 1, 0, 0),
         ColoredVector(1, -1, 1, 1, TEXTURE_RES, 0),
         ColoredVector(1, 1, 1, 1, TEXTURE_RES, TEXTURE_RES)),
    Face(ColoredVector(-1, -1, 1, 1, 0, 0),
         ColoredVector(1, 1, 1, 1, TEXTURE_RES, TEXTURE_RES),
         ColoredVector(-1, 1, 1, 1, 0, TEXTURE_RES)),
    // right
    Face(ColoredVector(1, -1, 1, 1, 0, TEXTURE_RES),
         ColoredVector(1, -1, -1, 1, 0, 0),
         ColoredVector(1, 1, -1, 1, TEXTURE_RES, 0)),
    Face(ColoredVector(1, -1, 1, 1, 0, TEXTURE_RES),
         ColoredVector(1, 1, -1, 1, TEXTURE_RES, 0),
         ColoredVector(1, 1, 1, 1, TEXTURE_RES, TEXTURE_RES)),
    // backward
    Face(ColoredVector(1, -1, -1, 1, TEXTURE_RES, 0),
         ColoredVector(-1, -1, -1, 1, 0, 0),
         ColoredVector(-1, 1, -1, 1, 0, TEXTURE_RES)),
    Face(ColoredVector(1, -1, -1, 1, TEXTURE_RES, 0),
         ColoredVector(-1, 1, -1, 1, 0, TEXTURE_RES),
         ColoredVector(1, 1, -1, 1, TEXTURE_RES, TEXTURE_RES)),
    // left
    Face(ColoredVector(-1, -1, -1, 1, 0, 0),
         ColoredVector(-1, -1, 1, 1, 0, TEXTURE_RES),
         ColoredVector(-1, 1, 1, 1, TEXTURE_RES, TEXTURE_RES)),
    Face(ColoredVector(-1, -1, -1, 1, 0, 0),
         ColoredVector(-1, 1, 1, 1, TEXTURE_RES, TEXTURE_RES),
         ColoredVector(-1, 1, -1, 1, TEXTURE_RES, 0)),
    // up
    Face(ColoredVector(-1, 1, 1, 1, 0, TEXTURE_RES),
         ColoredVector(1, 1, 1, 1, TEXTURE_RES, TEXTURE_RES),
         ColoredVector(1, 1, -1, 1, TEXTURE_RES, 0)),
    Face(ColoredVector(-1, 1, 1, 1, 0, TEXTURE_RES),
         ColoredVector(1, 1, -1, 1, TEXTURE_RES, 0),
         ColoredVector(-1, 1, -1, 1, 0, 0)),
    // down
    Face(ColoredVector(1, -1, 1, 1, TEXTURE_RES, TEXTURE_RES),
         ColoredVector(-1, -1, -1, 1, 0, 0),
         ColoredVector(1, -1, -1, 1, TEXTURE_RES, 0)),
    Face(ColoredVector(1, -1, 1, 1, TEXTURE_RES, TEXTURE_RES),
         ColoredVector(-1, -1, 1, 1, 0, TEXTURE_RES),
         ColoredVector(-1, -1, -1, 1, 0, 0))};

void Object::update() {
  update_direction();
  update_world();
  update_faces();
}

Vector get_direction(SCALAR horizontal, SCALAR vertical) {
  return Vector(cos(vertical) * (-sin(horizontal)), sin(vertical),
                cos(vertical) * cos(horizontal), 0);
}

void Object::update_direction() {
  direction = get_direction(horizontal, horizontal);
}

void Object::update_world() {
  Matrix rotate_x = rotate(X_AXIS, vertical);
  Matrix rotate_y = rotate(Y_AXIS, horizontal);
  Matrix shift_pos = shift(pos);
  world = rotate_x * rotate_y;
  world = world * shift_pos;
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

SCALAR block_block_collision(const Object &o1, const Object &o2,
                             const Vector &v, Vector &normal) {
  SCALAR min_t = 1;
  SCALAR t;
  Vector curr_normal;
  // for now it is not needed
  Face argmin;
  for (int i = 0; i < o1.faces_len; i++) {
    Face &face1 = o1.faces[i];
    for (int j = 0; j < o2.faces_len; j++) {
      Face &face2 = o2.faces[j];
      t = face_face_collision(face1, face2, v, curr_normal);
      if (t < min_t) {
        normal = curr_normal;
        argmin = face2;
        min_t = t;
      }
    }
  }
  return min_t;
}

Face &Object::operator[](int index) { return faces[index]; }

const Face &Object::operator[](int index) const { return faces[index]; }
