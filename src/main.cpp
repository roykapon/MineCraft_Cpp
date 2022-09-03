#include "camera.h"
#include <iostream>
#include <sstream>

#define SPEED 5.0f
#define JUMP_HEIGHT 2.0f

using namespace std;

int game() {
  // My code
  Vector pos = Vector(0, 0, -10);
  Camera camera = Camera(pos, 0, 0);

  Env env = Env();
  for (SCALAR y = 0.0f; y < 30; y += 2) {
    for (SCALAR x = 0.0f; x < 30; x += 2) {
      for (SCALAR z = 0.0f; z < 2; z += 2) {
        Vector block_pos = Vector(x, y, y);
        env.create_block(block_pos);
      }
    }
  }

  Vector itays_house = Vector(20, 2, 20);
  for (SCALAR x = 0.0f; x < 6; x += 2) {
    for (SCALAR y = 0.0f; y < 6; y += 2) {
      for (SCALAR z = 0.0f; z < 6; z += 2) {
        Vector block_pos = Vector(x, y, z);
        env.create_block(block_pos + itays_house);
      }
    }
  }

  // Vector block_pos = Vector(0.0f, 0.0f, 8.0f);
  // env.create_block(block_pos);

  Vector entity_pos = Vector(0, 10, 0);
  env.create_entity(entity_pos);

  Object &player = env.entities[0];

  env.update_visible_faces();

  // for (SCALAR x = 0.0f; x < 20; x += 2) {
  //   for (SCALAR z = 0.0f; z < 20; z += 2) {
  //     Vector block_pos = Vector(x, 0.0f, z) + Vector(0.0f, 10.0f, 10.0f);
  //     env.create_block(block_pos);
  //   }
  // }

  // library code
  SDL_Init(SDL_INIT_EVERYTHING);

  SDL_Window *window =
      SDL_CreateWindow("Shady MineCraft", SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, camera.width, camera.height, 0);

  SDL_SetRelativeMouseMode(SDL_TRUE);

  SDL_Renderer *renderer =
      SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB888,
                                           SDL_TEXTUREACCESS_STREAMING,
                                           camera.width, camera.height);

  bool isRunning = true;
  SDL_Event event;

  int start_time = 0;
  SCALAR frame_time = 0;
  //
  Vector offset;
  Vector right;
  while (isRunning) {

    SCALAR speed = SPEED * sqrt(frame_time);

    offset = Vector(0, 0, 0, 0);
    Vector right = cross(camera.direction, Vector(0, 1, 0, 0));

    start_time = SDL_GetTicks();

    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_QUIT:
        isRunning = false;
        break;

      case SDL_MOUSEBUTTONDOWN:
        switch (event.button.button) {
        case SDL_BUTTON_LEFT:
          env.player_interact(player, camera.direction);
          break;
        }
        break;

      case SDL_KEYDOWN:
        if (event.key.keysym.sym == SDLK_ESCAPE) {
          isRunning = false;
        }
        switch (event.key.keysym.sym) {
        case SDLK_w: {
          offset = camera.direction;
          // cout << "direction: " << offset << endl;

          // camera.pos = camera.pos + offset;
          break;
        }

        case SDLK_s: {
          offset = (-1) * camera.direction;

          // camera.pos = camera.pos + offset;
          break;
        }
        case SDLK_a: {
          offset = offset + right;

          // camera.pos = camera.pos + offset;
          break;
        }
        case SDLK_d: {
          offset = offset - right;

          // camera.pos = camera.pos + offset;
          break;
        }
        case SDLK_SPACE: {
          offset = offset + Vector(0, JUMP_HEIGHT, 0, 0);

          // camera.pos = camera.pos + offset;
          break;
        }
        case SDLK_LSHIFT: {
          offset = offset + Vector(0, -JUMP_HEIGHT, 0, 0);

          // camera.pos = camera.pos + offset;
          break;
        }
        }
        break;

      case SDL_MOUSEMOTION:
        int mouseX = event.motion.x;
        int mouseY = event.motion.y;

        camera.horizontal = -mouseX * 2 * PI / camera.width + PI;
        camera.vertical = -mouseY * 2 * PI / camera.height - PI;

        break;
      }
    }
    // =============================== updates ============================
    // Vector entity_offset = offset;
    // Vector entity_offset = Vector(0, 0, 0.7, 0);
    offset = normalized_horizontal(offset);
    offset.x *= speed;
    offset.z *= speed;
    player.horizontal = camera.horizontal;
    // player.vertical = camera.vertical;
    player.v = player.v + offset;
    // env.move_object(player, player.v);
    env.apply_physics(frame_time);

    camera.update_wrt_player(player);

    camera.render(env);

    SDL_UpdateTexture(texture, NULL, camera.picture,
                      camera.width * sizeof(Uint32));
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);

    // Display fps
    // converts from ms to seconds
    frame_time = (SCALAR)(SDL_GetTicks() - start_time) / 1000;
    SCALAR fps = (frame_time > 0) ? 1 / frame_time : 0.0f;
    stringstream ss;
    ss << "fps: " << fps;
    SDL_SetWindowTitle(window, ss.str().c_str());
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_DestroyTexture(texture);
  SDL_Quit();

  // free stuff here
  return 0;
}

