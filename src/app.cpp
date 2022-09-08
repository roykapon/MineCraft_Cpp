#include "app.h"

void App::init_terrain() {
  for (SCALAR x = 0.0f; x < 30; x += 2) {
    for (SCALAR y = 0.0f; y < 2; y += 2) {
      for (SCALAR z = 0.0f; z < 30; z += 2) {
        Vector block_pos = Vector(x, z, z);
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
  // env.update_visible_faces();

  // for (SCALAR x = 0.0f; x < 20; x += 2) {
  //   for (SCALAR z = 0.0f; z < 20; z += 2) {
  //     Vector block_pos = Vector(x, 0.0f, z) + Vector(0.0f, 10.0f, 10.0f);
  //     env.create_block(block_pos);
  //   }
  // }
}

void App::init_screen() {
  // library code
  SDL_Init(SDL_INIT_EVERYTHING);

  window = SDL_CreateWindow("Shady MineCraft", SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
  // hack the mouse (muhahahaha)
  SDL_SetRelativeMouseMode(SDL_TRUE);
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  screen = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB888,
                             SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);

  is_running = true;
}

void App::render() {
  camera.render(env);

  SDL_UpdateTexture(screen, NULL, camera.picture, WIDTH * sizeof(Uint32));
  SDL_RenderCopy(renderer, screen, NULL, NULL);
  SDL_RenderPresent(renderer);
}

void App::game_loop() {
  int start_time = 0;
  SCALAR frame_time = 0;

  while (is_running) {

    start_time = SDL_GetTicks();

    step(frame_time);

    // =============================== updates ============================

    render();

    // Display fps
    // converts from ms to seconds
    frame_time = (SCALAR)(SDL_GetTicks() - start_time) / 1000;
    SCALAR fps = (frame_time > 0) ? 1 / frame_time : 0.0f;
    stringstream ss;
    ss << "fps: " << fps;
    SDL_SetWindowTitle(window, ss.str().c_str());
  }
}

void App::step(SCALAR frame_time) {
  SCALAR speed = SPEED * (frame_time);

  Vector player_direction = Vector(0, 0, 0, 0);
  poll_input(player_direction);
  player_direction.x *= speed;
  player_direction.z *= speed;

  player.v = player.v + player_direction;
  player.horizontal = camera.horizontal;
  env.apply_physics(frame_time);

  camera.update_wrt_player(player);
}

void App::poll_input(Vector &player_direction) {
  SDL_Event event;
  Vector right = cross(camera.direction, Vector(0, 1, 0, 0));

  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_QUIT:
      is_running = false;
      break;

    case SDL_MOUSEBUTTONDOWN:
      switch (event.button.button) {
      case SDL_BUTTON_LEFT:
        env.player_interact(player, camera.direction, ADD_BLOCK);
        break;
      case SDL_BUTTON_RIGHT:
        env.player_interact(player, camera.direction, REMOVE_BLOCK);
        break;
      }
      break;

    case SDL_KEYDOWN:
      if (event.key.keysym.sym == SDLK_ESCAPE) {
        is_running = false;
      }
      switch (event.key.keysym.sym) {
      case SDLK_w: {
        player_direction += camera.direction;
        break;
      }

      case SDLK_s: {
        player_direction -= camera.direction;
        break;
      }
      case SDLK_a: {
        player_direction += right;
        break;
      }
      case SDLK_d: {
        player_direction -= right;
        break;
      }
      case SDLK_SPACE: {
        player_direction += JUMP;
        break;
      }
      case SDLK_LSHIFT: {
        player_direction -= JUMP;
        break;
      }
      }
      break;

    case SDL_MOUSEMOTION:
      int mouseX = event.motion.x;
      int mouseY = event.motion.y;

      camera.horizontal = -mouseX * 2 * PI / WIDTH + PI;
      camera.vertical = -mouseY * 2 * PI / HEIGHT - PI;

      break;
    }
  }
  player_direction = normalized_horizontal(player_direction);
}

App::~App() {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_DestroyTexture(screen);
  SDL_Quit();
}