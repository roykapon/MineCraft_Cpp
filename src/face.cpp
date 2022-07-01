#include "face.h"

using namespace std;

Vector &Face::operator[](int index) { return vertices[index % 3]; }
