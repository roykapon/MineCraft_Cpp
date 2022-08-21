#include "block.h"

Block::Block(const Vector &_pos) : Object(_pos) {}

Block::Block() : Object(Vector()) {}

Block::Block(const Block &other) : Object((Object &)other) {}

Block::~Block() {}

Face &Block::operator[](int index) { return faces[index]; }

bool Block::operator==(const Block &b2) const { return (pos == b2.pos); }