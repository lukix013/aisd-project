#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

struct Point {
    int x;
    int y;
};

Point p0;

// Funkcja zwraca -1 jeśli p1 jest "mniejszy", 1 jeśli "większy", 0 jeśli współliniowy i bliższy
int det(Point p1, Point p2, Point p3) {
    return (p2.x - p1.x)*(p3.y - p1.y) - (p3.x - p1.x)*(p2.y - p1.y);
}

int distanceSq(Point p1, Point p2) {
    return (p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y);
}

bool comparePoints(Point p1, Point p2) {
    int orientation = det(p0, p1, p2);
    if (orientation == 0)
        return distanceSq(p0, p1) < distanceSq(p0, p2); // bliższy punkt pierwszy
    return (orientation > 0); // p1 bardziej na lewo niż p2
}

int findFirst(const std::vector<Point>& points) {
    int minIdx = 0;
    for (int i = 1; i < points.size(); ++i) {
        if (points[i].y < points[minIdx].y || (points[i].y == points[minIdx].y && points[i].x < points[minIdx].x)) {
            minIdx = i;
        }
    }
    return minIdx;
}

int main() {
    std::ifstream file("input2.txt");
    int x, y;
    std::vector<Point> points;

    while (file >> x >> y) {
        points.push_back({x, y});
    }
    file.close();

    if (points.size() < 3) {
        std::cout << "Nie można utworzyć otoczki wypukłej." << std::endl;
        return 1;
    }

    // Znajdź punkt startowy
    int first = findFirst(points);
    std::swap(points[0], points[first]);
    p0 = points[0];

    // Sortuj względem kąta
    std::sort(points.begin() + 1, points.end(), comparePoints);

    // Stos do otoczki wypukłej
    std::vector<Point> hull;
    hull.push_back(points[0]);
    hull.push_back(points[1]);
    hull.push_back(points[2]);

    for (int i = 3; i < points.size(); ++i) {
        while (hull.size() >= 2 &&
               det(hull[hull.size()-2], hull[hull.size()-1], points[i]) <= 0) {
            hull.pop_back(); // zakręt w prawo lub współliniowe
        }
        hull.push_back(points[i]);
    }

    std::cout << "Punkty na otoczce wypukłej:\n";
    for (auto p : hull) {
        std::cout << p.x << " " << p.y << std::endl;
    }

    return 0;
}
