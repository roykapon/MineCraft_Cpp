#include "camera.h"

using namespace std;

// =========================== Camera ===========================

void Camera::update_world() {
  Vector offset = pos * (-1);
  Matrix shift = Matrix::shift(offset);
  Matrix rotate_x = Matrix::rotate(X_AXIS, -vertical);
  Matrix rotate_y = Matrix::rotate(Y_AXIS, -horizontal);
  world = shift * rotate_x;
  world = world * rotate_y;
}

void Camera::update() {
  update_world();
  update_direction();
}

void Camera::init_picture_colored() {
  for (int x = 0; x < width; x++) {
    for (int y = 0; y < height; y++) {
      picture_colored[y * width + x] = x;
    }
  }
}

void Camera::project(Vector &v, Pixel *p) {
  Vector rotated = v * world;
  double ratio = ffd / (rotated.z + ffd);
  p->x = pixel_x(rotated.x * ratio);
  p->y = pixel_y(rotated.y * ratio);
  p->source = v;
  p->texture_x = v.texture_x;
  p->texture_y = v.texture_y;
}

void Camera::update_direction() {
  direction.x = cos(vertical) * (-sin(horizontal));
  direction.y = sin(vertical);
  direction.z = cos(vertical) * cos(horizontal);
}

void Camera::project(Face &face, Pixel *projected) {
  for (int i = 0; i < 3; i++) {
    project(face[i], &projected[i]);
  }
}

int Camera::pixel_x(double x) {
  return round(min(height, width) * x) + width / 2;
}

int Camera::pixel_y(double y) {
  return round(min(height, width) * y) + height / 2;
}

void update_edges(Pixel &p, Pixel *edges, int *extremum) {
  if (p.x < edges[2 * (p.y - extremum[0]) + 0].x) {
    edges[2 * (p.y - extremum[0]) + 0] = p;
  }
  if (p.x > edges[2 * (p.y - extremum[0]) + 1].x) {
    edges[2 * (p.y - extremum[0]) + 1] = p;
  }
}

void Camera::save_line(Pixel *line, Pixel *edges, int *extremum) {
  int line_extremum[2];
  get_extremum(line, 2, line_extremum, 0, height);
  Pixel &p1 = line[0];
  Pixel &p2 = line[1];
  if (p1.y >= extremum[0] && p1.y <= extremum[1]) {
    update_edges(p1, edges, extremum);
  }
  if (p2.y >= extremum[0] && p2.y <= extremum[1]) {
    update_edges(p2, edges, extremum);
  }

  for (int y = line_extremum[0] + 1; y < line_extremum[1]; y++) {
    double ratio = (double)(p2.y - y) / (double)(p2.y - p1.y);
    Pixel curr = interpolate(p1, p2, ratio);
    curr.y = y;
    update_edges(curr, edges, extremum);
  }
}

void init_edges(Pixel *edges, int *extremum) {
  for (int y = extremum[0]; y <= extremum[1]; y++) {
    edges[2 * (y - extremum[0]) + 0] = Pixel(INT_MAX, y);
    edges[2 * (y - extremum[0]) + 1] = Pixel(INT_MIN, y);
  }
}

void Camera::render(Face &face) {
  Pixel projected[3];
  project(face, projected);
  int extremum[2] = {INT_MAX, INT_MIN};
  get_extremum(projected, 3, extremum, 0, height);
  // Array of the pixels of the edges of the face on every y coordinate
  if (extremum[1] - extremum[0] <= 0) {
    return;
  }
  Pixel edges[2 * (extremum[1] - extremum[0] + 1)];
  init_edges(edges, extremum);
  Pixel line[2];
  for (int i = 0; i < 3; i++) {
    line[0] = projected[i];
    line[1] = projected[(i + 1) % 3];
    save_line(line, edges, extremum);
  }

  paint_face(edges, extremum);
}

double average_dist(Face &face, Vector &pos) {
  Vector center =
      face[0] * (1.0f / 3) + face[1] * (1.0f / 3) + face[2] * (1.0f / 3);
  return distance(center, pos);
}

