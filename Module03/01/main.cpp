#include <iostream>
#include <queue>
#include <cassert>

#include "ArcGraph.cpp"
#include "ListGraph.cpp"
#include "MatrixGraph.cpp"
#include "SetGraph.cpp"

void BFS(const IGraph &graph, int vertex, std::function<void(int)> visit) {
    std::vector<bool> visited(graph.VerticesCount(), false);
    std::queue<int> bfsQ;
    bfsQ.push(vertex);
    visited[vertex] = true;

    while (bfsQ.size() > 0) {
        int current = bfsQ.front();
        bfsQ.pop();
        visit(current);

        std::vector<int> adjList = graph.GetNextVertices(current);
        for (int i = 0; i < adjList.size(); ++i) {
            if (!visited[adjList[i]]) {
                bfsQ.push(adjList[i]);
                visited[adjList[i]] = true;
            }
        }
    }
}

int main() {
    MatrixGraph matrixGraph(8);
    matrixGraph.AddEdge(0, 1);
    matrixGraph.AddEdge(0, 4);
    matrixGraph.AddEdge(1, 2);
    matrixGraph.AddEdge(1, 3);
    matrixGraph.AddEdge(2, 5);
    matrixGraph.AddEdge(3, 0);
    matrixGraph.AddEdge(3, 4);
    matrixGraph.AddEdge(3, 2);
    matrixGraph.AddEdge(4, 6);
    matrixGraph.AddEdge(5, 3);
    matrixGraph.AddEdge(6, 3);
    matrixGraph.AddEdge(6, 5);
    matrixGraph.AddEdge(6, 7);
    matrixGraph.AddEdge(7, 5);

    SetGraph setGraph(matrixGraph);
    ArcGraph arcGraph(matrixGraph);
    ListGraph listGraph(matrixGraph);

    assert(setGraph.VerticesCount() == listGraph.VerticesCount()); 
    assert(setGraph.VerticesCount() == arcGraph.VerticesCount()); 
    assert(setGraph.VerticesCount() == matrixGraph.VerticesCount()); 
    assert(arcGraph.VerticesCount() == listGraph.VerticesCount()); 
    std::cout << "Vertices count is equal for all graphs\n" << std::endl; 

    std::vector<int> listGraphBFS; 
    BFS(listGraph, 0, [&listGraphBFS](int vertex) { listGraphBFS.push_back(vertex); }); 

    std::vector<int> setGraphBFS; 
    BFS(listGraph, 0, [&setGraphBFS](int vertex) { setGraphBFS.push_back(vertex); }); 

    std::vector<int> matrixGraphBFS; 
    BFS(listGraph, 0, [&matrixGraphBFS](int vertex) { matrixGraphBFS.push_back(vertex); }); 

    std::vector<int> arcGraphBFS; 
    BFS(listGraph, 0, [&arcGraphBFS](int vertex) { arcGraphBFS.push_back(vertex); }); 

    for (int i = 0; i < listGraphBFS.size(); ++i) {
        assert(listGraphBFS[i] == matrixGraphBFS[i]); 
        assert(setGraphBFS[i] == arcGraphBFS[i]); 
        assert(setGraphBFS[i] == matrixGraphBFS[i]);
    }
    std::cout << "BFS traversals are equal for all graphs\n" << std::endl; 

    BFS(listGraph, 0, [](int vertex) { std::cout << vertex << " "; });
    std::cout << std::endl;
    BFS(matrixGraph, 0, [](int vertex) { std::cout << vertex << " "; });
    std::cout << std::endl;
    BFS(setGraph, 0, [](int vertex) { std::cout << vertex << " "; });
    std::cout << std::endl;
    BFS(arcGraph, 0, [](int vertex) { std::cout << vertex << " "; });
    std::cout << std::endl;
    return 0;
}