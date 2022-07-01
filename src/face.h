#ifndef FACE_H
#define FACE_H

#include "linlib.h"

#define NUM_CHANNELS 3
#define TEXTURE_WIDTH 10
#define TEXTURE_HEIGHT 10

class Face {

public:
  Vector vertices[3];
  int ***texture;

  Face(Vector v1 = Vector(), Vector v2 = Vector(), Vector v3 = Vector(),
       int ***_texture = default_texture()) {
    vertices[0] = v1;
    vertices[1] = v2;
    vertices[2] = v3;
    texture = _texture;
  }

  static int ***default_texture() {
    int ***texture = (int ***)malloc(sizeof(int) * NUM_CHANNELS *
                                     TEXTURE_WIDTH * TEXTURE_HEIGHT);
    // for (int x = 0; x < TEXTURE_WIDTH; x++) {
    //   for (int y = 0; y < TEXTURE_HEIGHT; y++) {
    //     int color[3] = {x, y, 0};
    //     texture[x][y] = color;
    //   }
    // }
    return texture;
  }

  /** Returns the vertex at the given index */
  Vector &operator[](int index);
};

#endif