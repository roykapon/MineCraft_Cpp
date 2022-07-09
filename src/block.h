#include "face.h"

#define NUM_TRIANGLES_IN_BLOCK 12

class Block {
public:
  Vector pos;
  Face faces[NUM_TRIANGLES_IN_BLOCK];

  Block();

  Block(const Vector &pos);

  Face &operator[](int index);

  bool operator==(const Block &b2) const;
};
