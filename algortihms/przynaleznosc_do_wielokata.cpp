#include <iostream>
#include <vector>
#include <algorithm>

struct Point {
    int x;
    int y;
};

bool czy_wspoliniowe(Point a, Point b, Point c) {
    return (b.x - a.x) * (c.y - a.y) == (b.y - a.y) * (c.x - a.x);
}

bool do_krawedzi(Point p, Point a, Point b) {
    if (czy_wspoliniowe(p, a, b)) {
        if (std::min(a.x, b.x) <= p.x && p.x <= std::max(a.x, b.x) &&
            std::min(a.y, b.y) <= p.y && p.y <= std::max(a.y, b.y)) {
            return true;
        }
    }
    return false;
}

bool przecina_polprosta(Point p, Point a, Point b) {
    if (a.y > b.y) std::swap(a, b);

    if (p.y == a.y || p.y == b.y)
        p.y += 1;

    if (p.y < a.y || p.y > b.y) return false;

    double x_skrzyzowania = a.x + (double)(b.x - a.x) * (p.y - a.y) / (b.y - a.y);

    return x_skrzyzowania > p.x;
}

bool punkt_w_wielokacie(Point p, const std::vector<Point>& wielokat) {
    int n = wielokat.size();
    int przeciecia = 0;

    for (int i = 0; i < n; ++i) {
        Point a = wielokat[i];
        Point b = wielokat[(i + 1) % n];

        if (do_krawedzi(p, a, b)) return 1;

        if (przecina_polprosta(p, a, b))
            przeciecia++;
    }

    return (przeciecia % 2 == 1) ? 1 : 0;
}

int main() {
    std::vector<Point> wielokat = {{0, 0}, {5, 0}, {5, 5}, {0, 5}};
    Point p = {3, 3};

    std::cout << punkt_w_wielokacie(p, wielokat) << "\n";

    return 0;
}
