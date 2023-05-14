// parser.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

void loadFile(string path, char*& data) {
    streampos size;
    //char* data;
    ifstream caff(path, ios::in | ios::binary | ios::ate);
    if (caff.is_open()) {
        size = caff.tellg();
        data = new char[size];
        caff.seekg(0, ios::beg);
        caff.read(data, size);
        caff.close();
    }
    else {
        cout << "Unable to open file\n";
    }
}

void processData(char* data) {
    uint64_t length;
    switch (data[0]) {
        case 0x1:
            length = static_cast<uint64_t>(static_cast<unsigned char>(data[1]) |
                    static_cast<unsigned char>(data[2]) << 8 |
                    static_cast<unsigned char>(data[3]) << 16 |
                    static_cast<unsigned char>(data[4]) <<  24 |
                    static_cast<unsigned char>(data[5]) <<  32 |
                    static_cast<unsigned char>(data[6]) <<  40 |
                    static_cast<unsigned char>(data[7]) <<  48 |
                    static_cast<unsigned char>(data[8]) <<  56);
            cout << length;
            break;
        default: 
            cout << "Not found header!";
            exit(1);
    }
}

int main(int argc, char* argv[])
{
    string path = argv[0];
    path = "C:\\Users\\ticka\\Documents\\BME\\Szoftverbiztonsag\\caff_files\\1.caff";
    char* data;
    loadFile(path, data);
    if (data != NULL) {
        processData(data);
    }
    return 0;
}



// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
