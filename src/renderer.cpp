#include "renderer.h"
#include <iomanip>
#include <iostream>
// remove
#include <unistd.h>
using namespace std;

// =========================== Renderer ===========================

void Renderer::update_world() {
  Vector offset = pos * (-1);
  Matrix shift = Matrix::shift(offset);
  Matrix rotate_x = Matrix::rotate(X_AXIS, -vertical);
  Matrix rotate_y = Matrix::rotate(Y_AXIS, -horizontal);
  world = shift * rotate_y;
  world = world * rotate_x;
}

void Renderer::update() {
  update_world();
  update_direction();
}

void Renderer::init_picture_colored() {
  memset(picture_colored, 0, sizeof(int) * width * height);
}

void Renderer::project(Vector &v, Pixel &p) {
  // assumes the point was already rotated
  p.source = v;
  double ratio = ffd / v.z;
  p.x = COORD_TO_PIXEL_X(v.x * ratio);
  p.y = COORD_TO_PIXEL_Y(v.y * ratio);
}

void Renderer::update_direction() {
  direction.x = cos(vertical) * (-sin(horizontal));
  direction.y = sin(vertical);
  direction.z = cos(vertical) * cos(horizontal);
  direction.w = 0;
}

int Renderer::project(Face &face, Pixel *projected) {
  Vector rotated[3];
  int j = 0;
  for (int i = 0; i < 3; i++) {
    rotated[i] = face[i] * world;
  }
  for (int i = 0; i < 3; i++) {
    // fisrt point is in front of the camera
    if (rotated[i].z > EPSILON) {
      project(rotated[i], projected[j]);
      j++;
    }
    // one point is in front and one is behind the camera
    if ((rotated[(i + 1) % 3].z <= EPSILON && rotated[i].z > EPSILON) ||
        (rotated[i].z <= EPSILON && rotated[(i + 1) % 3].z > EPSILON)) {
      // find collision
      double ratio = (EPSILON - rotated[(i + 1) % 3].z) /
                     (rotated[i].z - rotated[(i + 1) % 3].z);
      Vector collision = interpolate(rotated[i], rotated[(i + 1) % 3], ratio);
      // can be optimized (x and y pixel coordinates can be easily calculated)
      project(collision, projected[j]);
      j++;
    }
    // else both points are behind the camera (do not add anything)
  }
  return j;
}

void update_edges(Pixel &p, Pixel *edges, int *extremum) {
  if (p.x < edges[2 * (p.y - extremum[0]) + 0].x) {
    edges[2 * (p.y - extremum[0]) + 0] = p;
  }
  if (p.x > edges[2 * (p.y - extremum[0]) + 1].x) {
    edges[2 * (p.y - extremum[0]) + 1] = p;
  }
}

void Renderer::save_line(Pixel *line, Pixel *edges, int *extremum) {
  int line_extremum[2];
  get_extremum(line, 2, line_extremum, 0, height);
  Pixel &p1 = line[0];
  Pixel &p2 = line[1];
  // handle "edge" cases (get the joke?)
  if (p1.y >= extremum[0] && p1.y <= extremum[1]) {
    update_edges(p1, edges, extremum);
  }
  if (p2.y >= extremum[0] && p2.y <= extremum[1]) {
    update_edges(p2, edges, extremum);
  }
  for (int y = line_extremum[0] + 1; y < line_extremum[1]; y++) {
    Pixel curr = interpolate_by_y(p1, p2, y);
    update_edges(curr, edges, extremum);
  }
}

void init_edges(Pixel *edges, int *extremum) {
  for (int y = extremum[0]; y <= extremum[1]; y++) {
    edges[2 * (y - extremum[0]) + 0] = Pixel(INT_MAX, y);
    edges[2 * (y - extremum[0]) + 1] = Pixel(INT_MIN, y);
  }
}

bool Renderer::decide_to_render2(Pixel *projected, int len) {
  /** temp_i is true if the face is not out of the picture in the i'th
   direction (left, right, down, up) */
  bool temp_left = false, temp_right = false, temp_bottom = false,
       temp_top = false, temp_forward = false;
  for (int i = 0; i < len; i++) {
    Pixel &p = projected[i];
    if (p.x >= 0) {
      temp_left = true;
    }
    if (p.x < width) {
      temp_right = true;
    }
    if (p.y >= 0) {
      temp_bottom = true;
    }
    if (p.y < height) {
      temp_top = true;
    }
    if (p.source.z > 0 + EPSILON) {
      temp_forward = true;
    }
  }
  return temp_left && temp_right && temp_bottom && temp_top && temp_forward;
}

