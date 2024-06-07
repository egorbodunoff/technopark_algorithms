#ifndef LISTGRAPH_H
#define LISTGRAPH_H

#include <cassert>
#include <vector>

struct Edge {
    int to;
    int weight;

    Edge(int _to, int _weight) : to(_to), weight(_weight) {}
};

class ListGraph {
   public:
    ListGraph(int vertexCount);

    void AddEdge(int from, int to, int weight);
    int VerticesCount() const { return adjLists.size(); }
    int GetWeight(int from, int to) const;

    std::vector<Edge> GetNextVertices(int vertex) const;

   private:
    std::vector<std::vector<Edge>> adjLists;
};

#endif