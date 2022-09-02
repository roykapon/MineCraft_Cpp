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
  // multiply by world on the right
  Matrix world;
  Face *faces;
  int faces_len;
  // the closer friction to 1 is, the smaller the friction
  SCALAR friction;

  Object(const Vector &_pos, SCALAR vertical = 0.0, SCALAR horizontal = 0.0,
         SCALAR friction = FEFAULT_FRICTION);

  Object(const Object &other);

  ~Object();

  Object &operator=(const Object &other);

  void update();

  void update_world();

  void update_faces();

  Face &operator[](int index);
};

SCALAR block_block_collision(const Object &o1, const Object &o2,
                             const Vector &v, Vector &normal);