struct Comparator {
  Vector pos;
  Comparator(Vector &_pos) { pos = _pos; }

  bool operator()(Face f1, Face f2) {
    return average_dist(f1, pos) < average_dist(f2, pos);
  }

  int paramA;
};

void Camera::render(Env &env) {
  init_picture_colored();
  sort(env.visible_faces.begin(), env.visible_faces.end(), Comparator(pos));
  for (Face face : env.visible_faces) {
    // should probably filter the faces before sorting
    if (decide_to_render(face)) {
      render(face);
    }
  }
}

bool Camera::decide_to_render(Face &face) {
  Vector face_direction = face[0] - pos;
  double relative_direction = face_direction * face.normal;
  double relative_direction2 = direction * face_direction;
  return relative_direction < 0 && relative_direction2 > 0;
}

int bound(int i, int upper_bound, int lower_bound) {
  return max(min(i, upper_bound), lower_bound);
}

struct ThreadArgs {
  int y;
  Pixel *edges;
  int *extremum;
  Camera *camera;
};

DWORD WINAPI paint_line(ThreadArgs *args) {
  // get arguments
  int y = args->y;
  Pixel *edges = args->edges;
  int *extremum = args->extremum;
  Camera *camera = args->camera;

  int width = camera->width;
  int *picture_colored = camera->picture_colored;

  Pixel left = edges[2 * (y - extremum[0]) + 0];
  Pixel right = edges[2 * (y - extremum[0]) + 1];
  // handle edge cases
  if (max(left.x, 0) > min(right.x, width - 1)) {
    return -1;
  }
  if (max(left.x, 0) == min(right.x, width - 1)) {
    if (picture_colored[right.y * width + max(left.x, 0)] == right.x) {
      SDL_SetRenderDrawColor(renderer, left.texture_x, left.texture_y, 0, 255);
      SDL_RenderDrawPoint(renderer, left.x, y);
    }
    return -1;
  }
  double diff = right.x - left.x;
  for (int x = camera->get_next_x(max(left.x, 0), y);
       x <= min(right.x, width - 1);) {
    // interpolate the texture coordinates
    double ratio = ((double)(x - left.x)) / diff;
    int texture_x = left.texture_x * ratio + right.texture_x * (1.0f - ratio);
    int texture_y = left.texture_y * ratio + right.texture_y * (1.0f - ratio);
    // paint the pixel
    SDL_SetRenderDrawColor(renderer, texture_x, texture_y, 0, 255);
    SDL_RenderDrawPoint(renderer, x, y);
    // set the pixel colored
    picture_colored[y * width + x] =
        bound(right.x + 1, width, picture_colored[y * width + x]);
    // get next uncolored pixel in that line
    x = camera->get_next_x(x + 1, y);
  }
  return 0;
}

void Camera::paint_face(Pixel *edges, int *extremum) {
  int y_start = max(extremum[0], 0);
  int y_end = min(extremum[1], height - 1);
  int y_range = y_end - y_start + 1;
  if (y_range < 0) {
    return;
  }
  DWORD thread_id[y_range];
  HANDLE handles[y_range];
  ThreadArgs thread_args[y_range];

  for (int y = y_start; y <= y_end; ++y) {
    thread_args[y - y_start].y = y;
    thread_args[y - y_start].edges = edges;
    thread_args[y - y_start].extremum = extremum;
    thread_args[y - y_start].camera = this;

    handles[y - y_start] =
        CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&paint_line,
                     &thread_args[y - y_start], 0, &thread_id[y - y_start]);

    if (handles[y - y_start] == NULL) {
      cout << "error in " << y << endl;
      return;
    }
  }
  for (int i = 0; i < y_range; i++) {
    WaitForSingleObject(handles[i], INFINITE);
    CloseHandle(handles[i]);
  }
}

int Camera::get_next_x(int x, int y) {
  if (x > width - 1) {
    return x;
  }
  int next_x = picture_colored[y * width + x];
  while (next_x != x && next_x < width) {
    x = next_x;
    next_x = picture_colored[y * width + x];
  }
  return next_x;
}

void Camera::free_camera() { free(picture_colored); }