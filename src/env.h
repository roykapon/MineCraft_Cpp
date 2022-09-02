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

  /* creates a block of 2 by 2 by 2 centerd in the given position */
  void create_block(const Vector &pos);

  /* creates an entity of 2 by 2 by 2 centerd in the given position */
  void create_entity(const Vector &pos);

  /* updates the visible_faces vector.
   * should be called on any update to the blocks */
  void update_visible_faces();

  /* moves the object with the given velocity and updates it accordingly */
  void move_object(Object &object, Vector &v);

  /* returns the ratio of the velocity vector in which the object can move
   * before colliding with something */
  SCALAR object_env_collision(Object &object, Vector &v, Vector &normal,
                              Object *&collision);

  /* moves all entities following some physics rules */
  void apply_physics(SCALAR frame_time);
};