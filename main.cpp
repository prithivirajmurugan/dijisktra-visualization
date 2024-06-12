#include "Renderer.h"
#include "OSMJson.h"

int main(){
    maps::renderer::init(1080,720, "Dijikstra Pathfinding Visualizer");
    maps::renderer::run();
    return 0;
}