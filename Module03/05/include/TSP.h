#ifndef TSP_H
#define TSP_H

#include <iostream>
#include <set>
#include "../ListGraph.cpp"

class TSP {
   public:
    int CalcMST(const ListGraph& graph);
    int Enumeration(const ListGraph& graph);

   private:
    int calculatePathWeight(const ListGraph& graph, const std::vector<int>& path);
};

#endif