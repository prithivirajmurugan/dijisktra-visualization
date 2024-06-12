#include "Renderer.h"
#include "AdjList.h"
#include "Dijkstra.h"
#include "Graph.h"
#include "NodeAnim.h"
#include "OSMJson.h"
#include "inputs.h"
#include <iostream>

Camera2D maps::renderer::camera;
RenderTexture2D maps::renderer::target;
Vector4 *maps::renderer::bounds;
int maps::renderer::activeScene;
std::future<void> maps::renderer::isLoaded;
int maps::renderer::dataLoadStatus;

void maps::renderer::init(int screenWidth, int screenHeight,
                          const char *title) {
  activeScene = 0;
  dataLoadStatus = 0;
  camera = {0};
  camera.zoom = 1.0f;
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(screenWidth, screenHeight, title);
  MaximizeWindow();
  SetTargetFPS(60);
}

void maps::renderer::run() {
  while (!WindowShouldClose()) {
    ProcessInput(&camera);
    BeginDrawing();
    BeginMode2D(camera);
    ClearBackground(Color{17, 17, 16, 255});
    if (activeScene == 0) {
      DrawText("Dijkstra",
               GetScreenWidth() / 2 - MeasureText("Dijkstra", 60) / 2,
               GetScreenHeight() / 2 - 60, 60, Color{105, 61, 228, 255});
      DrawText("Pathfinding Visualizer",
               GetScreenWidth() / 2 -
                   MeasureText("Pathfinding Visualizer", 40) / 2,
               GetScreenHeight() / 2 + 20, 40, Color{105, 61, 228, 255});
      switch (dataLoadStatus) {
      case 0:
        DrawText("Press Enter to load data",
                 GetScreenWidth() / 2 -
                     MeasureText("Press Enter to load data", 20) / 2,
                 GetScreenHeight() / 2 + 150, 20, Color{155, 212, 242, 255});
        break;
      case 1:
        DrawText("Loading data...",
                 GetScreenWidth() / 2 - MeasureText("Loading data...", 20) / 2,
                 GetScreenHeight() / 2 + 150, 20, Color{155, 212, 242, 255});
        break;
      case 2:
        DrawText("Reading file...",
                 GetScreenWidth() / 2 - MeasureText("Reading file...", 20) / 2,
                 GetScreenHeight() / 2 + 150, 20, Color{155, 212, 242, 255});
        break;
      case 3:
        DrawText("Parsing data...",
                 GetScreenWidth() / 2 - MeasureText("Parsing data...", 20) / 2,
                 GetScreenHeight() / 2 + 150, 20, Color{155, 212, 242, 255});
      case 4:
        DrawText("Standardizing data...",
                 GetScreenWidth() / 2 -
                     MeasureText("Standardizing data...", 20) / 2,
                 GetScreenHeight() / 2 + 150, 20, Color{155, 212, 242, 255});
        break;
      case 5:
        DrawText("Scaling data...",
                 GetScreenWidth() / 2 - MeasureText("Scaling data...", 20) / 2,
                 GetScreenHeight() / 2 + 150, 20, Color{155, 212, 242, 255});
        break;
      default:
        break;
      }
      DrawText("By Prithiviraj", 10, GetScreenHeight() - 30, 20,
               Color{105, 61, 228, 255});
      if (IsKeyPressed(KEY_ENTER) && dataLoadStatus == 0) {
        std::cout << "Starting loading data!" << std::endl;
        dataLoadStatus = 1;
        isLoaded = std::async(std::launch::async, AsyncDataLoader);
      }
      if (dataLoadStatus != 0 && isLoaded.wait_for(std::chrono::seconds(0)) ==
                                       std::future_status::ready) {
        activeScene = 1;
        std::cout << "Data loaded!" << std::endl;
        InitMapTexture();
      }
    } else {
      DrawTexturePro(
          target.texture,
          {0, 0, (float)target.texture.width, (float)-target.texture.height},
          {0, 0, (float)GetScreenWidth(), (float)GetScreenWidth()}, {0, 0}, 0,
          WHITE);
      maps::NodeAnim::UpdateNode(GetFrameTime());
      DrawCursor();
      DrawDijkstra();
      DrawSelectedNodes();
      EndMode2D();
      DrawText("Istanbul", 20, GetScreenHeight() - 50, 40,
               Color{105, 61, 228, 255});
      DrawText("By Prithiviraj", 50, GetScreenHeight() - 15, 10,
               Color{105, 61, 228, 255});
    }
    EndDrawing();
  }
  CloseWindow();
}

