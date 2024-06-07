#include "BoxMuller.cpp"
#include "TSP.cpp"


double approximation(double real, double alg) {
    if (alg == 0 || real == 0) {
        return 0;
    }
    return std::max(real / alg, alg / real);
}

double average(const std::vector<double>& values) {
    double valueSum = 0;
    for (auto value : values) {
        valueSum += value;
    }
    return valueSum / values.size();
}

std::pair<double, double> statistic(const std::vector<double>& values) {
    double avg = average(values);
    double dispersion = 0;
    for (double value : values) {
        dispersion += std::pow(value - avg, 2);
    }
    dispersion /= values.size();
    double std = std::sqrt(dispersion);
    return std::pair<double, double>(avg, std);
}

std::vector<double> experiment(int c) {
    int count = 10;
    std::vector<double> results;
    for (int i = 0; i < count; i++) {
        Points points(c);
        ListGraph graph(c);
        for (int from = 0; from < c; from++) {
            for (int to = from + 1; to < c; to++) {
                graph.AddEdge(from, to, points.GetWeight(from, to));
            }
        }
       
        TSP tsp;
        double TSP_MST = tsp.CalcMST(graph);
        double TSP_BF = tsp.Enumeration(graph);

        results.push_back(approximation(TSP_BF, TSP_MST));
    }
    return results;
}

int main() {
    for (int c = 2; c < 11; c++) {
        std::vector<double> result = experiment(c);
        std::pair<double, double> stats = statistic(result);

        std::cout << c << " среднее " << stats.first << " стандартное отклонение " << stats.second
                  << std::endl;
    }
    return 0;
}