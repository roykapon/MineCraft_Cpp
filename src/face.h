#ifndef FACE_H
#define FACE_H

#include "linlib.h"

#define NUM_CHANNELS 3
#define TEXTURE_WIDTH 10
#define TEXTURE_HEIGHT 10

#define TEXTURE_RES 255

class Face {

public:
  Vector vertices[3];
  Vector normal;
  double d;
  int ***texture;

  Face(Vector v1 = Vector(), Vector v2 = Vector(), Vector v3 = Vector(),
       int ***_texture = default_texture()) {
    vertices[0] = v1;
    vertices[1] = v2;
    vertices[2] = v3;
    update_plane();
    texture = _texture;
  }

  /** Updates the normal according to the vertices (assumes clock-wise order)*/
  void update_plane();

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
  const Vector &operator[](int index) const;

  /** Returns a mutable reference to the vertex at the given index */
  Vector &operator[](int index);
};

/** Returns the shifted face by the given position */
Face operator+(const Face &face, const Vector &pos);

/** Returns the minimal distance of the vertices of the face from the given
 * point */
double mindist(const Face &face, const Vector &pos);

bool is_in(const Vector &p, const Face &face);

double intersection(const Vector *line1, const Vector *line2, const Vector &v,
                    Vector &res);

double intersection(const Vector &p, const Face &face, const Vector &v,
                    Vector &res);

double intersection(const Face &face1, const Face &face2, Vector &v,
                    Vector &res);

Face operator*(const Face &face, const Matrix &M);
#endif