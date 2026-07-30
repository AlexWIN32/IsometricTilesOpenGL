#pragma once
#include <vector>

class Tile;

class Sorter
{
public:
    static std::vector<Tile> SortTiles(std::vector<Tile>& tiles);
private:
    static void BuildGraph(std::vector<Tile>& tiles, std::vector<std::vector<int>>& graph, std::vector<int>& indegree);
    static std::vector<Tile> PerformTopologicalSort(std::vector<Tile>& tiles, std::vector<std::vector<int>>& graph, std::vector<int>& indegree);
};
