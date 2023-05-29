#include <iostream>
#include <fstream>
#include <string>
#include "parse_caff.h"
#include "parse_ciff.h"

using namespace std;

//A kivalasztott fajl betoltese a memoriaba
void loadFile(string path, char*& data) {
    streampos size;                                         //A fajlmeret meghatarozasahoz hasznalt valtozo
    ifstream caff(path, ios::in | ios::binary | ios::ate);  //A bemenet a fajl beolvasasahoz
    if (caff.is_open()) {                                   //Csak akkor dolgozza fel a fajlt, ha a fajl meg lett nyitva
        size = caff.tellg();                                //A fajl vegen levo pozicio a fajl meretehez
        data = new char[size];                              //Fajl meretu karaktersorozat lefoglalasa a fajl beolvasasahoz
        caff.seekg(0, ios::beg);                            //A mutato visszaallitasa a fajl kezdetere
        caff.read(data, size);                              //Fajl beolvasasa a karatkersorozatba
        caff.close();                                       //Fajl bezarasa
    }
    else {
        cout << "Unable to open file\n";                    //Hibauzenet, ha nem nyilt meg a fajl
        std::exit(-1);                                      //-1-el valo kilepes, ha nem nyilt meg a fajl
    }
}

//A kapott utvonalbol a fajlnev megallapitasa
string filename(string name) {
    string file = name.substr(name.find_last_of("/\\") + 1);    //Az utolso \\ karakterek utani resz kiszedese
    string::size_type const p(file.find_last_of("."));          //Az utolso pont megtalalasa
    return file.substr(0, p);                                   //A levalasztott resz es az utolso pont kozotti karakterek visszaadasa
}

int main(int argc, char* argv[])
{
    if (argc != 3) {                                            //Nem megfelelo mennyisegu parameterszam eseten -1-el ter vissza.
        return -1;
    }
    bool caff;
    bool same = false;
    const char* first = "-caff";
    const char* second = "-ciff";
    for (int i = 0; argv[1][i] != '\0'; i++) {
        if (argv[1][i] == first[i]) {                           //-caff parameter ellenorzese
            same = true;
        }
        else {
            same = false;
            break;
        }
    }
    if (same) {
        caff = true;
    }
    else {
        same = false;
        for (int i = 0; argv[2][i] != '\0'; i++) {
            if (argv[2][i] == second[i]) {                      //-ciff parameter ellenorzese
                same = true;
            }
            else {
                same = false;
                break;
            }
        }
        if (same) {
            caff = false;
        }
        else {
            return -1;
        }
    }
    if (argv[2] == NULL) {                                      //Ha nincs megadott utvonal, akkor -1-el ter vissza
        return -1;
    }
    string path = argv[2];
    string fileName = filename(path);                           //A megadott utvonalbol a fajl nevenek kiszedese
    char* data;
    loadFile(path, data);                                       //Fajl tartalmanak betoltese a memoriaba
    int result = -1;                                            //Alapbol hibaval visszateres, siker eseten 0-ra feluliras
    if (data != NULL) {                                         //Sikeres beolvasas ellenorzese
        if (caff) {                                             //CAFF fajl feldolgozasa
            uint64_t length = determineLength(data);            //Header blokk hosszanak meghatarozasa
            result = processHeader(data, length);               //Header feldolgozasa
            uint64_t index = length + 9;                        //Az adaton vegigmeno index novelese
            uint64_t size = processCreator(data, index);        //Alkoto blokk feldolgozasa
            index += size;                                      //Az adaton vegigmeno index novelese
            size = processCIFF(data, index, fileName);          //CIFF blokk feldolgozasa
        }
        else {
            result = processCIFFData(data, 0, fileName);        //CIFF adatok feldolgozasa
        }
    }
    delete[] data;                                              //Adat memoriajanak felszabaditasa
    return result;                                              //Sikeres feldolgozas - 0 , sikertelen feldolgozas - -1
}