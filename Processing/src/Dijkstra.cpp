#include "Dijkstra.h"
#include "OSMJson.h"
#include <cmath>

maps::Dijkstra::SPT *maps::Dijkstra::spt;
std::vector<int> maps::Dijkstra::stepList;
int maps::Dijkstra::n = 0;
float maps::Dijkstra::frameTime = 0;
bool maps::Dijkstra::isRunning = false;
bool maps::Dijkstra::isFinished = false;
int maps::Dijkstra::startStop[2] = {0, 0};

void maps::Dijkstra::init(int n) {
  maps::Dijkstra::n = n;
  spt = new SPT[n];
  for (int i = 0; i < n; i++) {
    spt[i].weight = INFINITY;
    spt[i].parent = -1;
    spt[i].visited = false;
  }
  stepList.clear();
}

void maps::Dijkstra::start(int start, int end) {
  startStop[0] = start;
  startStop[1] = end;

  isRunning = true;
  spt[start].weight = 0;
  spt[start].visited = true;
  maps::AdjList::Point *temp = &maps::AdjList::array[start];

  if (temp->id != -1) {
    spt[temp->id].weight = sqrt(pow(maps::fileparser::nodes[start].x -
                                        maps::fileparser::nodes[temp->id].x,
                                    2) +
                                pow(maps::fileparser::nodes[start].y -
                                        maps::fileparser::nodes[temp->id].y,
                                    2));
    spt[temp->id].parent = start;
    stepList.push_back(temp->id);
  }
  while (temp->next != nullptr) {
    temp = temp->next;
    spt[temp->id].weight = sqrt(pow(maps::fileparser::nodes[start].x -
                                        maps::fileparser::nodes[temp->id].x,
                                    2) +
                                pow(maps::fileparser::nodes[start].y -
                                        maps::fileparser::nodes[temp->id].y,
                                    2));
    spt[temp->id].parent = start;
    stepList.push_back(temp->id);
  }
}

void maps::Dijkstra::dijkstra() {
  // check if end is reached
  if (spt[startStop[1]].visited) {
    isRunning = false;
    isFinished = true;
    return;
  }
  std::vector<int> temp;
  for (size_t i = 0; i < stepList.size(); i++) {
    // check adj nodes is visited
    if (!spt[stepList[i]].visited) {
      spt[stepList[i]].visited = true;
      maps::AdjList::Point *tempAdj = &maps::AdjList::array[stepList[i]];

      if (tempAdj->id != -1) {
        float weight = sqrt(pow(maps::fileparser::nodes[stepList[i]].x -
                                    maps::fileparser::nodes[tempAdj->id].x,
                                2) +
                            pow(maps::fileparser::nodes[stepList[i]].y -
                                    maps::fileparser::nodes[tempAdj->id].y,
                                2));
        if (spt[tempAdj->id].weight > spt[stepList[i]].weight + weight) {
          spt[tempAdj->id].weight = spt[stepList[i]].weight + weight;
          spt[tempAdj->id].parent = stepList[i];
          temp.push_back(tempAdj->id);
        }
      }

      while (tempAdj->next != nullptr) {
        tempAdj = tempAdj->next;
        float weight = sqrt(pow(maps::fileparser::nodes[stepList[i]].x -
                                    maps::fileparser::nodes[tempAdj->id].x,
                                2) +
                            pow(maps::fileparser::nodes[stepList[i]].y -
                                    maps::fileparser::nodes[tempAdj->id].y,
                                2));
        if (spt[tempAdj->id].weight > spt[stepList[i]].weight + weight) {
          spt[tempAdj->id].weight = spt[stepList[i]].weight + weight;
          spt[tempAdj->id].parent = stepList[i];
          temp.push_back(tempAdj->id);
        }
      }
    }
  }
  stepList = temp;
}