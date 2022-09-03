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

void Renderer::init_z_buffer() {
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      z_buffer[y * width + x] = SCALAR_MAX;
    }
  }
}

void Renderer::project(ColoredVector &v, Pixel &p) {
  // assumes the point was already rotated
  p.source = v;
  SCALAR ratio = ffd / v.z;
  p.x = COORD_TO_PIXEL_X(v.x * ratio);
  p.y = COORD_TO_PIXEL_Y(v.y * ratio);
}

void Renderer::update_direction() {
  direction = get_direction(horizontal, vertical);
}

int Renderer::project(Face &face, Pixel *projected) {
  ColoredVector rotated[3];
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
      // find intersection
      SCALAR ratio = (EPSILON - rotated[(i + 1) % 3].z) /
                     (rotated[i].z - rotated[(i + 1) % 3].z);
      ColoredVector inter;
      interpolate(rotated[i], rotated[(i + 1) % 3], ratio, inter);
      // can be optimized (x and y pixel coordinates can be easily calculated)
      project(inter, projected[j]);
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
  Pixel curr;
  for (int y = line_extremum[0] + 1; y < line_extremum[1]; y++) {
    curr = interpolate_by_y(p1, p2, y);
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
    if (p.source.z > EPSILON) {
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
  Pixel *edges =
      (Pixel *)malloc((2 * (extremum[1] - extremum[0] + 1)) * sizeof(Pixel));
  init_edges(edges, extremum);
  Pixel line[2];
  for (int i = 0; i < len; i++) {
    line[0] = projected[i];
    line[1] = projected[(i + 1) % len];
    save_line(line, edges, extremum);
  }

  paint_face(edges, extremum);
  free(edges);
}

struct FaceComparator {
  Vector pos;

  FaceComparator(Vector _pos) { pos = _pos; }

  bool operator()(FACE_OBJECT &p1, FACE_OBJECT &p2) {
    return p1.first->average_dist(pos) < p2.first->average_dist(pos);
  }
};

void Renderer::render(Env &env) {
  init_picture();
  init_picture_colored();
  init_z_buffer();
  sort(env.visible_faces.begin(), env.visible_faces.end(), FaceComparator(pos));
  for (auto face_object : env.visible_faces) {
    Face &face = *(face_object.first);
    // should probably filter the faces before sorting
    if (decide_to_render(face)) {
      render(face);
    }
  }
}

bool Renderer::decide_to_render(Face &face) {
  Vector face_direction = normalized(face[0] - pos);
  /** assert the angle between the ray to the face and the normal is lower than
   90 degrees (otherwise the face cannot be seen from the camera's position) */
  SCALAR relative_direction = face_direction * face.normal;
  /** assert the angle between the camera's direction and the normal is between
   * 90 and 270 (otherwise the face cannot be seen from the camera's direction)
   */
  // should be changed (it is not enough to look only at a single vertex)
  // SCALAR relative_direction2 = direction * face_direction;
  return relative_direction < -100 * EPSILON; //&& relative_direction2 > 0;
}

int bound(int i, int upper_bound, int lower_bound) {
  return max(min(i, upper_bound), lower_bound);
}

void Renderer::paint_face(Pixel *edges, int *extremum) {
  int texture_x, texture_y;
  SCALAR x_coord, ratio, diff_x, diff_z, z;
  for (int y = max(extremum[0], 0); y <= min(extremum[1], height - 1); y++) {
    Pixel &left = edges[2 * (y - extremum[0]) + 0];
    Pixel &right = edges[2 * (y - extremum[0]) + 1];
    // actual work:
    diff_x = left.source.x - right.source.x;
    diff_z = left.source.z - right.source.z;
    // paint a single horizontal line
    for (int x = max(left.x, 0); x <= min(right.x, width - 2);) {
      x_coord = PIXEL_TO_COORD_X(x);
      // assumes ffd = 1!!!
      ratio = (right.source.x - right.source.z * x_coord) /
              ((diff_z)*x_coord - diff_x);
      z = INTERPOLATE(left.source.z, right.source.z, ratio);
      // check the z_buffer
      if (z < z_buffer[y * width + x] - EPSILON) {
        // paint the pixel
        paint_pixel(left, right, x, y, ratio);
        // set the pixel colored
        picture_colored[y * width + x] = (right.x - x);
        z_buffer[y * width + x] = z;
        x++;
      } else {
        x += (1 + picture_colored[y * width + x]);
      }
    }
  }
}

Renderer::~Renderer() { free(picture_colored); }

Pixel Renderer::interpolate_by_y(Pixel &p1, Pixel &p2, int y) {
  Pixel res;
  // assumes p2.y - p1.y != 0
  SCALAR ratio = (SCALAR)(p2.y - y) / (SCALAR)(p2.y - p1.y);
  res.x = round(INTERPOLATE(p1.x, p2.x, ratio));
  res.y = y;

  SCALAR y_coord = PIXEL_TO_COORD_Y(y);
  // assumes ffd = 1 !!!
  SCALAR ratio_3d =
      (p2.source.y - p2.source.z * y_coord) /
      ((p1.source.z - p2.source.z) * y_coord - p1.source.y + p2.source.y);
  interpolate(p1.source, p2.source, ratio_3d, res.source);
  return res;
}