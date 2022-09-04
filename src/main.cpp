#include "app.h"
#include <iostream>
#include <sstream>

using namespace std;

int game() {

  App app = App();

  app.game_loop();

  // app is deleted
  return 0;
}

// struct ThreadArgs {
//   // DWORD thread_id;
//   int x;
//   int y;
// };

// DWORD WINAPI thread_func(ThreadArgs *args) {
//   // cout << "hi from " args->thread_id << endl;
//   cout << "x = " << args->x << endl;
//   return 0;
// }

// int threading() {
//   DWORD thread_id[10];
//   HANDLE handles[10];
//   ThreadArgs thread_args[10];
//   for (int i = 0; i < 10; i++) {

//     thread_args[i].x = i;
//     handles[i] = CreateThread(NULL, 0,
//     (LPTHREAD_START_ROUTINE)&thread_func,
//                               &thread_args[i], 0, &thread_id[i]);

//     if (handles[i] == NULL) {
//       cout << "error in " << i << endl;
//       return -1;
//     }
//   }
//   for (int i = 0; i < 10; i++) {
//     WaitForSingleObject(handles[i], INFINITE);
//     CloseHandle(handles[i]);
//   }
//   cout << "finished waiting" << endl;

//   return 0;
// }

int main(int argv, char **args) {
  game();

  // Vector p1 = Vector(0, 0, 0);
  // Vector p2 = Vector(0, 1, 0);
  // Vector p3 = Vector(1, 0, 1.5);
  // Vector p4 = Vector(-1, 1, 1.5);
  // Vector v = Vector(0.1, 0, 1);
  // Vector line1[] = {p1, p2};
  // Vector line2[] = {p3, p4};

  // Vector inter;
  // SCALAR intersected = collision(line1, line2, v, inter);
  // cout << "intersected: " << intersected << endl;
  // cout << inter << endl;

  // Vector p1 = Vector(0, 0, 0);
  // Vector p2 = Vector(0, 1, 0);
  // Vector p3 = Vector(1, 0, 0);
  // Face face = Face(p1, p2, p3);

  // Vector p = Vector(0.1, 0.1, -10);
  // Vector v = Vector(0.02, 0, 1);
  // Vector inter;

  // SCALAR intersected = collision(p, face, v, inter);
  // cout << "intersected: " << intersected << endl;
  // cout << inter << endl;

  // Vector p1 = Vector(0, 1, 0, 0);
  // Vector p2 = Vector(0, -3, 15, 0);
  // Vector p3 = Vector(0.2, 0.5, 2, 0);
  // Vector p4 = Vector(0, 0, 0, 0);

  // Matrix M = Matrix(p1, p2, p3, p4);
  // Matrix res;

  // inverse(M, res);

  return 0;
}