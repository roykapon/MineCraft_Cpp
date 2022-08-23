#include "block.h"

Block::Block(const Vector &_pos) : Object(_pos) {}

Block::Block() : Object(Vector()) {}

Block::Block(const Block &other) : Object((Object &)other) {}

Block::~Block() {}

bool Block::operator==(const Block &b2) const { return (pos == b2.pos); }