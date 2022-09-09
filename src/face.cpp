#include "face.h"

using namespace std;

void Face::update_plane() {
  Vector edge1 = vertices[1] - vertices[0];
  Vector edge2 = vertices[2] - vertices[1];
  normal = cross(edge1, edge2);
  d = normal * (Vector)(vertices[0]);
}

void Face::update_edge_normals() {
  for (int i = 0; i < 3; i++) {
    edge_normals[i] = cross(vertices[(i + 1) % 3] - vertices[i], normal);
  }
}

const ColoredVector &Face::operator[](int index) const {
  const ColoredVector &res = vertices[index % 3];
  return res;
}

SCALAR Face::average_dist(const Vector &pos) {
  Vector center = vertices[0] * (1.0f / 3) + vertices[1] * (1.0f / 3) +
                  vertices[2] * (1.0f / 3);
  return distance(center, pos);
}

ColoredVector &Face::operator[](int index) { return vertices[index % 3]; }

Face operator+(const Face &face, const ColoredVector &pos) {
  Face res;
  for (int i = 0; i < 3; i++) {
    res[i] = face[i] + pos;
    res[i].texture_x = face[i].texture_x;
    res[i].texture_y = face[i].texture_y;
  }
  res.normal = face.normal;
  return res;
}

SCALAR line_line_collision(const Vector *line1, const Vector *line2,
                           const Vector &v, Vector &normal) {
  Vector direction1 = line1[1] - line1[0];
  Vector direction2 = line2[1] - line2[0];
  Matrix M = Matrix(direction1, (-1) * direction2, v);
  Matrix M_inverse;
  if (!inverse(M, M_inverse)) {
    return SCALAR_MAX;
  }
  Vector diff = line2[0] - line1[0];
  Vector t = diff * M_inverse;

  if (t[0] <= EPSILON || t[0] >= 1 - EPSILON || t[1] <= EPSILON ||
      t[1] >= 1 - EPSILON || t[2] < -EPSILON || t[2] >= 1) {
    return SCALAR_MAX;
  }
  normal = cross(line1[1] - line1[0], line2[1] - line2[0]);
  SCALAR sign = ((normal * v) > 0) ? -1.0f : 1.0f;
  normal *= sign;
  // make the object have some distance from the other object
  // SCALAR dist = EPSILON * t[2] * abs(normal * v) / (norm(v) * norm(v));
  // t[2] = max(t[2] - dist, 0.0f);
  return t[2];
}

// bool line_line_collision(const Pixel *line1, const Pixel *line2, Pixel &res)
// {
//   Vector v11 = Vector(line1[0].x, line1[0].y, 0, 0);
//   Vector v12 = Vector(line1[1].x, line1[1].y, 0, 0);
//   Vector v21 = Vector(line2[0].x, line2[0].y, 0, 0);
//   Vector v22 = Vector(line2[1].x, line2[1].y, 0, 0);
//   Matrix M = Matrix(v11 - v12, v21 - v22);
//   Matrix M_inverse;
//   if (!inverse(M, M_inverse, 2)) {
//     return false;
//   }
//   Vector diff = v21 - v11;
//   Vector t = diff * M_inverse;
//   if (t[0] < 0 || t[0] > 1 || t[1] < 0 || t[1] > 1) {
//     return false;
//   }
//   res.x = v11.x + t[0] * v12.x;
//   res.y = v11.y + t[0] * v12.y;
//   return true;
// }

SCALAR point_face_collision(const Vector &p, const Face &face, const Vector &v,
                            Vector &normal) {
  if (face.normal * v > -EPSILON) {
    return SCALAR_MAX;
  }
  // make the object have some distance from the other object
  SCALAR t = (face.d - face.normal * p) / (face.normal * v);
  if (t < 0) {
    return SCALAR_MAX;
  }

  Vector res = p + (t * v);
  if (!is_in(res, face)) {
    return SCALAR_MAX;
  }
  normal = face.normal;
  return t;
}

bool is_in(const Vector &p, const Face &face) {
  // assumes the point is inside the face's plane
  for (int i = 0; i < 3; i++) {
    // the normals are facing the outside
    if (face.edge_normals[i] * (p - face[i]) > -EPSILON) {
      return false;
    }
  }
  return true;
}

bool can_collide(const Face &face1, const Face &face2, const Vector &v) {
  if (face1.normal * v < EPSILON || face2.normal * v > -EPSILON) {
    return false;
  }
  // SCALAR max_angle = SCALAR_MIN;
  // for (int i = 0; i < 3; i++) {
  //   for (int j = 0; j < 3; j++) {
  //     max_angle = max(max_angle, ((Vector)face2[j] - (Vector)face1[i]) * v);
  //   }
  // }
  // return max_angle > 0;
  return true;
}

SCALAR face_face_collision(const Face &face1, const Face &face2,
                           const Vector &v, Vector &normal) {
  if (!can_collide(face1, face2, v)) {
    return SCALAR_MAX;
  }
  Vector line1[2];
  Vector line2[2];
  Vector curr_normal;
  SCALAR min_t = SCALAR_MAX;
  SCALAR t;
  for (int i = 0; i < 3; i++) {
    line1[0] = face1[i];
    line1[1] = face1[i + 1];
    for (int j = 0; j < 3; j++) {
      line2[0] = face2[j];
      line2[1] = face2[j + 1];
      t = line_line_collision(line1, line2, v, curr_normal);
      if (t < min_t) {
        min_t = t;
        normal = curr_normal;
      }
    }
    t = point_face_collision(face1[i], face2, v, curr_normal);
    if (t < min_t) {
      min_t = t;
      normal = curr_normal;
    }
    t = point_face_collision(face2[i], face1, (-1) * v, curr_normal);
    if (t < min_t) {
      min_t = t;
      normal = curr_normal * (-1);
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
    res[i] = face[i] * M;
  }
  res.update();
  return res;
}