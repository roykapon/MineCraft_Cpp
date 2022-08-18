#ifndef OBJECT_H
#define OBJECT_H

#include "camera.h"
#include "env.h"

class Object : Camera {
  Vector pos;
  Vector v;

  Object(Vector _pos) { pos = _pos; }

  void apply_physics(Env &env);
};

#endif