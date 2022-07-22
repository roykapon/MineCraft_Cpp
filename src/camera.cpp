#include "camera.h"
#include <iomanip>
#include <iostream>
// remove
#include <unistd.h>
using namespace std;

// =========================== Camera ===========================

void Camera::update_world() {
  Vector offset = pos * (-1);
  Matrix shift = Matrix::shift(offset);
  Matrix rotate_x = Matrix::rotate(X_AXIS, -vertical);
  Matrix rotate_y = Matrix::rotate(Y_AXIS, -horizontal);
  world = shift * rotate_y;
  world = world * rotate_x;
}

void Camera::update() {
  update_world();
  update_direction();
}

void Camera::init_picture() {
  for (int x = 0; x < width; x++) {
    for (int y = 0; y < height; y++) {
      picture[y * width + x] = DEFAULT_COLOR;
    }
  }
}

void Camera::init_picture_colored() {
  for (int x = 0; x < width; x++) {
    for (int y = 0; y < height; y++) {
      picture_colored[y * width + x] = x;
    }
  }
}

void Camera::project(Vector &v, Pixel *p) {
  // assumes the point was already rotated
  p->source = v;
  double ratio = ffd / (p->source.z);
  p->x = coord_to_pixel_x(p->source.x * ratio);
  p->y = coord_to_pixel_y(p->source.y * ratio);
}

void Camera::update_direction() {
  direction.x = cos(vertical) * (-sin(horizontal));
  direction.y = sin(vertical);
  direction.z = cos(vertical) * cos(horizontal);
}

// bool Camera::face_too_close(Pixel *projected) {
//   // assumes a part of the face is in front of the camera
// }

// void Camera::handle_close_face(Pixel *projected, Pixel *edges, int *extremum)
// {
//   Pixel line[2];
//   bool intersected = false;
//   int i = 0;
//   for (int line_index = 0; line_index < 2; line_index++) {
//     for (; i < 3; i++) {
//       if ((line[i].source.z <= ffd && line[i + 1].source.z > ffd) ||
//           (line[i + 1].source.z <= ffd && line[i].source.z > ffd)) {
//         intersected = true;
//         // get intersection of the line and the picture plane;
//         double ratio = (ffd - line[i + 1].source.z) /
//                        (line[i].source.z - line[i + 1].source.z);
//         Vector intersection =
//             interpolate(projected[i].source, projected[i + 1].source, ratio);
//         // can be optimized (x and y pixel coordinates can be easily
//         calculated) project(intersection, &line[line_index]); i++; break;
//       }
//     }
//   }
//   save_line(line, edges, extremum);
// }

int Camera::project(Face &face, Pixel *projected) {
  Vector rotated[3];
  int j = 0;
  for (int i = 0; i < 3; i++) {
    rotated[i] = face[i] * world;
  }
  for (int i = 0; i < 3; i++) {
    // fisrt point is in front of the camera
    if (rotated[i].z > EPSILON) {
      project(rotated[i], &projected[j]);
      j++;
    }
    // one point is in front and one is behind the camera
    if ((rotated[(i + 1) % 3].z <= EPSILON && rotated[i].z > EPSILON) ||
        (rotated[i].z <= EPSILON && rotated[(i + 1) % 3].z > EPSILON)) {
      // find intersection
      double ratio = (EPSILON - rotated[(i + 1) % 3].z) /
                     (rotated[i].z - rotated[(i + 1) % 3].z);
      Vector intersection =
          interpolate(rotated[i], rotated[(i + 1) % 3], ratio);
      // can be optimized (x and y pixel coordinates can be easily calculated)
      project(intersection, &projected[j]);
      j++;
    }
    // else both points are behind the camera (do not add anything)
  }
  return j;
}

int Camera::coord_to_pixel_x(double x) {
  return round(min(height, width) * x) + width / 2;
}

int Camera::coord_to_pixel_y(double y) {
  return round(min(height, width) * y) + height / 2;
}

double Camera::pixel_to_coord_y(int y) {
  return (double)(y - height / 2) / min(height, width);
}

double Camera::pixel_to_coord_x(int x) {
  return (double)(x - width / 2) / min(height, width);
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

bool Camera::decide_to_render2(Pixel *projected, int len) {
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

void Camera::render(Face &face) {
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
  Vector pos;
  Comparator(Vector &_pos) { pos = _pos; }

  bool operator()(Face f1, Face f2) {
    return average_dist(f1, pos) < average_dist(f2, pos);
  }

  int paramA;
};

void Camera::render(Env &env) {
  init_picture();
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

void Camera::paint_face(Pixel *edges, int *extremum) {
  for (int y = max(extremum[0], 0); y <= min(extremum[1], height - 1); y++) {
    Pixel left = edges[2 * (y - extremum[0]) + 0];
    Pixel right = edges[2 * (y - extremum[0]) + 1];
    // actual work:
    double diff = left.source.x - right.source.x;
    int texture_x, texture_y;
    for (int x = get_next_x(max(left.x, 0), y); x <= min(right.x, width - 1);) {
      // interpolate the texture coordinates
      double x_coord = pixel_to_coord_x(x);
      // assumes ffd = 1!!!
      double ratio_3d = (right.source.x - right.source.z * x_coord) /
                        ((left.source.z - right.source.z) * x_coord -
                         left.source.x + right.source.x);
      texture_x =
          INTERPOLATE(left.source.texture_x, right.source.texture_x, ratio_3d);
      texture_y =
          INTERPOLATE(left.source.texture_y, right.source.texture_y, ratio_3d);

      // paint the pixel
      picture[y * width + x] = COLOR(texture_x, texture_y, 255);
      // set the pixel colored
      picture_colored[y * width + x] =
          bound(right.x + 1, width, picture_colored[y * width + x]);
      x = get_next_x(x + 1, y);
    }
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

void Camera::free_camera() {
  free(picture_colored);
  free(picture);
}

Pixel Camera::interpolate_by_y(Pixel &p1, Pixel &p2, int y) {
  Pixel res;
  // assumes p2.y - p1.y != 0
  double ratio = (double)(p2.y - y) / (double)(p2.y - p1.y);
  res.x = round(INTERPOLATE(p1.x, p2.x, ratio));
  res.y = y;

  double y_coord = pixel_to_coord_y(y);
  // assumes ffd = 1 !!!
  double ratio_3d =
      (p2.source.y - p2.source.z * y_coord) /
      ((p1.source.z - p2.source.z) * y_coord - p1.source.y + p2.source.y);
  res.source = interpolate(p1.source, p2.source, ratio_3d);
  return res;
}