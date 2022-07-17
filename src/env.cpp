#include "env.h"

using namespace std;

/** creates a block in the environment at the given position */
void Env::create_block(const Vector &pos) { blocks[pos] = Block(pos); }

/** Updates visible faces list */
void Env::update_visible_faces() {
  visible_faces.clear();
  for (auto pos_block : blocks) {
    Vector pos = pos_block.first;
    Block &block = pos_block.second;
    for (int i = 0; i < NUM_TRIANGLES_IN_BLOCK; i++) {
      if (blocks.count(pos + block[i].normal * 2) == 0)
        visible_faces.push_back(block[i]);
    }
  }
}
