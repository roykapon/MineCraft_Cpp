#include "block.h"

Face default_faces[NUM_TRIANGLES_IN_BLOCK] = {
    // forward
    Face(Vector(-1, -1, 1, 1, 0, 0), Vector(1, -1, 1, 1, TEXTURE_RES, 0),
         Vector(1, 1, 1, 1, TEXTURE_RES, TEXTURE_RES)),
    Face(Vector(-1, -1, 1, 1, 0, 0),
         Vector(1, 1, 1, 1, TEXTURE_RES, TEXTURE_RES),
         Vector(-1, 1, 1, 1, 0, TEXTURE_RES)),
    // right
    Face(Vector(1, -1, 1, 1, 0, TEXTURE_RES), Vector(1, -1, -1, 1, 0, 0),
         Vector(1, 1, -1, 1, TEXTURE_RES, 0)),
    Face(Vector(1, -1, 1, 1, 0, TEXTURE_RES),
         Vector(1, 1, -1, 1, TEXTURE_RES, 0),
         Vector(1, 1, 1, 1, TEXTURE_RES, TEXTURE_RES)),
    // beckward
    Face(Vector(1, -1, -1, 1, TEXTURE_RES, 0), Vector(-1, -1, -1, 1, 0, 0),
         Vector(-1, 1, -1, 1, 0, TEXTURE_RES)),
    Face(Vector(1, -1, -1, 1, TEXTURE_RES, 0),
         Vector(-1, 1, -1, 1, 0, TEXTURE_RES),
         Vector(1, 1, -1, 1, TEXTURE_RES, TEXTURE_RES)),
    // left
    Face(Vector(-1, -1, -1, 1, 0, 0), Vector(-1, -1, 1, 1, 0, TEXTURE_RES),
         Vector(-1, 1, 1, 1, TEXTURE_RES, TEXTURE_RES)),
    Face(Vector(-1, -1, -1, 1, 0, 0),
         Vector(-1, 1, 1, 1, TEXTURE_RES, TEXTURE_RES),
         Vector(-1, 1, -1, 1, TEXTURE_RES, 0)),
    // up
    Face(Vector(-1, 1, 1, 1, 0, TEXTURE_RES),
         Vector(1, 1, 1, 1, TEXTURE_RES, TEXTURE_RES),
         Vector(1, 1, -1, 1, TEXTURE_RES, 0)),
    Face(Vector(-1, 1, 1, 1, 0, TEXTURE_RES),
         Vector(1, 1, -1, 1, TEXTURE_RES, 0), Vector(-1, 1, -1, 1, 0, 0)),
    Face(Vector(1, -1, 1, 1, TEXTURE_RES, TEXTURE_RES),
         // down
         Vector(-1, -1, -1, 1, 0, 0), Vector(1, -1, -1, 1, TEXTURE_RES, 0)),
    Face(Vector(1, -1, 1, 1, TEXTURE_RES, TEXTURE_RES),
         Vector(-1, -1, 1, 1, 0, TEXTURE_RES), Vector(-1, -1, -1, 1, 0, 0))};

Block::Block(const Vector &_pos) {
  pos = _pos;
  for (int i = 0; i < NUM_TRIANGLES_IN_BLOCK; i++) {
    faces[i] = default_faces[i] + _pos;
  }
}

Block::Block() {
  pos = Vector(0, 0, 0);
  for (int i; i < NUM_TRIANGLES_IN_BLOCK; i++) {
    faces[i] = default_faces[i] + pos;
  }
}

Face &Block::operator[](int index) { return faces[index]; }

bool Block::operator==(const Block &b2) const { return (pos == b2.pos); }