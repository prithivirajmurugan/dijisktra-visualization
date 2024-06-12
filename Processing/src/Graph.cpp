#include "Graph.h"
#include "OSMJson.h"

float euclideanDistance(float x1, float x2, float y1, float y2) {
  return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

Vector3 maps::graph::SelectClosestNode(Vector2 cursor) {
  Vector2 closestNode = fileparser::nodes[0];
  int id = 0;
  float closestDistance =
      euclideanDistance(cursor.x, closestNode.x, cursor.y, closestNode.y);
  for (int i = 1; i < fileparser::nodes.size(); i++) {
    Vector2 node = fileparser::nodes[i];
    float distance = euclideanDistance(cursor.x, node.x, cursor.y, node.y);
    if (distance < closestDistance) {
      closestDistance = distance;
      closestNode = node;
      id = i;
    }
  }
  return {closestNode.x, closestNode.y, (float)id};
}
