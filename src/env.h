#pragma once

#include "block.h"
#include <map>
#include <vector>

#include <unordered_map>

#define WORLD_SIZE 100
#define GRAVITY Vector(0, -3.0f, 0)
#define AIR_FRICTION 0.95

using namespace std;

class Env {
public:
  unordered_map<Vector, Block, Vector_hash> blocks;
  vector<Face *> visible_faces;
  vector<Object> entities;

  Env() {
    blocks.clear();
    visible_faces.clear();
  };

  void create_block(const Vector &pos);

  void create_entity(const Vector &pos);

  void update_visible_faces();

  void move_object(Object &object, Vector &v);

  double object_env_collision(Object &object, Vector &v, Vector &normal,
                              Object *&collision);

  void apply_physics(double frame_time);
};