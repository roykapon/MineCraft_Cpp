#ifndef CAMERA_H
#define CAMERA_H

#include "face.h"
#include "linlib.h"

#define DEFAULT_WIDTH 1080
#define DEFAULT_HEIGHT 540

#define DEFAULT_FFD 1.0f

class Camera {

public:
  Vector pos;
  float vertical, horizontal;
  Vector direction;
  float ffd;
  int width, height;
  Matrix world;
  Pixel **picture;
  int **picture_colored;

  Camera(Vector _pos = Vector(), float _vertical = 0, float _horizontal = 0,
         float _ffd = DEFAULT_FFD, int _width = DEFAULT_WIDTH,
         int _height = DEFAULT_HEIGHT) {
    pos = _pos;
    vertical = _vertical;
    horizontal = _horizontal;
    direction = get_direction();
    world = get_world();

    width = _width;
    height = _height;
    ffd = _ffd;
    // picture = (Pixel **)malloc(sizeof(Pixel) * (width * height));
    // picture_colored = (int **)malloc(sizeof(int) * (width * height));
  }
  /** Returns the camera's direction vector */
  Vector get_direction();

  /** Returns the world matrix of the camera */
  Matrix get_world();

  /** Update the camera (currently only world)*/
  void update();

  /** Returns the projected pixel of a point in the camera */
  Pixel project(Vector &v);

  /** Returns the projected pixels of a face in the camera */
  Pixel *project(Face &face);

  /** given x coordinate, returnd the x index of the pixel*/
  int pixel_x(float x);

  /** given y coordinate, returnd the y index of the pixel*/
  int pixel_y(float y);
};
#endif