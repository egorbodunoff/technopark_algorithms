#include "IGraph.h"

class ArcGraph : public IGraph {
   public:
    ArcGraph(int vertexCount);
    ArcGraph(const IGraph &graph);

    virtual void AddEdge(int from, int to) override;
    virtual int VerticesCount() const override;
    virtual std::vector<int> GetNextVertices(int vertex) const override;
    virtual std::vector<int> GetPrevVertices(int vertex) const override;

   private:
    struct Edge {
        int from;
        int to;

        Edge(int _from, int _to) : from(_from), to(_to) {}
    };

    int verticesNumber;
    std::vector<Edge> edgesList;
};