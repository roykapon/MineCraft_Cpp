#pragma once

#include "env.h"
#include "face.h"
#include "linlib.h"
#include "pixel.h"
#include <iomanip>
#include <iostream>

#define DEFAULT_WIDTH 1080
#define DEFAULT_HEIGHT 540

#define DEFAULT_FFD 1.0f

class Renderer {

public:
  Vector pos;
  SCALAR vertical, horizontal;
  Vector direction;
  SCALAR ffd;
  int width, height;
  Matrix world;
  // a 2d array that holds the offset to the next unpainted pixel
  SCALAR *z_buffer;
  int *picture_colored;

  Renderer(Vector _pos = Vector(0, 0, 0), SCALAR _vertical = 0,
           SCALAR _horizontal = 0, SCALAR _ffd = DEFAULT_FFD,
           int _width = DEFAULT_WIDTH, int _height = DEFAULT_HEIGHT)
      : pos(_pos), vertical(_vertical), horizontal(_horizontal), width(_width),
        height(_height), ffd(_ffd) {
    update_direction();
    update_world();
    picture_colored = (int *)malloc(sizeof(int) * width * height);
    z_buffer = (SCALAR *)malloc(sizeof(SCALAR) * width * height);

    init_picture_colored();
    init_z_buffer();
  }

  /** initialize picture by setting each pixel color to DEFAULT */
  virtual void init_picture() {}

  /** paints a single pixel (to be implemented)*/
  virtual void paint_pixel(Pixel &left, Pixel &right, int x, int y,
                           SCALAR ratio) {}

  /** initialize picture_colored by setting each pixel to point to the
   * following one (value of 0)*/
  void init_picture_colored();

  /** initialize z_buffer by setting each pixel to have a maximal depth */
  void init_z_buffer();

  /** free memory allocated by the camera */
  ~Renderer();

  /** Updates the camera's direction vector as a function of the horizontal and
   * vertical angles */
  void update_direction();

  /** Updates the world matrix of the camera */
  void update_world();

  /** Update the camera's world matrix and direction */
  void update();

  /** given x coordinate, returns the x index of the pixel*/
#define COORD_TO_PIXEL_X(x) round(height *x) + width / 2;

  /** given y coordinate, returns the y index of the pixel*/
#define COORD_TO_PIXEL_Y(y) round(height *y) + height / 2;

  /** given pixel y , returns the y coordinate of the pixel  in 3d world*/
#define PIXEL_TO_COORD_Y(x) (SCALAR)(x - height / 2) / height;

/** given pixel x , returns the x coordinate of the pixel in 3d world*/
#define PIXEL_TO_COORD_X(x) (SCALAR)(x - width / 2) / height;

  /** updates the projected array with the pixels of a face when projected onto
   * the camera. Returns the legnth of the projected array*/
  int project(Face &face, Pixel *projected);

  /** Returns the projected pixel of a point in the camera */
  void project(ColoredVector &v, Pixel &p);

  /** Renders the face onto the camera */
  void render(Face &face);

  /** Renders the list of faces onto the camera */
  void render(Env &env);

  /** Returns true if the face should be rendered */
  bool decide_to_render(Face &face);

  /** Returns true if the face should be rendered (after the vertices where
   * projected) */
  bool decide_to_render2(Pixel *projected, int len);

  /** Given a list of edges, updates the pixture */
  void paint_face(Pixel *edges, int *extremum);

  /** update the line in the edges array */
  void save_line(Pixel *line, Pixel *edges, int *extremum);
  /** Returns true if the first face is closer to the camera than the second
   * one */
  bool compare(Face *face1, Face *face2);

  /** Returns interpolation of the pixels in the specified y pixel-index */
  Pixel interpolate_by_y(Pixel &p1, Pixel &p2, int y);
};