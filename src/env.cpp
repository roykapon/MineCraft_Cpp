#include "env.h"

using namespace std;

/** creates a block in the environment at the given position */
void Env::create_block(const Vector &pos) {
  // pass
  blocks[pos] = Block(pos);
}

void Env::create_entity(const Vector &pos) {
  // pass
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

void Env::move_object(Object &object, Vector &v) {
  double min_t = 1;
  Vector offset;
  Vector block_pos;
  double t;
  Vector rounded = round(object.pos);
  for (double x = -1; x <= 1; x++) {
    for (double y = -1; y <= 1; y++) {
      for (double z = -1; z <= 1; z++) {
        if (x != 0 || y != 0 || z != 0) {
          offset = Vector(x, y, z);
          block_pos = rounded + 2 * offset;
          if (blocks.find(block_pos) != blocks.end()) {
            t = intersection(object, (Object &)blocks[block_pos], v);
            min_t = min(min_t, t);
          }
        }
      }
    }
  }

  object.pos = object.pos + min_t * v;
}
