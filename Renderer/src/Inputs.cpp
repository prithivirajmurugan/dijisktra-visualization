#include "inputs.h"
#include "raymath.h"
#include "Graph.h"
#include "Dijkstra.h"
#include "AdjList.h"
#include "NodeAnim.h"
#include "Renderer.h"
#include <iostream>

Vector3 maps::renderer::selectedPoints[2];
int maps::renderer::selectedPointsCount = 0;

namespace maps {
    namespace renderer{
        void ProcessInput(Camera2D *camera){
            if(maps::renderer::activeScene != 0){
                // Dragging the camera
                if(IsMouseButtonDown(MOUSE_BUTTON_RIGHT)){
                    Vector2 delta = GetMouseDelta();
                    delta = Vector2Scale(delta,-1.0f / camera->zoom);
                    camera->target = Vector2Add(camera->target, delta);
                }

                // Zooming the camera
                float wheel = GetMouseWheelMove();
                if(wheel != 0){
                    Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(),*camera);
                    camera->offset = GetMousePosition();
                    camera->target = mouseWorldPos;
                    const float zoomIncrement = 0.125f;
                    camera->zoom += wheel * zoomIncrement;
                    if(camera->zoom < zoomIncrement){
                        camera->zoom = zoomIncrement;
                    }
                }

                // Selecting points
                if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                    Vector2 mousePos = GetScreenToWorld2D(GetMousePosition(),*camera);
                    Vector3 closestNode = maps::graph::SelectClosestNode(mousePos);
                    if(selectedPointsCount < 2){
                        maps::NodeAnim::AddNode(selectedPointsCount,Color{105,61,228,255},0.5f);
                        selectedPoints[selectedPointsCount] = closestNode;
                        selectedPointsCount++;
                        if(selectedPointsCount == 2){
                            maps::Dijkstra::init(maps::AdjList::capacity);
                            maps::Dijkstra::start(selectedPoints[0].z,closestNode.z);
                        }
                    } else{
                        selectedPointsCount = 0;
                        maps::Dijkstra::init(maps::AdjList::capacity); 
                    }
                }
            }
        }
    }
}