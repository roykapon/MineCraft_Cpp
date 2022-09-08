#pragma once

#include "block.h"
#include <map>
#include <vector>

#include <unordered_map>
#include <unordered_set>


#define WORLD_SIZE 100
#define GRAVITY Vector(0, -3.0f, 0)
#define AIR_FRICTION 0.95
#define FACE_OBJECT pair<Face *, Object *>
#define REACH_DIST 10
#define SPEED 20.0f
#define JUMP_HEIGHT 2.0f
#define JUMP Vector(0, JUMP_HEIGHT, 0, 0);

using namespace std;

enum PlayerInteraction { ADD_BLOCK, REMOVE_BLOCK };

class Env {
public:
  unordered_map<Vector, Block *, Vector_hash> blocks;
  unordered_map<Face *, Object *> visible_faces;
  unordered_set<Object *> entities;

  Env();

  ~Env();

  void free_blocks();

  void free_entities();

  /* Creates a block of 2 by 2 by 2 centerd in the given position */
  void create_block(const Vector &pos);

  /** Creates an entity in the environment at the given position
   * (returns the created entity) */
  void remove_block(const Vector &pos);

  /* Creates an entity of 2 by 2 by 2 centerd in the given position */
  Object &create_entity(const Vector &pos);

  /* Removes the given entity from the environment
   (also deletes the entity's data) */
  void remove_entity(Object *&entity);

  /* updates the visible_faces map.
   * should be called on any update to the blocks */
  void update_visible_faces();

  void player_interact(Object &player, Vector &direction,
                       PlayerInteraction interaction);

  /* moves the object with the given velocity and updates it accordingly */
  void move_object(Object &object, Vector &v);

  /* returns the ratio of the velocity vector in which the object can move
   * before colliding with something */
  SCALAR object_env_collision(const Object &object, const Vector &v,
                              Vector &normal, Object *&collision);

  SCALAR point_env_collision(const Vector &pos, const Vector &v, Vector &normal,
                             Object *&collision);

  /* moves all entities following some physics rules */
  void apply_physics(SCALAR frame_time);
};