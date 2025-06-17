// wyp.h
#ifndef WYP_H
#define WYP_H

#include <vector>
#include "prz_wiel.h"

extern Point p0;

int det(Point p1, Point p2, Point p3);
int distanceSq(Point p1, Point p2);
bool comparePoints(Point p1, Point p2);
int findFirst(const std::vector<Point>& points);
std::vector<Point> convexHull(std::vector<Point> points);

#endif // WYP_H
