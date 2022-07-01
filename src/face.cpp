#include "face.h"

Vector &Face::operator[](int index) { return vertices[index % 3]; }
