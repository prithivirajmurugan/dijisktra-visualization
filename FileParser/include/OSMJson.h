#pragma once
#include "KeyTypes.h"
#include "nlohmann/json.hpp"
#include "raylib.h"
#include <map>
#include <vector>

namespace maps {
namespace fileparser {
extern nlohmann::json jsonData;
extern std::vector<Vector4> lines;
extern std::vector<maps::HighwayType> highwayTypes;
extern std::map<size_t, int> nodeMap;
extern std::vector<Vector2> nodes;

void ReadFile(const char *path);
void PrintData();
void AddNode(Vector2 node);
void ParseData();
Vector4 *GetBounds();
size_t GetHash(Vector2 node);
void StandardizeData();
void ScaleData(int scaleFactor);
} // namespace fileparser
} // namespace maps