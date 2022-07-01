#include "camera.h"
#include <iomanip>
#include <iostream>
// remove
#include <unistd.h>
using namespace std;

// =========================== Camera ===========================

Matrix Camera::get_world() {
  Vector offset = pos * (-1);
  Matrix shift = Matrix::shift(offset);
  Matrix rotate_x = Matrix::rotate(X, -vertical);
  Matrix rotate_y = Matrix::rotate(Y, -horizontal);
  world = shift * rotate_x;
  world = world * rotate_y;
  return world;
}

void Camera::update() {
  world = get_world();
  direction = get_direction();
}

void Camera::project(Vector &v, Pixel *p) {
  Vector rotated = v * world;
  float ratio = ffd / rotated.z;
  p->x = pixel_x(rotated.x * ratio);
  p->y = pixel_y(rotated.y * ratio);
  p->source = v;
  p->texture_x = v.texture_x;
  p->texture_y = v.texture_y;
}

Vector Camera::get_direction() {
  float x = cos(vertical) * (-sin(horizontal));
  float y = sin(vertical);
  float z = cos(vertical) * cos(horizontal);
  return Vector(x, y, z);
}

void Camera::project(Face &face, Pixel *projected) {
  for (int i = 0; i < 3; i++) {
    project(face[i], &projected[i]);
  }
}

int Camera::pixel_x(float x) { return round(width * x) + width / 2; }

int Camera::pixel_y(float y) { return round(height * y) + height / 2; }

void Camera::save_line(Pixel *line, Pixel *edges, int *extremum,
                       SDL_Renderer *renderer) {
  int line_extremum[2] = {INT_MAX, INT_MIN};
  get_extremum(line, 2, line_extremum, 0, height);
  for (int y = line_extremum[0]; y <= line_extremum[1]; y++) {
    float ratio = (float)(line[1][Y] - y) / (float)(line[1][Y] - line[0][Y]);
    Pixel curr = interpolate(line[0], line[1], ratio);
    if (curr[X] < edges[2 * (y - extremum[0]) + 0][X]) {
      edges[2 * (y - extremum[0]) + 0] = curr;
    }
    if (curr[X] > edges[2 * (y - extremum[0]) + 1][X]) {
      edges[2 * (y - extremum[0]) + 1] = curr;
    }
  }
}

void init_edges(Pixel *edges, int *extremum) {
  for (int y = extremum[0]; y <= extremum[1]; y++) {
    edges[2 * (y - extremum[0]) + 0] = Pixel(1000000, y);
    edges[2 * (y - extremum[0]) + 1] = Pixel(-1000000, y);
  }
}

void Camera::render(Face &face, SDL_Renderer *renderer) {
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
    save_line(line, edges, extremum, renderer);
  }

  paint_face(edges, extremum, renderer);
}

int bound(int i, int upper_bound, int lower_bound) {
  return max(min(i, upper_bound), lower_bound);
}

void Camera::paint_face(Pixel *edges, int *extremum, SDL_Renderer *renderer) {
  for (int y = extremum[0]; y <= extremum[1]; y++) {
    Pixel left = edges[2 * (y - extremum[0]) + 0];
    Pixel right = edges[2 * (y - extremum[0]) + 1];
    if (right[X] == left[X]) {
      SDL_SetRenderDrawColor(renderer, left.texture_x, left.texture_y, 0, 255);
      SDL_RenderDrawPoint(renderer, left[X], y);
      continue;
    }
    for (int x = max(left[X], 0); x <= min(right[X], width); x++) {

      float ratio = (x - left[X]) / (right[X] - left[X]);
      Pixel curr = interpolate(left, right, ratio);
      SDL_SetRenderDrawColor(renderer, curr.texture_x, curr.texture_y, 0, 255);
      SDL_RenderDrawPoint(renderer, x, y);
    }
  }
}
