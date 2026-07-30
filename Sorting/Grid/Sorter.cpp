#include <Grid/Sorter.h>
#include <Grid/Tile.h>
#include <queue>

std::vector<Tile> Sorter::SortTiles(std::vector<Tile>& tiles)
{
    std::vector<std::vector<int>> graph;
    std::vector<int> indegree;

    BuildGraph(tiles, graph, indegree);

    return PerformTopologicalSort(tiles, graph, indegree);
}

void Sorter::BuildGraph(std::vector<Tile>& tiles, std::vector<std::vector<int>>& graph, std::vector<int>& indegree)
{
    graph.resize(tiles.size());
    indegree.resize(tiles.size());

    for (int a = 0; a < tiles.size(); ++a) {
        for (int b = 0; b < tiles.size(); ++b) {
            if (a == b) {
                continue;
            }

            const auto& tileA = tiles[a];
            const auto& tileB = tiles[b];

            if (tileA.GetScreenRect().Overlaps(tileB.GetScreenRect()) == false) {
                continue;
            }

            /*
            we need to add edge only between those tiles which are not behind each other
            to prevent cycles. For example

            ^
            |
            |A
            | B
            ------->
            
            A is behind B - it's xMax is less than B's yMin. But B is also behind A
            - it's yMax is less than B's yMin
            */

            if (tileA.IsBehind(tileB) == true && tileB.IsBehind(tileA) == false) {
                graph[a].push_back(b);
                ++indegree[b];
            }
        }
    }
}

std::vector<Tile> Sorter::PerformTopologicalSort(std::vector<Tile>& tiles, std::vector<std::vector<int>>& graph, std::vector<int>& indegree)
{
    //using Khan algorithm

    std::vector<Tile> sorted;
    std::queue<int> bfs;

    for (int i = 0; i < indegree.size(); i++) {
        if (indegree[i] == 0) {
            bfs.push(i);
        }
    }

    std::vector<int> sortedInds;

    while (bfs.size() != 0) {
        int i = bfs.front();
        bfs.pop();

        sorted.push_back(std::move(tiles[i]));
        sortedInds.push_back(i);

        for (int a : graph[i]) {
            --indegree[a];

            if (indegree[a] == 0) {
                bfs.push(a);
            }
        }
    }

    if (sorted.size() < tiles.size()) {
        for (size_t i = 0; i < tiles.size(); ++i) {
            if (indegree[i] > 0) {
                sorted.push_back(std::move(tiles[i]));
            }
        }
    }

    return sorted;
}