// struct ThreadArgs {
//   // DWORD thread_id;
//   int x;
//   int y;
// };

// DWORD WINAPI thread_func(ThreadArgs *args) {
//   // cout << "hi from " args->thread_id << endl;
//   cout << "x = " << args->x << endl;
//   return 0;
// }

// int threading() {
//   DWORD thread_id[10];
//   HANDLE handles[10];
//   ThreadArgs thread_args[10];
//   for (int i = 0; i < 10; i++) {

//     thread_args[i].x = i;
//     handles[i] = CreateThread(NULL, 0,
//     (LPTHREAD_START_ROUTINE)&thread_func,
//                               &thread_args[i], 0, &thread_id[i]);

//     if (handles[i] == NULL) {
//       cout << "error in " << i << endl;
//       return -1;
//     }
//   }
//   for (int i = 0; i < 10; i++) {
//     WaitForSingleObject(handles[i], INFINITE);
//     CloseHandle(handles[i]);
//   }
//   cout << "finished waiting" << endl;

//   return 0;
// }

int main(int argv, char **args) {
  game();

  // Vector p1 = Vector(0, 0, 0);
  // Vector p2 = Vector(0, 1, 0);
  // Vector p3 = Vector(1, 0, 1.5);
  // Vector p4 = Vector(-1, 1, 1.5);
  // Vector v = Vector(0.1, 0, 1);
  // Vector line1[] = {p1, p2};
  // Vector line2[] = {p3, p4};

  // Vector inter;
  // SCALAR intersected = collision(line1, line2, v, inter);
  // cout << "intersected: " << intersected << endl;
  // cout << inter << endl;

  // Vector p1 = Vector(0, 0, 0);
  // Vector p2 = Vector(0, 1, 0);
  // Vector p3 = Vector(1, 0, 0);
  // Face face = Face(p1, p2, p3);

  // Vector p = Vector(0.1, 0.1, -10);
  // Vector v = Vector(0.02, 0, 1);
  // Vector inter;

  // SCALAR intersected = collision(p, face, v, inter);
  // cout << "intersected: " << intersected << endl;
  // cout << inter << endl;

  // Vector p1 = Vector(0, 1, 0, 0);
  // Vector p2 = Vector(0, -3, 15, 0);
  // Vector p3 = Vector(0.2, 0.5, 2, 0);
  // Vector p4 = Vector(0, 0, 0, 0);

  // Matrix M = Matrix(p1, p2, p3, p4);
  // Matrix res;

  // inverse(M, res);

  return 0;
}