#include "camera.h"
#include <iomanip>
#include <iostream>
// remove
#include <unistd.h>
using namespace std;

// =========================== Camera ===========================

void Camera::init_picture() {
  for (int x = 0; x < width; x++) {
    for (int y = 0; y < height; y++) {
      picture[y * width + x] = DEFAULT_COLOR;
    }
  }
}

/** paints a single pixel (to be implemented)*/
void Camera::paint_pixel(Pixel &left, Pixel &right, int x, int y, double diff_z,
                         double diff_x) {
  // interpolate the texture coordinates
  double x_coord = PIXEL_TO_COORD_X(x);
  // assumes ffd = 1!!!
  double ratio_3d =
      (right.source.x - right.source.z * x_coord) / ((diff_z)*x_coord - diff_x);
  int texture_x =
      INTERPOLATE(left.source.texture_x, right.source.texture_x, ratio_3d);
  int texture_y =
      INTERPOLATE(left.source.texture_y, right.source.texture_y, ratio_3d);

  // paint the pixel
  picture[y * width + x] = COLOR(texture_x, texture_y, 255);
}

/** free picture and picture_colored */
Camera::~Camera() {
  // calls Renderer destructor
  free(picture);
}