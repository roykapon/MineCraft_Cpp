#ifndef CAMERA_H
#define CAMERA_H

#include "env.h"
#include "face.h"
#include "linlib.h"

#define DEFAULT_WIDTH 1080
#define DEFAULT_HEIGHT 540

#define DEFAULT_FFD 2.0f

class Camera {

public:
  Vector pos;
  double vertical, horizontal;
  Vector direction;
  double ffd;
  int width, height;
  Matrix world;
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
  }

  /** initialize picture_colored by setting each pixel to point to the following
   * one */
  void init_picture_colored();

  /** free memory allocated by the camera */
  void free_camera();

  /** Returns the camera's direction vector */
  void update_direction();

  /** Updates the world matrix of the camera */
  void update_world();

  /** Update the camera (currently only world)*/
  void update();

  /** given x coordinate, returnd the x index of the pixel*/
  int pixel_x(double x);

  /** given y coordinate, returnd the y index of the pixel*/
  int pixel_y(double y);

  /** Returns the projected pixels of a face in the camera */
  void project(Face &face, Pixel *projected);

  /** Returns the projected pixel of a point in the camera */
  void project(Vector &v, Pixel *p);

  /** Renders the face onto the camera */
  void render(Face &face, SDL_Renderer *renderer);

  /** Renders the list of faces onto the camera */
  void render(Env &env, SDL_Renderer *renderer);

  /** Given a list of edges, updates the pixture */
  void paint_face(Pixel *edges, int *extremum, SDL_Renderer *renderer);

  /** update the line to the edges struct */
  void save_line(Pixel *line, Pixel *edges, int *extremum,
                 SDL_Renderer *renderer);
  /** Returns true if the first face is closer to the camera than the second one
   */
  bool compare(Face *face1, Face *face2);

  /** given x, y coordinates, returns the closest x (on the right) that was "
    "not colored yet"
    */
  int get_next_x(int x, int y);
};
#endif