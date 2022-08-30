#include "face.h"

using namespace std;

void Face::update_plane() {
  Vector edge1 = vertices[1] - vertices[0];
  Vector edge2 = vertices[2] - vertices[1];
  normal = cross(edge1, edge2);
  d = normal * vertices[0];
}

const Vector &Face::operator[](int index) const {
  const Vector &res = vertices[index % 3];
  return res;
}

Vector &Face::operator[](int index) { return vertices[index % 3]; }

Face operator+(const Face &face, const Vector &pos) {
  Face res;
  for (int i = 0; i < 3; i++) {
    res[i] = face[i] + pos;
    res[i].texture_x = face[i].texture_x;
    res[i].texture_y = face[i].texture_y;
  }
  res.normal = face.normal;
  return res;
}

double line_line_collision(const Vector *line1, const Vector *line2,
                           const Vector &v) {
  Vector direction1 = line1[1] - line1[0];
  Vector direction2 = line2[1] - line2[0];
  Matrix M = Matrix(direction1, (-1) * direction2, v);
  Matrix M_inverse;
  if (!inverse(M, M_inverse)) {
    return 1;
  }
  Vector diff = line2[0] - line1[0];
  Vector t = diff * M_inverse;
  if (t[0] < EPSILON || t[0] > 1 - EPSILON || t[1] < EPSILON ||
      t[1] > 1 - EPSILON || t[2] < -EPSILON) {
    return 1;
  }
  return t[2];
}

bool line_line_collision(const Pixel *line1, const Pixel *line2, Pixel &res) {
  Vector v11 = Vector(line1[0].x, line1[0].y, 0, 0);
  Vector v12 = Vector(line1[1].x, line1[1].y, 0, 0);
  Vector v21 = Vector(line2[0].x, line2[0].y, 0, 0);
  Vector v22 = Vector(line2[1].x, line2[1].y, 0, 0);
  Matrix M = Matrix(v11 - v12, v21 - v22);
  Matrix M_inverse;
  if (!inverse(M, M_inverse, 2)) {
    return false;
  }
  Vector diff = v21 - v11;
  Vector t = diff * M_inverse;
  if (t[0] < 0 || t[0] > 1 - 0 || t[1] < 0 || t[1] > 1) {
    return false;
  }
  res.x = v11.x + t[0] * v12.x;
  res.y = v11.y + t[0] * v12.y;
  return true;
}

double point_face_collision(const Vector &p, const Face &face,
                            const Vector &v) {
  // assumes face.normal * v != 0
  double t = (face.d - face.normal * p) / (face.normal * v);
  if (t < 0) {
    return 1;
  }

  Vector res = p + (t * v);
  if (!is_in(res, face)) {
    return 1;
  }
  return t;
}

bool is_in(const Vector &p, const Face &face) {
  // assumes the point is inside the face's plane
  Vector edge_normal;
  for (int i = 0; i < 3; i++) {
    edge_normal = cross(face[i + 1] - face[i], face.normal);
    // the normals are facing the outside
    if (edge_normal * (p - face[i]) > -EPSILON) {
      return false;
    }
  }
  return true;
}

double face_face_collision(const Face &face1, const Face &face2,
                           const Vector &v, Vector &normal) {
  if (face1.normal * v <= EPSILON || face2.normal * v >= -EPSILON) {
    return 1;
  }
  Vector line1[2];
  Vector line2[2];
  Vector inter;
  double min_t = 1;
  double t;
  for (int i = 0; i < 3; i++) {
    line1[0] = face1[i];
    line1[1] = face1[i + 1];
    for (int j = 0; j < 3; j++) {
      line2[0] = face2[j];
      line2[1] = face2[j + 1];
      t = line_line_collision(line1, line2, v);
      if (t < min_t) {
        min_t = t;
        normal = cross(line1[1] - line1[0], line2[1] - line2[0]);
        double sign = ((normal * v) > 0) ? -1.0 : 1.0;
        normal = normal * sign;
      }
    }
    t = point_face_collision(face1[i], face2, v);
    if (t < min_t) {
      min_t = t;
      normal = face2.normal;
    }
    t = point_face_collision(face2[i], face1, (-1) * v);
    if (t < min_t) {
      min_t = t;
      normal = face1.normal * (-1);
    }
  }
  return min_t;
}

// bool face_face_intersection(const Face &face1, const Face &face2,
//                             Vector *line) {
//   if (norm(face1.normal - face2.normal)) {
//     // in the comparator we should return something like face1.d > face2.d
//     return false;
//   }
//   Vector line_direction = cross(face1.normal, face2.normal);
//   Vector face1_line[2];
//   face1_line[0] = face1.normal * face1.d;
//   // line[0].w = 1; (might be needed)
//   face1_line[1] = face1_line[0] + cross(line_direction, face1.normal);
//   if (!line_face_intersection(face2, face1_line, line[0])) {
//     return false;
//   }
//   line[1] = line[0] + line_direction
//   return true;
// }

Face operator*(const Face &face, const Matrix &M) {
  Face res;
  for (int i = 0; i < 3; i++) {
    Vector tmp = face[i];
    res[i] = tmp * M;
  }
  res.update_plane();
  return res;
}