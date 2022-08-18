#include "object.h"

#define TICK_TIME 0.0001
#define G 9.8 * TICK_TIME

void Object::apply_physics(Env &env) {
  v = v - Vector(0, G, 0);
  Vector possible_block = round(v);
  //   pos.y -=
}