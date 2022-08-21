#ifndef ENV_H
#define ENV_H

#include "block.h"
#include <map>
#include <vector>

#include <unordered_map>

#define WORLD_SIZE 100

using namespace std;

class Env {
public:
  unordered_map<Vector, Block, Vector_hash> blocks;
  vector<Face> visible_faces;

  Env() {
    blocks.clear();
    visible_faces.clear();
  };

  void create_block(const Vector &pos);

  void update_visible_faces();

  void move_object(Object &object, Vector &v);
};

#endif