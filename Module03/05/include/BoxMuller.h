#ifndef BOXMULLER_H
#define BOXMULLER_H

#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <utility>
#include <algorithm>
#include <limits>
#include <random>

struct Points {
   public:
    Points(int count);
    double GetWeight(int from, int to);

   private:
    std::vector<std::pair<double, double>> points;
    double getDistance(std::pair<double, double> point1, std::pair<double, double> point2);
};

#endif
