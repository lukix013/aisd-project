#include "wyp.h"
#include <algorithm>
#include <cmath>

Point p0; // zmienna globalna - pierwszy punkt 

int det(Point p1, Point p2, Point p3) { // liczenie współrzędnej kątowej
    return (p2.x - p1.x)*(p3.y - p1.y) - (p3.x - p1.x)*(p2.y - p1.y); 
}

int distanceSq(Point p1, Point p2) { // jeżeli punkty są współlimiowe liczymy odległość
    return (p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y);
}

bool comparePoints(Point p1, Point p2) { // własna funkcja do sortowania 
    int orientation = det(p0, p1, p2); // orientation > 0 - mniejsza współrzędna, orientation < 0 - większa współrzędna, oeirntation == 0 - punkty współliniowe
    if (orientation == 0)
        return distanceSq(p0, p1) < distanceSq(p0, p2); // jeżeli współliniowe sortujemy po odległośći
    return (orientation > 0);
}

int findFirst(const std::vector<Point>& points) { // wyszukiwanie p0 (najniższy lewy punkt)
    int minIdx = 0;
    for (int i = 1; i < points.size(); ++i) {
        if (points[i].y < points[minIdx].y || 
           (points[i].y == points[minIdx].y && points[i].x < points[minIdx].x)) {
            minIdx = i;
        }
    }
    return minIdx;
}

std::vector<Point> convexHull(std::vector<Point> points) {
    if (points.size() < 3) {
        return {};
    }

    // znalezienie punktu początkowego
    int first = findFirst(points);
    std::swap(points[0], points[first]);
    p0 = points[0];

    // sortowanie względem kąta od p0
    std::sort(points.begin() + 1, points.end(), comparePoints);

    // inicjalizacja otoczki pierwszymi trzema punktami
    std::vector<Point> hull;
    hull.push_back(points[0]);
    hull.push_back(points[1]);
    hull.push_back(points[2]);

    // Dodawanie kolejnych punktów, usuwanie jeśli robią prawoskręt
    for (int i = 3; i < points.size(); ++i) {
        while (hull.size() >= 2 &&
               det(hull[hull.size()-2], hull[hull.size()-1], points[i]) <= 0) {
            hull.pop_back(); // jeżeli ostatnie dwa punkty i bieżący tworzą prawoskręt lub są współliniowe, usuwamy ostatni punkt z otoczki
        }
        hull.push_back(points[i]);
    }

    return hull;
}
