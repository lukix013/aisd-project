// wyp.cpp
#include "wyp.h"
#include <algorithm>
#include <cmath>

Point p0;

int det(Point p1, Point p2, Point p3) {
    return (p2.x - p1.x)*(p3.y - p1.y) - (p3.x - p1.x)*(p2.y - p1.y);
}

int distanceSq(Point p1, Point p2) {
    return (p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y);
}

bool comparePoints(Point p1, Point p2) {
    int orientation = det(p0, p1, p2);
    if (orientation == 0)
        return distanceSq(p0, p1) < distanceSq(p0, p2);
    return (orientation > 0);
}

int findFirst(const std::vector<Point>& points) {
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

    int first = findFirst(points);
    std::swap(points[0], points[first]);
    p0 = points[0];

    std::sort(points.begin() + 1, points.end(), comparePoints);

    std::vector<Point> hull;
    hull.push_back(points[0]);
    hull.push_back(points[1]);
    hull.push_back(points[2]);

    for (int i = 3; i < points.size(); ++i) {
        while (hull.size() >= 2 &&
               det(hull[hull.size()-2], hull[hull.size()-1], points[i]) <= 0) {
            hull.pop_back();
        }
        hull.push_back(points[i]);
    }

    return hull;
}
