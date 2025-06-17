#ifndef PRZ_WIEL_H
#define PRZ_WIEL_H

#include <vector>

struct Point {
    int x;
    int y;
};

bool czy_wspoliniowe(Point a, Point b, Point c);
bool do_krawedzi(Point p, Point a, Point b);
bool przecina_polprosta(Point p, Point a, Point b);
bool punkt_w_wielokacie(Point p, const std::vector<Point>& wielokat);

#endif // PRZ_WIEL_H
