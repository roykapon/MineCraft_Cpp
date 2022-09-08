#pragma once

#include "camera.h"
#include <sstream>

const Vector spawn_point = Vector(0.0f, 10.0f, 0.0f);

class App {
  Env env;
  Camera camera;
  Object &player;

  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Texture *screen;
  bool is_running;

  void init_terrain();

  void init_screen();

  void render();

  void step(SCALAR frame_time);

  void poll_input(Vector &player_direction);

public:
  App() : env(), camera(), player(env.create_entity(spawn_point)) {
    init_terrain();
    init_screen();
  }

  void game_loop();

  ~App();
};