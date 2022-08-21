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

double intersection(const Vector *line1, const Vector *line2, const Vector &v,
                    Vector &res) {
  Vector direction1 = line1[1] - line1[0];
  Vector direction2 = line2[1] - line2[0];
  Matrix M = Matrix(direction1, (-1) * direction2, v);
  cout << M << endl;
  Matrix M_inverse;
  if (!inverse(M, M_inverse)) {
    return -1;
  }

  cout << M_inverse << endl;

  Vector diff = line2[0] - line1[0];
  Vector t = diff * M_inverse;
  if (t[0] < 0 || t[0] > 1 || t[1] < 0 || t[1] > 1) {
    return -1;
  }
  res = line2[0] + t[1] * direction2;
  return t[2];
}

// bool intersection(const Vector *line1, const Vector *line2, const Vector &v,
//                   Vector &res) {
//   Vector direction1 = line1[1] - line1[0];
//   Vector direction2 = line2[1] - line2[0];
//   Vector normal = cross(direction1, v);
//   double d = normal * line1[0];
//   // handle division by zero!
//   double t = (d - normal * line2[0]) / (normal * direction2);
//   cout << t << endl;
//   if (t < 0 || t > 1) {
//     return false;
//   }

//   res = line2[0] + (t * direction2);
//   double t2 = (res - line1[0]) * direction1;

//   return (t2 >= 0 && t2 <= 1);
// }

// remove res as it not useful
double intersection(const Vector &p, const Face &face, const Vector &v,
                    Vector &res) {
  // handle division by zero!
  double t = (face.d - face.normal * p) / (face.normal * v);
  cout << t << endl;
  if (t < 0) {
    return -1;
  }

  res = p + (t * v);
  if (!is_in(res, face)) {
    return -1;
  }
  return t;
}

bool is_in(const Vector &p, const Face &face) {
  // assumes the point is inside the face's plane
  Vector edge_normal;
  for (int i = 0; i < 3; i++) {
    edge_normal = cross(face[i + 1] - face[i], face.normal);
    // the normals are facing the inside
    cout << edge_normal;
    if (edge_normal * (p - face[i]) > 0) {
      return false;
    }
  }
  return true;
}

double intersection(const Face &face1, const Face &face2, Vector &v,
                    Vector &res) {
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
      // should be optimized so that edges that point the other way are not
      // considered
      t = intersection(line1, line2, v, inter);
      if (t >= 0) {
        min_t = min(min_t, t);
      }
    }
    t = intersection(face1[i], face2, v, inter);
    if (t >= 0) {
      min_t = min(min_t, t);
    }
  }
  return min_t;
}

Face operator*(const Face &face, const Matrix &M) {
  Face res;
  for (int i = 0; i < 3; i++) {
    Vector tmp = face[i];
    res[i] = tmp * M;
  }
  return res;
}
