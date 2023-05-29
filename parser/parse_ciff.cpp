#include <iostream>
#include <fstream>
#include <string>
#include "webp/encode.h"
#include "parse_caff.h"

using namespace std;

//CIFF adatok feldolgozasa
int processCIFFData(char* data, uint64_t index, string fileName) {
    const char* magic = "CIFF";                             //A fuggveny ellenorzi a blokk elejen 
    int id = 0;                                             //talalhato, 4 hosszu, elore megadott CIFF karaktersorozatot.
    for (uint64_t i = index; i < index + 4; i++) {          //Ha barmilyen kulonbseget talal, akkor
        if (data[i] != magic[id]) {                         //-1-el ter vissza.
            return -1;
        }
        else {
            id++;
        }
    }
    index += 4;
    uint64_t headerSize = 0x0;
    headerSize = getInteger(data, index);                   //A header hosszanak meghatarozasa a 
    cout << "Header size : " << headerSize << endl;         //getInteger fuggveny segitsegevel.
    index += 8;
    uint64_t contentSize = 0x0;
    contentSize = getInteger(data, index);                  //A tartalom hosszanak meghatarozasa
    cout << "Content size : " << contentSize << endl;       //a getInteger fuggveny segitsegevel.
    index += 8;
    uint64_t width = 0x0;
    width = getInteger(data, index);                        //A kep szelessegenek meghatarozasa
    cout << "Width: " << width << endl;                     //a getInteger fuggveny segitsegevel.
    index += 8;
    uint64_t height = 0x0;      
    height = getInteger(data, index);                       //A kep magassaganak meghatarozasa
    cout << "Height: " << height << endl;                   //a getInteger fuggveny segitsegevel.
    index += 8;
    if (contentSize != width * height * 3) {                //A fuggveny ellenorzi, hogy a tartalom
        cout << "Content condition not met!" << endl;       //hossza egyenlo a szelesseg * magassag * 3
        return -1;                                          //ertekevel. Ha nem igy van, akkor -1-el ter vissza.
    }
    int num = 0;
    for (uint64_t i = index; data[i] != '\n'; i++) {
        cout << data[i];                                    //A kep cimenek kiszedese az adatbol
        num++;
    }
    cout << "\n";
    index += num + 1;
    uint64_t remaining = headerSize - 36 - num - 1;         //A header fennmarado reszenek kiszamolasa
    uint64_t max = index + remaining;                       //A header utolso indexenek kiszamitasa
    cout << "Tags:" << endl;
    while (index < max) {                                   //A ciklus megkeresi az osszes eltarolt 
        num = 0;                                            //taget a kephez. Vegigmegy a header fennmarado
        cout << "\t";                                       //reszen es az osszes \0 karakterrel vegzodo
        for (uint64_t i = index; data[i] != '\0'; i++) {    //karaktersorozatot kiszedi.
            cout << data[i];
            num++;
        }
        cout << endl;
        index += num + 1;
    }
    uint8_t* pixels = new uint8_t[contentSize];             //A kep meretenek megfelelo nagysagu tomb lefoglalasa
    uint64_t pixelEnd = index + contentSize;                //Az utolso pixel helyenek meghatarozasa
    num = 0;
    for (uint64_t i = index; i < pixelEnd; i++) {
        pixels[num] = static_cast<uint8_t>(static_cast<unsigned char>(data[i]));    //A pixel tomb feltoltese az adatban tarolt
        num++;                                                                      //karakterek castolasaval.
    }
    uint8_t* output;
    size_t webp = WebPEncodeRGB(pixels, width, height, width * 3, 100, &output);    //A pixelekbol webp formatum eloallitasa
    for (size_t i = 0; i < webp; i++) {
        output[i] = (static_cast<unsigned char>(output[i]));                        //Az eloallitott webp atalakitasa byte-okka
    }
    delete[] pixels;                                        //A pixeleket tarolo tomb felszabaditasa
    ofstream image(fileName + ".webp");                     //A kiirando fajl letrehozasa es a kimenet megnyitasa
    if (image.is_open()) {                                  //Csak akkor irja ki az adatot, ha megnyilt a fajl
        for (size_t i = 0; i < webp; i++) {
            image << output[i];                             //Az adat kiirasa a fajlba
        }
        image.close();                                      //A fajl bezarasa
    }
    else {
        WebPFree(output);                                   //A tarolt webp felszabaditasa
        return -1;                                          //Sikertelen fajlmegnyitas eseten -1-el ter vissza
    }
    WebPFree(output);                                       //A tarolt webp felszabaditasa
    return 0;
}

//CIFF blokk feldolgozasa
int processCIFF(char* data, uint64_t index, string fileName) {
    if (data[index] != 0x3) {                               //A CIFF blokkot jelzo szam ellenorzese
        cout << "Animation not found!";                     //Ha nem talal egyezest -1-el ter vissza
        return -1;
    }
    index++;
    uint64_t length = 0x0;
    length = getInteger(data, index);                       //A blokk hosszanak meghatarozasa
    cout << "Animation length: ";                           //a getInteger fuggveny segitsegevel.
    cout << length;
    cout << "\n";
    index += 8;
    uint64_t anim_dur = 0x0;
    anim_dur = getInteger(data, index);                     //Az animacio hosszanak meghatarozasa
    cout << "Duration of the animation: ";                  //a getInteger fuggveny segitsegevel.
    cout << anim_dur;
    cout << " ms\n";
    index += 8;
    int result = processCIFFData(data, index, fileName);    //A CIFF adatok feldolgozasa a processCIFFData
    return result;                                          //fuggveny segitsegevel.
}

