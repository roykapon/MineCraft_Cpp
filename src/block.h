#pragma once

#include "object.h"
#define NUM_TRIANGLES_IN_BLOCK 12

class Block : public Object {
public:
  Block();

  Block(const Vector &pos);

  Block(const Block &other);

  ~Block();

  /* compares the blocks' positions */
  bool operator==(const Block &b2) const;
};

Vector get_block_pos(const Vector &pos);

class ObjectFace : Face {
public:
  Vector pos;
};
