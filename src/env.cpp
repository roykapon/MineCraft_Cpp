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

SCALAR Env::object_env_collision(Object &object, Vector &v, Vector &normal,
                                 Object *&collision) {
  SCALAR n = norm(v);
  if (n < EPSILON) {
    return 1;
  }
  SCALAR min_t = 1;
  SCALAR t;
  Vector offset;
  Vector block_pos;
  Vector curr_normal;
  Vector rounded = get_block_pos(object.pos);
  SCALAR radius = ceil(norm(v));
  collision = nullptr;
  // should be optimized so that only the relevant block positions are
  // considered
  for (SCALAR x = -radius; x <= radius; x++) {
    for (SCALAR y = -radius; y <= radius; y++) {
      for (SCALAR z = -radius; z <= radius; z++) {
        // could be deleted
        if (x != 0 || y != 0 || z != 0) {
          offset = Vector(2 * x, 2 * y, 2 * z);
          block_pos = rounded + offset;
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

// SCALAR min_t = 1;
// vector<Vector> collisions;
// Vector direction, curr_offset, block_pos, curr_pos, curr_normal;

// SCALAR t;
// for (int axis = 0; axis < AXES; axis++) {
//   if (v[axis] == 0) {
//     continue;
//   }
//   direction = v * (1 / abs(v[axis]));
//   // the reason this thing works is not trivial: since we intersect any of
//   // the blocks in the trajectory only once and from a *single face* we
//   // get that we add every block in the trajectory exactly once
//   for (curr_offset = direction; abs(curr_offset[axis]) < abs(v[axis]) + 1;
//        curr_offset = curr_offset + direction) {
//     curr_pos = object.pos + curr_offset;
//     block_pos = curr_pos;
//     block_pos[axis] += direction[axis];
//     block_pos = get_block_pos(block_pos);
//     collisions.push_back(block_pos);
//   }

//   for (Vector block_pos : collisions) {
//     if (blocks.find(block_pos) != blocks.end()) {
//       Object &block = blocks[block_pos];
//       t = block_block_collision(object, block, v, curr_normal);
//       if (t < min_t) {
//         min_t = t;
//         normal = curr_normal;
//         collision = &block;
//       }
//     }
//   }
// }

// may replace v with object.v
void Env::move_object(Object &object, Vector &v) {
  Vector normal;
  Object *collision = nullptr;
  SCALAR t = object_env_collision(object, v, normal, collision);
  while (t < 1) {
    object.pos = object.pos + v * t;
    SCALAR force = (normal * v) * (t - 1);
    v = ((1 - t) * v + force * normal) * (collision->friction / (force + 1));
    t = object_env_collision(object, v, normal, collision);
  }
  object.pos = object.pos + v;
  object.v = object.v * AIR_FRICTION;
  object.update();
}

void Env::apply_physics(SCALAR frame_time) {
  Vector gravity = GRAVITY * frame_time;
  for (Object &entity : entities) {
    entity.v = entity.v + gravity;
    move_object(entity, entity.v);
  }
}