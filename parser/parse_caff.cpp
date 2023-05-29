#include <iostream>
#include <fstream>
#include <string>

using namespace std;

//8 bajtos integer beolvasasa az adatbol
uint64_t getInteger(char* data, uint64_t index) {
    uint64_t Integer = 0x0;                                                                 //A fuggveny egy for ciklusban
    int num = 0;                                                                            //vegighalad a 8 karakteren es
    for (uint64_t i = index; i < index + 8; i++) {                                          //mindegyiket egy megadott ertekkel
        Integer |= static_cast<uint64_t>(static_cast<unsigned char>(data[i]) << num * 8);   //balra shiftelve beleteszi az elso
        num++;                                                                              //sorban letrehozott valtozoba,
    }                                                                                       //amelyet visszaad.
    return Integer;
}

//Header blokk hosszanak meghatarozasa
uint64_t determineLength(char* data) {
    uint64_t length = 0x0;                  //A fuggveny ellenorzi a blokk elejen levo szamot,
    if (data[0] == 0x1) {                   //mely jelzi a header blokkot. Ha nem megfelelot talal,
        length = getInteger(data, 1);       //akkor -1-el ter vissza. A getInteger fuggvennyel 
        cout << "Size of the header: ";     //meghatarozza a kovetkezo 8 karakterben tarolt hosszat.
        cout << length;                     //Visszateresi erteke a header hossza, amit az index 
        cout << "\n";                       //tovabbleptetesere hasznal a program.
    }
    else {
        cout << "Header not found!\n";
        std::exit(-1);
    }
    return length;
}

//Header blokk feldolgozasa
int processHeader(char* data, uint64_t length) {
    if (data[0] != 0x1) {                   //A header blokkot jelzo szam ellenorzese
        return -1;                          //Ha nem jot talal, akkor -1-el ter vissza
    }
    const char* magic = "CAFF";             //A fuggveny ellenorzi, hogy a kovetkezo negy 
    int index = 0;                          //karakterben az elore meghatarozott CAFF 
    for (int i = 9; i < 13; i++) {          //karakterlanc talalhato-e. Ha barmelyik karakter 
        if (data[i] != magic[index]) {      //nem egyezik, akkor -1-el ter vissza.
            return -1;
        }
        else {
            index++;
        }
    }
    uint64_t headerSize = 0x0;              //A header meretenek meghatarozasa a kovetkezo 
    headerSize = getInteger(data, 13);      //8 karakterbol.
    cout << "Size of the header: ";
    cout << headerSize;
    cout << "\n";
    if (length != headerSize) {             //Ha a parameterben kapott hossz nem egyezik a 
        return -1;                          //meghatarozottal, akkor -1-el ter vissza.
    }
    uint64_t num_anim = 0x0;
    num_anim = getInteger(data, 21);        //Animaciok szamanak meghatarozasa a getInteger 
    cout << "Number of animations: ";       //fuggvennyel.
    cout << num_anim;
    cout << "\n";
    return 0;
}

//Alkoto blokk feldolgozasa
uint64_t processCreator(char* data, uint64_t index) {
    if (data[index] != 0x2) {               //A fuggveny ellenorzi az alkoto blokkot jelzo
        cout << "Creator not found!";       //karaktert. Ha nincs egyezes, akkor -1-el ter vissza.
        return 0x0;
    }
    index++;
    uint64_t length = 0x0;
    length = getInteger(data, index);       //Az alkoto blokk hosszanak meghatarozasa
    cout << "Creator length: ";             //a getInteger fuggveny segitsegevel.
    cout << length;
    cout << "\n";
    index += 8;
    uint64_t year = static_cast<uint64_t>(static_cast<unsigned char>(data[index]) |         //A ket bajtos integer meghatarozasa
        static_cast<unsigned char>(data[index + 1]) << 8);                                  //shiftelessel
    uint64_t month = static_cast<uint64_t>(static_cast<unsigned char>(data[index + 2]));    //Az egy bajtos integerek
    uint64_t day = static_cast<uint64_t>(static_cast<unsigned char>(data[index + 3]));      //meghatarozasa castolassal
    uint64_t hour = static_cast<uint64_t>(static_cast<unsigned char>(data[index + 4]));
    uint64_t minute = static_cast<uint64_t>(static_cast<unsigned char>(data[index + 5]));
    cout << year;
    cout << "-";
    cout << month;
    cout << "-";
    cout << day;
    cout << " ";
    cout << hour;
    cout << ":";
    cout << minute;
    cout << "\n";
    index += 6;
    uint64_t creatorLength = 0x0;                       //Az alkotonev hosszanak meghatarozasa a
    creatorLength = getInteger(data, index);            //getInteger fuggvennyel.
    if (creatorLength == 0x0) {                         
        cout << "There is no creator name given.\n";
    }
    else {
        cout << "Length of creator string: ";           //Az alkoto nevenek feldolgozasa csak akkor, 
        cout << creatorLength;                          //ha a hossza nagyobb, mint 0.
        cout << "\n";
        index += 8;
        char* creator = new char[creatorLength + 1];    //A meghatarozott hossz nagysagu karakterlanc lefoglalasa
        for (uint64_t i = 0; i < creatorLength; i++) {
            creator[i] = data[index + i];               //A karakterek kiolvasasa a lefoglalt karakterlancba
        }
        creator[creatorLength] = '\0';
        cout << "Name of creator: ";
        cout << creator;
        cout << "\n";
        delete[] creator;                               //A lefoglalt karakterlanc felszabaditasa
    }
    return length + uint64_t(9);                        //Visszateres a blokk hosszaval, ezzel az ertekkel
}                                                       //mozgatja arebb a program a mutatot.