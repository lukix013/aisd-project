#include "prz_wiel.h"
#include <algorithm>

bool czy_wspoliniowe(Point a, Point b, Point c) {
    return (b.x - a.x) * (c.y - a.y) == (b.y - a.y) * (c.x - a.x);
} // sprawdza czy 3 punkty są współliniowe

bool do_krawedzi(Point p, Point a, Point b) {
    if (czy_wspoliniowe(p, a, b)) { // sprawdza wspołliniowość punktu z krawędzią
        if (std::min(a.x, b.x) <= p.x && p.x <= std::max(a.x, b.x) &&
            std::min(a.y, b.y) <= p.y && p.y <= std::max(a.y, b.y)) { // czy punkt leży w zakresie współrzędnych końców odcinka
            return true; // jeśli tak punkt leży dokładnie na krawędzi
        }
    }
    return false; // jeśli nie, zwraca fałsz
}

bool przecina_polprosta(Point p, Point a, Point b) {
    if (a.y > b.y) std::swap(a, b); // upewniamy się, że a.y <= b.y

    // Sprawdzenie, czy półprosta y = p.y przecina przedział [a.y, b.y)
    if (p.y <= a.y || p.y > b.y) return false;

    // Ustal, po której stronie względem p.x znajduje się przecięcie
    long long det = 1LL * (b.x - a.x) * (p.y - a.y) - 1LL * (p.x - a.x) * (b.y - a.y);

    return det > 0; // jeśli det > 0, przecięcie jest na prawo od punktu p (czyli półprosta przecina krawędź)
}


bool punkt_w_wielokacie(Point p, const std::vector<Point>& wielokat) {
    int n = wielokat.size();
    int przeciecia = 0; // liczba przeciec

    for (int i = 0; i < n; ++i) {
        Point a = wielokat[i];
        Point b = wielokat[(i + 1) % n]; // iterujemy po wszystkich bokach wielokąta

        if (do_krawedzi(p, a, b)) return 1; // jeżeli punkt na krawędzi to należy do wielokąta

        if (przecina_polprosta(p, a, b))
            przeciecia++; // jeżeli półprosta przecina to dodajemy liczbę przecięć
    }

    return (przeciecia % 2 == 1); // jeżeli nieparzysta liczba przecięć to punkt leży w wielokącie
}
