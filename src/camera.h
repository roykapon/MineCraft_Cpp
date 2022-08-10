#ifndef CAMERA_H
#define CAMERA_H

#include "SDL2\x86_64-w64-mingw32\include\SDL2\SDL.h"
#include "renderer.h"

#define DEFAULT_WIDTH 1080
#define DEFAULT_HEIGHT 540

#define DEFAULT_FFD 1.0f

#define DEFAULT_COLOR 0

#define COLOR(R, G, B) (((Uint32)R) << 16) + (((Uint32)G) << 8) + ((Uint32)B)

class Camera : public Renderer {

public:
  Uint32 *picture;

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

  /** initialize picture by setting each pixel color to DEFAULT_COLOR */
  void init_picture() override;

  /** paints a single pixel (to be implemented)*/
  void paint_pixel(Pixel &left, Pixel &right, int x, int y, double diff_z,
                   double diff_x) override;

  /** free memory allocated by the camera */
  ~Camera();
};
#endif