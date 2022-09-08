#pragma once

#include "include\SDL.h"
#include "renderer.h"

#define DEFAULT_WIDTH 1080
#define DEFAULT_HEIGHT 540

#define DEFAULT_FFD 1.0f

#define DEFAULT_COLOR 0

#define DEFAULT_DISTANCE 10.0f

#define COLOR(R, G, B) (((Uint32)R) << 16) + (((Uint32)G) << 8) + ((Uint32)B)

class Camera : public Renderer {

public:
  Uint32 (*picture)[WIDTH];
  SCALAR distance;

  Camera(Vector _pos = Vector(0, 0, 0), SCALAR _vertical = 0,
         SCALAR _horizontal = 0, SCALAR _ffd = DEFAULT_FFD,
         SCALAR _distance = DEFAULT_DISTANCE) {
    pos = _pos;
    vertical = _vertical;
    horizontal = _horizontal;
    update_direction();
    update_world();
    ffd = _ffd;
    distance = _distance;
    picture_colored = new int[HEIGHT][WIDTH];
    init_picture_colored();
    picture = new Uint32[HEIGHT][WIDTH];
  }

  /** initialize picture by setting each pixel color to DEFAULT_COLOR */
  void init_picture() override;

  /** paints a single pixel (to be implemented)*/
  void paint_pixel(const Pixel &left, const Pixel &right, int x, int y,
                   SCALAR ratio) override;

  /** update the camera's position to folllow the player in 3rd person */
  void update_wrt_player(Object &player);

  /** free memory allocated by the camera */
  ~Camera();
};