void maps::renderer::InitMapTexture() {
  int scale = 8;
  target =
      LoadRenderTexture(GetScreenWidth() * scale, GetScreenWidth() * scale);
  BeginTextureMode(target);
  ClearBackground(Color{17, 17, 16, 255});
  DrawFromAdjList(scale);
  EndTextureMode();
  bounds = fileparser::GetBounds();
  std::cout << "Bounds: " << bounds->x << " " << bounds->y << " " << bounds->z
            << " " << bounds->w << std::endl;
}

void maps::renderer::AsyncDataLoader() {
  dataLoadStatus = 2;
  fileparser::ReadFile("./Maps/simpleIST2.json");
  dataLoadStatus = 3;
  fileparser::ParseData();
  dataLoadStatus = 4; // standardizing data
  fileparser::StandardizeData();
  dataLoadStatus = 5; // scaling data
  fileparser::ScaleData(GetScreenWidth());
}

void maps::renderer::DrawSelectedNodes() {
  for (int i = 0; i < selectedPointsCount; i++) {
    Vector3 node = selectedPoints[i];
    if (maps::NodeAnim::nodeAnimList[i].isStarted ||
        maps::NodeAnim::nodeAnimList[i].isFinished) {
      DrawCircle(node.x, node.y, maps::NodeAnim::nodeAnimList[i].radius,
                 maps::NodeAnim::nodeAnimList[i].color);
    }
  }
}

void maps::renderer::DrawCursor() {
  Vector2 mousePos = GetScreenToWorld2D(GetMousePosition(), camera);
  Vector3 closestNode = maps::graph::SelectClosestNode(mousePos);
  DrawCircle(closestNode.x, closestNode.y, 2.f, Color{240, 240, 240, 255});
}

void maps::renderer::DrawNodes() {
  for (int i = 0; i < fileparser::nodes.size(); i++) {
    Vector2 node = fileparser::nodes[i];
    DrawRectangle(node.y + 50, node.x + 50, 1.f, 1.f,
                  Color{113, 254, 190, 255});
  }
}

void maps::renderer::DrawHighways() {
  for (int i = 0; i < fileparser::lines.size(); i++) {
    Vector4 line = fileparser::lines[i];
    DrawLineEx({line.y + 50, line.x + 50}, {line.w + 50, line.z + 50}, 1.f,
               Color{110, 110, 110, 255});
  }
}

void maps::renderer::DrawFromAdjList(int scale) {
  for (int i = 0; i < AdjList::capacity; i++) {
    AdjList::Point *temp = &AdjList::array[i];
    while (temp != nullptr) {
      Vector2 start = {fileparser::nodes[i].x * scale,
                       fileparser::nodes[i].y * scale};
      Vector2 end = {fileparser::nodes[temp->id].x * scale,
                     fileparser::nodes[temp->id].y * scale};
      DrawLineEx(start, end, scale, Color{70, 62, 88, 255});
      temp = temp->next;
    }
  }
}

void maps::renderer::DrawDijkstra() {
  if (maps::Dijkstra::isRunning) {
    maps::Dijkstra::dijkstra();
    for (int i = 0; i < maps::Dijkstra::n; i++) {
      if (Dijkstra::spt[i].parent != -1) {
        Vector2 start = fileparser::nodes[Dijkstra::spt[i].parent];
        Vector2 end = fileparser::nodes[i];
        DrawLineEx(start, end, 1.f, Color{155, 212, 242, 255});
      }
    }
  }

  // draw path from start to end
  if (maps::Dijkstra::isFinished) {
    int endId = maps::Dijkstra::startStop[1];
    int parent = maps::Dijkstra::spt[endId].parent;
    while (parent != -1) {
      Vector2 start = fileparser::nodes[parent];
      Vector2 end = fileparser::nodes[endId];
      DrawLineEx(start, end, 2.f, Color{105, 61, 228, 255});
      endId = parent;
      parent = maps::Dijkstra::spt[endId].parent;
    }
  }
}