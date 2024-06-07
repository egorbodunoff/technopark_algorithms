#include "IGraph.h"
#include <unordered_set>

class SetGraph : public IGraph {
   public:
    SetGraph(int vertexCount);
    SetGraph(const IGraph &graph);

    virtual void AddEdge(int from, int to) override;
    virtual int VerticesCount() const override;
    virtual std::vector<int> GetNextVertices(int vertex) const override;
    virtual std::vector<int> GetPrevVertices(int vertex) const override;

   private:
    std::vector<std::unordered_set<int>> adjLists;
    std::vector<std::unordered_set<int>> prevAdjLists;
};