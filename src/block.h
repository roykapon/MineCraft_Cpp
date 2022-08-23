#include "object.h"
#define NUM_TRIANGLES_IN_BLOCK 12

class Block : public Object {
public:
  Block();

  Block(const Vector &pos);

  Block(const Block &other);

  ~Block();

  bool operator==(const Block &b2) const;
};
