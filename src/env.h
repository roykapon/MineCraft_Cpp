#pragma once

#include "block.h"
#include <map>
#include <vector>

#include <unordered_map>

#define WORLD_SIZE 100
#define GRAVITY Vector(0, -3.0f, 0)
#define AIR_FRICTION 0.95
#define FACE_OBJECT pair<Face *, Object *>
#define REACH_DIST 10
using namespace std;

class Node {
public:
  Object *data;
  Node *next;
  Node *prev;

  Node();

  Node(Object *_data, Node *_next, Node *_prev);

  ~Node();
};

/* Add a new element to head of the list and returns the created node
 * (creating the node is the responsibility of the caller) */
Node *push(Node *&head, Object *data);

/* Removes the given node from the list */
void pop(Node *node);

class Env {
public:
  Node *memory;
  unordered_map<Vector, Node *, Vector_hash> blocks;
  vector<pair<Face *, Object *>> visible_faces;
  vector<Object> entities;

  Env();

  ~Env();

  void init_memory();

  void delete_memory();

  /* creates a block of 2 by 2 by 2 centerd in the given position */
  void create_block(const Vector &pos);

  /* creates an entity of 2 by 2 by 2 centerd in the given position */
  void create_entity(const Vector &pos);

  /* updates the visible_faces vector.
   * should be called on any update to the blocks */
  void update_visible_faces();

  void player_interact(Object &player, Vector &direction);

  /* moves the object with the given velocity and updates it accordingly */
  void move_object(Object &object, Vector &v);

  /* returns the ratio of the velocity vector in which the object can move
   * before colliding with something */
  SCALAR object_env_collision(Object &object, Vector &v, Vector &normal,
                              Object *&collision);

  SCALAR point_env_collision(const Vector &pos, const Vector &v, Vector &normal,
                             Object *&collision);

  /* moves all entities following some physics rules */
  void apply_physics(SCALAR frame_time);
};