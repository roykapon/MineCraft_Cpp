#ifndef OBJECT_H
#define OBJECT_H

#include "Face.h"

#define TICK_TIME 0.0001
#define G 9.8 * TICK_TIME
#define NUM_TRIANGLES_IN_BOX 12

class Object {
public:
  Vector pos;
  double vertical, horizontal;
  Vector v;
  // multiply by world on the right
  Matrix world;
  Face *faces;
  int faces_len;

  Object(const Vector &_pos, double vertical = 0.0, double horizontal = 0.0);

  Object(const Object &other);

  ~Object();

  Object &operator=(const Object &other);

  void update();

  void update_world();

  void update_faces();
};

double intersection(const Object &o1, const Object &o2, Vector &v);

#endif