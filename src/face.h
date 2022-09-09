#pragma once

#include "linlib.h"
#include "pixel.h"
#include "SDL.h"


#define NUM_CHANNELS 3
#define TEXTURE_WIDTH 10
#define TEXTURE_HEIGHT 10

#define TEXTURE_RES 255

class Face {

public:
  ColoredVector vertices[3];
  Vector rotated[3];
  Vector edge_normals[3];
  Vector normal;
  SCALAR d;
  Uint32 (*texture)[TEXTURE_WIDTH];

  Face(const ColoredVector v1 = ColoredVector(), const ColoredVector v2 = ColoredVector(),
       const ColoredVector v3 = ColoredVector(), Uint32(*_texture)[TEXTURE_WIDTH] = NULL) {
    vertices[0] = v1;
    vertices[1] = v2;
    vertices[2] = v3;
    update();
    texture = _texture;
  }

  void update() {
    update_plane();
    update_edge_normals();
  }

  /** Updates the normal and "d" value
   * (such that the equasion representing the plane is x*N=d)
   *  according to the vertices (assumes clock-wise order)*/
  void update_plane();

  /** Updates the normal according to the vertices (assumes clock-wise order)*/
  void update_edge_normals();

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

  /** Returns a const reference to the vertex at the given index */
  const ColoredVector &operator[](int index) const;

  /** Returns a mutable reference to the vertex at the given index */
  ColoredVector &operator[](int index);

  SCALAR average_dist(const Vector &pos);
};

/** Returns the shifted face by the given position */
Face operator+(const Face &face, const Vector &pos);

/** Returns the minimal distance of the vertices of the face from the given
 * point */
SCALAR mindist(const Face &face, const Vector &pos);

/* returns true if the given point is inside the given face.
assumes that the point is already on the plane. */
bool is_in(const Vector &p, const Face &face);

/* returns the portion of the velocity vector that the 1st section travels
 * before hitting the 2nd section */
SCALAR line_line_collision(const Vector *line1, const Vector *line2,
                           const Vector &v, Vector &normal);

/* returns the portion of the velocity vector that the point travels
 * before hitting the face */
SCALAR point_face_collision(const Vector &p, const Face &face, const Vector &v,
                            Vector &normal);

/* returns the portion of the velocity vector that the 1st face travels
 * before hitting the 2nd face */
SCALAR face_face_collision(const Face &face1, const Face &face2,
                           const Vector &v, Vector &normal);

/* returns the face where each vertex of the given face is multiplied by the
 * matrix */
Face operator*(const Face &face, const Matrix &M);
