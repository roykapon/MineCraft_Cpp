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
void Camera::paint_pixel(Pixel &left, Pixel &right, int x, int y,
                         SCALAR ratio) {
  // interpolate the texture coordinates
  int texture_x =
      INTERPOLATE(left.source.texture_x, right.source.texture_x, ratio);
  int texture_y =
      INTERPOLATE(left.source.texture_y, right.source.texture_y, ratio);

  // paint the pixel
  picture[(height - 1 - y) * width + x] = COLOR(texture_x, texture_y, 255);
}

/** free picture and picture_colored */
Camera::~Camera() {
  // calls Renderer destructor
  free(picture);
}

void Camera::update_wrt_player(Object &player) {
  update_direction();
  pos = player.pos - direction * distance;
  update_world();
}
