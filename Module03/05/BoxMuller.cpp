#include "include/BoxMuller.h"

Points::Points(int count) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<> d(0, 1);

    for (int i = 0; i < count; i++) {
        double x = d(gen);
        double y = d(gen);
        points.emplace_back(std::make_pair(x, y));
    }
}

double Points::GetWeight(int from, int to) { return from == to ? 0 : getDistance(points[from], points[to]); }

double Points::getDistance(std::pair<double, double> point1, std::pair<double, double> point2) {
    return std::sqrt(std::pow(point1.first - point2.second, 2) + std::pow(point1.second - point2.second, 2));
}
