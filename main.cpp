#include <iostream>
#include "map.h"
#include "procedmapgen.h"
// #include "filemaploader.h" // Decomentează dacă vrei să testezi încărcarea din fișier

using namespace std;

int main() {
    // 1. Configurare parametri de test
    int rows = 15;      // Înălțimea hărții
    int cols = 20;      // Lățimea hărții
    int stations = 3;   // Număr stații de încărcare (S)
    int clients = 5;    // Număr clienți (D)

    cout << "=== Start Test Harta HiveMind ===" << endl;

    // 2. Creăm obiectul Harta (gol momentan)
    Map cityMap(rows, cols);

    // 3. Alegem Strategia: Generare Procedurală (Random)
    // 0.2f înseamnă densitate de ziduri 20%
    ProceduralMapGenerator* pGen = new ProceduralMapGenerator(0.2f);
    
    // 4. Încărcăm harta (asta apelează intern generate + isValid)
    // Dacă prima generare e invalidă, va încerca din nou automat
    cout << "[INFO] Se genereaza si valideaza harta..." << endl;
    cityMap.loadMap(pGen, stations, clients);

    // 5. Afișăm harta în consolă
    cityMap.printMap();

    // 6. Curățăm memoria
    delete pGen;

    cout << "=== Test Finalizat ===" << endl;
    
    // Ținem consola deschisă ca să poți vedea rezultatul
    cout << "Scrie orice litera si apasa Enter pentru a inchide: ";
    char temp;
    cin >> temp;
    
    return 0;
}