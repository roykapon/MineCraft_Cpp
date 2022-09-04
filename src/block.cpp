#include "block.h"

Block::Block(const Vector &_pos) : Object(_pos) {}

Block::Block() : Object(Vector(0, 0, 0)) {}

Block::Block(const Block &other) : Object((Object &)other) {}

Vector get_block_pos(const Vector &pos) {
  return Vector(round(pos.x / 2) * 2.0, round(pos.y / 2) * 2.0,
                round(pos.z / 2) * 2.0);
}

Block::~Block() {}

bool Block::operator==(const Block &b2) const { return (pos == b2.pos); }