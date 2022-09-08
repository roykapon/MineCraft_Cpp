#include "camera.h"

using namespace std;

// =========================== Camera ===========================

void Camera::init_picture() {
  for (int x = 0; x < WIDTH; x++) {
    for (int y = 0; y < HEIGHT; y++) {
      picture[y][x] = DEFAULT_COLOR;
    }
  }
}

/** paints a single pixel (to be implemented)*/
void Camera::paint_pixel(const Pixel &left, const Pixel &right, int x, int y,
                         SCALAR ratio) {
  // interpolate the texture coordinates
  const int texture_x =
      lround(INTERPOLATE(left.source.texture_x, right.source.texture_x, ratio));
  const int texture_y =
      lround(INTERPOLATE(left.source.texture_y, right.source.texture_y, ratio));

  // paint the pixel
  picture[HEIGHT - 1 - y][x] = COLOR(texture_x, texture_y, 255);
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
