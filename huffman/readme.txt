Program do kompresji i dekompresji plikow uzywajac kodowania huffmana. Mozna podac dane zarowno z pomoca argumentow, jak i bez.
Argi to <sciezka pliku wejscowego> <sciezka pliku wyjscowego> <tryb>. Tryb to albo 'd' dla dekompresji, albo 'k', dla kompresji.
Plikiem kompresowanym powinien byc .txt, a dekompresowanym .bin
(choc pomyslnie testowano to dla innych rozszerzen, optymalne dzialanie, ani uzytecznosc tych plikow nie sa wtedy gwarantowane)
Program dziala tylko miedzy plikami, sam ich nie wyswietla, ani nie pobiera danych bezposrednio.
Program moze nie byc optymalny dla bardzo malych/specyficznych (np. 1 wystapienie kazdego znaku) plikow.
Kodujac plik liczy ilosc wystapien znakow i buduje mape czestotliwosci, buduje drzewo huffmana.
Zapisujac najwierw wpisuje wielkosc mapy, a potem mape czestosci wystepowania znakow. A potem same dane. Dekodujac zczytuje mape,
liczy z jej pomoca calkowita ilosc danych, buduje drzewo huffmana z uzyciem tej mapy, a potem zczytuje i dekoduje dane, zapisujac je do nowego pliku.
