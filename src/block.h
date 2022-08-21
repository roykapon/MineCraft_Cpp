#include "object.h"
#define NUM_TRIANGLES_IN_BLOCK 12

class Block : Object {
public:
  Block();

  Block(const Vector &pos);

  Block(const Block &other);

  ~Block();

  Face &operator[](int index);

  bool operator==(const Block &b2) const;
};
