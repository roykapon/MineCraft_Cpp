#ifndef RENDERER_H
#define RENDERER_H

#include "env.h"
#include "face.h"
#include "linlib.h"
#include <iomanip>
#include <iostream>

#define DEFAULT_WIDTH 1080
#define DEFAULT_HEIGHT 540

#define DEFAULT_FFD 1.0f

class Renderer {

public:
  Vector pos;
  double vertical, horizontal;
  Vector direction;
  double ffd;
  int width, height;
  Matrix world;
  int *picture_colored;

  Renderer(Vector _pos = Vector(), double _vertical = 0, double _horizontal = 0,
           double _ffd = DEFAULT_FFD, int _width = DEFAULT_WIDTH,
           int _height = DEFAULT_HEIGHT) {
    pos = _pos;
    vertical = _vertical;
    horizontal = _horizontal;
    update_direction();
    update_world();

    width = _width;
    height = _height;
    ffd = _ffd;
    picture_colored = (int *)malloc(sizeof(int) * width * height);
    init_picture_colored();
  }

  /** initialize picture by setting each pixel color to DEFAULT */
  virtual void init_picture() {}

  /** paints a single pixel (to be implemented)*/
  virtual void paint_pixel(Pixel &left, Pixel &right, int x, int y,
                           double diff_z, double diff_x) {}

  /** initialize picture_colored by setting each pixel to point to the
   * following one */
  void init_picture_colored();

  /** free memory allocated by the camera */
  ~Renderer();

  /** Returns the camera's direction vector */
  void update_direction();

  /** Updates the world matrix of the camera */
  void update_world();

  /** Update the camera (currently only world)*/
  void update();

  /** given x coordinate, returns the x index of the pixel*/
#define COORD_TO_PIXEL_X(x) round(height *x) + width / 2;

  /** given y coordinate, returns the y index of the pixel*/
#define COORD_TO_PIXEL_Y(y) round(height *y) + height / 2;

  /** given pixel y , returns the y coordinate of the pixel  in 3d world*/
#define PIXEL_TO_COORD_Y(x) (double)(x - height / 2) / height;

/** given pixel x , returns the x coordinate of the pixel in 3d world*/
#define PIXEL_TO_COORD_X(x) (double)(x - width / 2) / height;

  /** updates the projected array with the pixels of a face when projected onto
   * the camera. Returns the legnth of the projected array*/
  int project(Face &face, Pixel *projected);

  /** Returns the projected pixel of a point in the camera */
  void project(Vector &v, Pixel *p);

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

  /** given x, y coordinates, returns the closest x (on the right) that was "
        "not colored yet" */
  int get_next_x(int x, int y);

  /** Returns interpolation of the pixels in the specified y pixel-index */
  Pixel interpolate_by_y(Pixel &p1, Pixel &p2, int y);
};
#endif