#include "camera.h"
#include <SDL2/SDL.h>
#include <iostream>
#include <sstream>

#define SPEED 0.10f

using namespace std;

int game() {
  // My code
  Vector pos = Vector(0, 0, -10);
  Camera camera = Camera(pos, 0, 0);
  //

  SDL_Init(SDL_INIT_EVERYTHING);

  SDL_Window *window =
      SDL_CreateWindow("Shady MineCraft", SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, camera.width, camera.height, 0);
  SDL_Renderer *renderer =
      SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  bool isRunning = true;
  SDL_Event event;

  // My code

  Env env = Env();
  // Vector v1 = Vector(0, 0, 4, 1, 0, 0);
  // Vector v2 = Vector(1, 0, 4, 1, 255, 0);
  // Vector v3 = Vector(0, 1, 4, 1, 0, 255);
  // Face face = Face(v1, v2, v3);
  // env.visible_faces.push_back(face);
  Vector block_pos = Vector(0.0f, 0.0f, 0.0f);
  Vector block_pos2 = Vector(2.0f, 0.0f, 0.0f);
  env.create_block(block_pos);
  env.create_block(block_pos2);
  int start_time = 0;
  int frame_time = 0;
  //

  while (isRunning) {
    start_time = SDL_GetTicks();

    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_QUIT:
        isRunning = false;
        break;

      case SDL_KEYDOWN:
        if (event.key.keysym.sym == SDLK_ESCAPE) {
          isRunning = false;
        }
        switch (event.key.keysym.sym) {
        case SDLK_w: {
          Vector offset = camera.direction * (SPEED * sqrt(frame_time));
          // cout << "direction: " << offset << endl;

          camera.pos = camera.pos + offset;
          break;
        }

        case SDLK_s: {
          Vector offset = camera.direction * (SPEED * sqrt(frame_time));

          camera.pos = camera.pos - offset;
          break;
        }
        }
        break;

      case SDL_MOUSEMOTION:
        int mouseX = event.motion.x;
        int mouseY = event.motion.y;

        camera.horizontal = -mouseX * 2 * PI / camera.width + PI;
        camera.vertical = mouseY * 2 * PI / camera.height - PI;

        break;
      }
    }
    camera.update();

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);

    camera.render(env, renderer);

    SDL_RenderPresent(renderer);

    // Display fps
    frame_time = SDL_GetTicks() - start_time;
    double fps = (frame_time > 0) ? 1000.0f / frame_time : 0.0f;
    stringstream ss;
    ss << "fps: " << fps;
    SDL_SetWindowTitle(window, ss.str().c_str());
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  // free stuff here
  camera.free_camera();
  return 0;
}

int main(int argv, char **args) {
  game();
  return 0;
}