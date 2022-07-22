#ifndef CAMERA_H
#define CAMERA_H

#include "SDL2/SDL_Thread.h"
#include "env.h"
#include "face.h"
#include "linlib.h"
// #include <Windows.h>
#include <iomanip>
#include <iostream>

#define DEFAULT_WIDTH 1080
#define DEFAULT_HEIGHT 540

#define DEFAULT_FFD 1.0f

#define DEFAULT_COLOR 0

#define COLOR(R, G, B) (((Uint32)R) << 16) + (((Uint32)G) << 8) + ((Uint32)B)

class Camera {

public:
  Vector pos;
  double vertical, horizontal;
  Vector direction;
  double ffd;
  int width, height;
  Matrix world;
  Uint32 *picture;
  int *picture_colored;

  Camera(Vector _pos = Vector(), double _vertical = 0, double _horizontal = 0,
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
    picture = (Uint32 *)malloc(width * height * sizeof(Uint32));
  }

  /** initialize picture_colored by setting each pixel to point to the following
   * one */
  void init_picture();

  /** initialize picture by setting each pixel color to DEFAULT_COLOR */
  void init_picture_colored();

  /** free memory allocated by the camera */
  void free_camera();

  /** Returns the camera's direction vector */
  void update_direction();

  /** Updates the world matrix of the camera */
  void update_world();

  /** Update the camera (currently only world)*/
  void update();

  /** given x coordinate, returns the x index of the pixel*/
  int coord_to_pixel_x(double x);

  /** given y coordinate, returns the y index of the pixel*/
  int coord_to_pixel_y(double y);

  /** given pixel y , returns the y coordinate of the pixel  in 3d world*/
  double pixel_to_coord_y(int y);

  /** given pixel x , returns the x coordinate of the pixel in 3d world*/
  double pixel_to_coord_x(int x);

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

  // /** given left and right edges, paints the line between them */
  // void paint_line(Pixel &left, Pixel &right, int *extremum);

  /** Returns true if a part of the face is in front of the camera and a part
   * is behind */
  bool face_too_close(Pixel *projected);

  /** Updates edges with the line of the intersection between the picture's
   * plane and the face (assumes there axists such) */
  void handle_close_face(Pixel *projected, Pixel *edges, int *extremum);

  /** update the line in the edges array */
  void save_line(Pixel *line, Pixel *edges, int *extremum);
  /** Returns true if the first face is closer to the camera than the second
   * one
   */
  bool compare(Face *face1, Face *face2);

  /** given x, y coordinates, returns the closest x (on the right) that was "
        "not colored yet"
        */
  int get_next_x(int x, int y);

  /** Returns interpolation of the pixels in the specified y pixel-index */
  Pixel interpolate_by_y(Pixel &p1, Pixel &p2, int y);
};
#endif