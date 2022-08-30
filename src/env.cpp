#include "env.h"

using namespace std;

/** creates a block in the environment at the given position */
void Env::create_block(const Vector &pos) {
  //
  blocks[pos] = Block(pos);
}

void Env::create_entity(const Vector &pos) {
  //
  entities.push_back(Object(pos));
}

/** Updates visible faces list */
void Env::update_visible_faces() {
  visible_faces.clear();
  for (auto &pos_block : blocks) {
    const Vector &pos = pos_block.first;
    Block &block = pos_block.second;
    for (int i = 0; i < block.faces_len; i++) {
      if (blocks.count(pos + block[i].normal * 2) == 0)
        visible_faces.push_back(&block[i]);
    }
  }

  for (Object &entity : entities) {
    for (int i = 0; i < entity.faces_len; i++) {
      visible_faces.push_back(&entity[i]);
    }
  }
}

double Env::object_env_collision(Object &object, Vector &v, Vector &normal,
                                 Object *&collision) {
  if (norm(v) < 0.01) {
    return 1;
  }
  double min_t = 1;
  Vector offset;
  Vector block_pos;
  double t;
  Vector curr_normal;
  Vector rounded = round(object.pos);
  double radius = ceil(norm(v));
  // should be optimized so that only the relevant block positions are
  // considered
  for (double x = -radius; x <= radius; x++) {
    for (double y = -radius; y <= radius; y++) {
      for (double z = -radius; z <= radius; z++) {
        // could be deleted
        if (x != 0 || y != 0 || z != 0) {
          offset = Vector(x, y, z);
          block_pos = rounded + 2 * offset;
          if (blocks.find(block_pos) != blocks.end()) {
            Object &block = blocks[block_pos];
            t = block_block_collision(object, block, v, curr_normal);
            if (t < min_t) {
              min_t = t;
              normal = curr_normal;
              collision = &block;
            }
          }
        }
      }
    }
  }
  return min_t;
}
// may replace v with object.v
void Env::move_object(Object &object, Vector &v) {
  Vector normal;
  Object *collision;
  double t = object_env_collision(object, v, normal, collision);
  int counter = 0;
  while (t < 1 - EPSILON) {
    if (counter == 2) {
      counter++;
    }
    object.pos = object.pos + v * t;
    double force = (normal * v) * (t - 1);
    v = ((1 - t) * v + force * normal) * (collision->friction / (force + 1));
    t = object_env_collision(object, v, normal, collision);
    counter++;
  }
  object.pos = object.pos + v;
  object.v = object.v * AIR_FRICTION;
  object.update();
}

void Env::apply_physics(double frame_time) {
  Vector gravity = GRAVITY * frame_time;
  for (Object &entity : entities) {
    entity.v = entity.v + gravity;
    move_object(entity, entity.v);
  }
}