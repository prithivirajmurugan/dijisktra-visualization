#pragma once
#include "raylib.h"
#include "OSMJson.h"


namespace maps{
    namespace renderer {
        extern Vector3 selectedPoints[2];
        extern int selectedPointsCount;

        void ProcessInput(Camera2D *camera);
    }
}