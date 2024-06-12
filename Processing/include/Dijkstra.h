#pragma once
#include "AdjList.h"
#include <vector>

namespace maps {
namespace Dijkstra {
typedef struct SPT {
  float weight;
  int parent;
  bool visited;
} SPT;

extern SPT *spt;
extern std::vector<int> stepList;
extern int n;
extern float frameTime;
extern bool isRunning;
extern bool isFinished;
extern int startStop[2];

void init(int n);
void start(int start, int end);
void dijkstra();
} // namespace Dijkstra
} // namespace maps