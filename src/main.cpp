#include "camera.h"
#include <SDL2/SDL.h>
#include <iostream>
#include <sstream>

#define SPEED 0.01f
#define DELAY 30

using namespace std;

int game() {
  Vector pos = Vector(0, 0, -10);
  Camera camera = Camera(pos, 0, 0);

  Vector v1 = Vector(0, 0, 4, 1, 0, 0);
  Vector v2 = Vector(1, 0, 4, 1, 255, 0);
  Vector v3 = Vector(0, 1, 4, 1, 0, 255);

  Face face = Face(v1, v2, v3);
  // Pixel projected[3];

  SDL_Init(SDL_INIT_EVERYTHING);

  SDL_Window *window =
      SDL_CreateWindow("Shady MineCraft", SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, camera.width, camera.height, 0);
  SDL_Renderer *renderer =
      SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  bool isRunning = true;
  SDL_Event event;

  while (isRunning) {
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
          Vector offset = camera.direction * (SPEED * DELAY);
          // cout << "direction: " << offset << endl;

          camera.pos = camera.pos + offset;
          break;
        }

        case SDLK_s: {
          Vector offset = camera.direction * (SPEED * DELAY);

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

        stringstream ss;
        ss << "X: " << mouseX << " Y: " << mouseY;

        SDL_SetWindowTitle(window, ss.str().c_str());
        break;
      }
    }
    camera.update();

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);

    camera.render(face, renderer);

    // camera.project(face, projected);

    // int x1, y1, x2, y2;
    // for (int i = 0; i < 3; i++) {
    //   x1 = projected[i][X];
    //   y1 = projected[i][Y];
    //   x2 = projected[(i + 1) % 3][X];
    //   y2 = projected[(i + 1) % 3][Y];

    //   SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
    // }

    SDL_RenderPresent(renderer);

    SDL_Delay(DELAY);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}

int main(int argv, char **args) {
  game();
  // Vector source1 = Vector(0, 0, 0);
  // Vector source2 = Vector(100, 100, 0);

  // Pixel p1 = Pixel(99, 0, source1);
  // Pixel p2 = Pixel(0, 0, source2);

  // Pixel p3 = interpolate(p1, p2, 0.5);
  // cout << p3;

  // Camera camera = Camera(pos, 0, 0);

  // Vector v1 = Vector(0, 0, 2);
  // Vector v2 = Vector(1, 0, 2);
  // Vector v3 = Vector(0, 1, 2);

  // Face face = Face(v1, v2, v3);

  // Pixel *projected = camera.project(face);

  // for (int i = 0; i < 3; i++) {
  //   cout << projected[i];
  // }
  return 0;
}