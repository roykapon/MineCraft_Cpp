#pragma once

#include "Face.h"

#define TICK_TIME 0.0001
#define G 9.8 * TICK_TIME
#define NUM_TRIANGLES_IN_BOX 12
#define FEFAULT_FRICTION 0.8
class Object {
public:
  Vector pos;
  SCALAR vertical, horizontal;
  Vector v;
  Vector direction;
  // multiply by world on the right
  Matrix world;
  Face *faces;
  int faces_len;
  // the closer friction to 1 is, the smaller the friction
  SCALAR friction;

  Object(const Vector &_pos = Vector(0, 0, 0), SCALAR _vertical = 0.0,
         SCALAR _horizontal = 0.0, SCALAR _friction = FEFAULT_FRICTION,
         const Vector &_v = Vector(0, 0, 0, 0))
      : pos(_pos), vertical(_vertical), horizontal(_horizontal),
        friction(_friction), v(_v), faces_len(12) {
    faces = new Face[faces_len];
    update();
  }

  Object(const Object &other)
      : Object(other.pos, other.vertical, other.horizontal, other.friction,
               other.v) {}

  ~Object();

  Object &operator=(const Object &other);

  void update();

  void update_direction();

  void update_world();

  void update_faces();

  /* Returns a mutable reference to face at the given index */
  Face &operator[](int index);

  /* Returns a const reference to face at the given index */
  const Face &operator[](int index) const;
};

SCALAR block_block_collision(const Object &o1, const Object &o2,
                             const Vector &v, Vector &normal);

Vector get_direction(SCALAR horizontal, SCALAR vertical);