void Renderer::render(Face &face) {
  Pixel projected[4];
  int len = project(face, projected);
  if (!decide_to_render2(projected, len)) {
    return;
  }
  int extremum[2];
  get_extremum(projected, len, extremum, 0, height - 1);
  if (extremum[1] - extremum[0] <= 0) {
    // happens only if all points are under the view or above the view
    return;
  }
  // Array of the pixels of the edges of the face on every y coordinate
  Pixel edges[2 * (extremum[1] - extremum[0] + 1)];
  init_edges(edges, extremum);
  Pixel line[2];
  for (int i = 0; i < len; i++) {
    line[0] = projected[i];
    line[1] = projected[(i + 1) % len];
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
  Renderer *renderer;

  Comparator(Renderer *_renderer) { renderer = _renderer; }

  bool operator()(Face *f1, Face *f2) {
    // double t1, t2;
    // Face rotated1 = (*f1) * renderer->world;
    // Pixel projected1[4];
    // Pixel line1[2];
    // int len1 = renderer->project(*f1, projected1);
    // Pixel projected2[4];
    // Pixel line2[2];
    // Pixel inter;
    // int len2 = renderer->project(*f2, projected2);
    // for (int i = 0; i < len1; i++) {
    //   line1[0] = projected1[i];
    //   line1[1] = projected1[(i + 1) % len1];
    //   for (int j = 0; j < len2; j++) {
    //     line2[0] = projected2[i];
    //     line2[1] = projected2[(i + 1) % len1];
    //     if (line_line_collision(line1, line2, inter)) {
    //       t1 = (f1->d - *f1->normal);
    //     }
    //   }
    // }

    return average_dist(*f1, renderer->pos) < average_dist(*f2, renderer->pos);
  }

  int paramA;
};

void Renderer::render(Env &env) {
  init_picture();
  init_picture_colored();
  sort(env.visible_faces.begin(), env.visible_faces.end(), Comparator(this));
  for (Face *face : env.visible_faces) {
    // should probably filter the faces before sorting
    if (decide_to_render(*face)) {
      render(*face);
    }
  }
}

bool Renderer::decide_to_render(Face &face) {
  Vector face_direction = face[0] - pos;
  /** assert the angle between the ray to the face and the normal is lower than
   90 degrees (otherwise the face cannot be seen from the camera's position) */
  double relative_direction = face_direction * face.normal;
  /** assert the angle between the camera's direction and the normal is between
   * 90 and 270 (otherwise the face cannot be seen from the camera's direction)
   */
  // should be changed (it is not enough to look only at a single vertex)
  // double relative_direction2 = direction * face_direction;
  return relative_direction < 0; //&& relative_direction2 > 0;
}

int bound(int i, int upper_bound, int lower_bound) {
  return max(min(i, upper_bound), lower_bound);
}

void Renderer::paint_face(Pixel *edges, int *extremum) {
  for (int y = max(extremum[0], 0); y <= min(extremum[1], height - 1); y++) {
    Pixel left = edges[2 * (y - extremum[0]) + 0];
    Pixel right = edges[2 * (y - extremum[0]) + 1];
    // actual work:
    double diff_x = left.source.x - right.source.x;
    double diff_z = left.source.z - right.source.z;
    int texture_x, texture_y;
    for (int x = get_next_x(max(left.x, 0), y); x <= min(right.x, width - 1);) {
      // paint the pixel
      paint_pixel(left, right, x, y, diff_z, diff_x);
      // set the pixel colored
      picture_colored[y * width + x] = (right.x + 1 - x);
      x = get_next_x(x + 1, y);
    }
  }
}

int Renderer::get_next_x(int x, int y) {
  while (x < width && picture_colored[y * width + x] != 0) {
    x += picture_colored[y * width + x];
  }
  return x;
}

Renderer::~Renderer() { free(picture_colored); }

Pixel Renderer::interpolate_by_y(Pixel &p1, Pixel &p2, int y) {
  Pixel res;
  // assumes p2.y - p1.y != 0
  double ratio = (double)(p2.y - y) / (double)(p2.y - p1.y);
  res.x = round(INTERPOLATE(p1.x, p2.x, ratio));
  res.y = y;

  double y_coord = PIXEL_TO_COORD_Y(y);
  // assumes ffd = 1 !!!
  double ratio_3d =
      (p2.source.y - p2.source.z * y_coord) /
      ((p1.source.z - p2.source.z) * y_coord - p1.source.y + p2.source.y);
  res.source = interpolate(p1.source, p2.source, ratio_3d);
  return res;
}