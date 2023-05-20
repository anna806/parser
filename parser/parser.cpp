// parser.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include <encode.h>
#include <decode.h>
#include <types.h>
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

uint64_t determineLength(char* data) {
    uint64_t length = 0x0;
    if (data[0] == 0x1) {
        for (int i = 1; i < 9; i++) {
            length |= static_cast<uint64_t>(static_cast<unsigned char>(data[i]) << (i - 1) * 8);
        }
        /*length = static_cast<uint64_t>(static_cast<unsigned char>(data[1]) |
            static_cast<unsigned char>(data[2]) << 8 |
            static_cast<unsigned char>(data[3]) << 16 |
            static_cast<unsigned char>(data[4]) << 24 |
            static_cast<unsigned char>(data[5]) << 32 |
            static_cast<unsigned char>(data[6]) << 40 |
            static_cast<unsigned char>(data[7]) << 48 |
            static_cast<unsigned char>(data[8]) << 56);*/
        cout << "Size of the header: ";
        cout << length;
        cout << "\n";
    }
    else {
        cout << "Header not found!\n";
        exit(1);
    }
    return length;

}

void processData(char* data, uint64_t length) {
    if (length == 0) {
        cout << "Given 0 length!\n";
        return;
    }
    switch (data[0]) {
    case 0x1: {

        for (int i = 9; i < 13; i++) {
            cout << data[i];
        }
        cout << "\n";
        uint64_t headerSize = 0x0;
        int index = 0;
        for (int i = 13; i < 21; i++) {
            headerSize |= static_cast<int>(static_cast<unsigned char>(data[i]) << index * 8);
            index++;
        }
        cout << headerSize;
        cout << "\n";
        if (length != headerSize) {
            return;
        }
        uint64_t num_anim = 0x0;
        index = 0;
        for (int i = 21; i < 29; i++) {
            num_anim |= static_cast<int>(static_cast<unsigned char>(data[i]) << index * 8);
            index++;
        }
        cout << num_anim;
        break;
    }
    case 0x2: {

        break;
    }
    case 0x3: {

        break;
    }
    default: {
        cout << "Not correct block type!";
        exit(1);
    }
    }
}

uint64_t processHeader(char* data, uint64_t length) {
    if (data[0] != 0x1) {
        return 0x0;
    }
    cout << "Magic ";
    for (int i = 9; i < 13; i++) {
        cout << data[i];
    }
    cout << "\n";
    uint64_t headerSize = 0x0;
    int index = 0;
    for (int i = 13; i < 21; i++) {
        headerSize |= static_cast<int>(static_cast<unsigned char>(data[i]) << index * 8);
        index++;
    }
    cout << "Size of the header: ";
    cout << headerSize;
    cout << "\n";
    if (length != headerSize) {
        return 0x0;
    }
    uint64_t num_anim = 0x0;
    index = 0;
    for (int i = 21; i < 29; i++) {
        num_anim |= static_cast<int>(static_cast<unsigned char>(data[i]) << index * 8);
        index++;
    }
    cout << "Number of animations: ";
    cout << num_anim;
    cout << "\n";
    return num_anim;
}

uint64_t processCreator(char* data, uint64_t index) {
    if (data[index] != 0x2) {
        cout << "Creator not found!";
        return 0x0;
    }
    uint64_t length = 0x0;
    int num = 0;
    for (uint64_t i = index + 1; i < index + 9; i++) {
        length |= static_cast<uint64_t>(static_cast<unsigned char>(data[i]) << num * 8);
        num++;
    }
    cout << "Creator length: ";
    cout << length;
    cout << "\n";
    index += 9;
    uint64_t year = static_cast<uint64_t>(static_cast<unsigned char>(data[index]) |
        static_cast<unsigned char>(data[index + 1]) << 8);
    uint64_t month = static_cast<uint64_t>(static_cast<unsigned char>(data[index + 2]));
    uint64_t day = static_cast<uint64_t>(static_cast<unsigned char>(data[index + 3]));
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
    uint64_t creatorLength = 0x0;
    num = 0;
    for (uint64_t i = index; i < index + 8; i++) {
        creatorLength |= static_cast<uint64_t>(static_cast<unsigned char>(data[i]) << num * 8);
        num++;
    }
    if (creatorLength == 0x0) {
        cout << "There is no creator name given.\n";
    }
    else {
        cout << "Length of creator string: ";
        cout << creatorLength;
        cout << "\n";
        index += 8;
        char* creator = new char[creatorLength + 1];
        for (uint64_t i = 0; i < creatorLength; i++) {
            creator[i] = data[index + i];
        }
        creator[creatorLength] = '\0';
        cout << "Name of creator: ";
        cout << creator;
        cout << "\n";
    }
    return length + uint64_t(9);
}

void processCIFFData(char* data, uint64_t index) {
    cout << "Magic ";
    for (uint64_t i = index; i < index + 4; i++) {
        cout << data[i];
    }
    cout << "\n";
    index += 4;
    uint64_t headerSize = 0x0;
    int num = 0;
    for (uint64_t i = index; i < index + 8; i++) {
        headerSize |= static_cast<uint64_t>(static_cast<unsigned char>(data[i]) << num * 8);
        num++;
    }
    cout << "Header size : " << headerSize << endl;
    index += 8;
    uint64_t contentSize = 0x0;
    num = 0;
    for (uint64_t i = index; i < index + 8; i++) {
        contentSize |= static_cast<uint64_t>(static_cast<unsigned char>(data[i]) << num * 8);
        num++;
    }
    cout << "Content size : " << contentSize << endl;
    index += 8;
    uint64_t width = 0x0;
    num = 0;
    for (uint64_t i = index; i < index + 8; i++) {
        width |= static_cast<uint64_t>(static_cast<unsigned char>(data[i]) << num * 8);
        num++;
    }
    cout << "Width: " << width << endl;
    index += 8;
    uint64_t height = 0x0;
    num = 0;
    for (uint64_t i = index; i < index + 8; i++) {
        height |= static_cast<uint64_t>(static_cast<unsigned char>(data[i]) << num * 8);
        num++;
    }
    cout << "Height: " << height << endl;
    index += 8;
    if (contentSize != width * height * 3) {
        cout << "Content condition not met!" << endl;
        return;
    }
    num = 0;
    for (uint64_t i = index; data[i] != '\n'; i++) {
        cout << data[i];
        num++;
    }
    cout << "\n";
    index += num + 1;
    uint64_t remaining = headerSize - 36 - num - 1;
    uint64_t max = index + remaining;
    cout << "Tags:" << endl;
    while (index < max) {
        num = 0;
        cout << "\t";
        for (uint64_t i = index; data[i] != '\0'; i++) {
            cout << data[i];
            num++;
        }
        cout << endl;
        index += num + 1;
    }
    uint8_t* pixels = new uint8_t[contentSize];
    uint64_t pixelEnd = index + contentSize;
    num = 0;
    for (uint64_t i = index; i < pixelEnd; i++) {
        pixels[num] = static_cast<uint8_t>(static_cast<unsigned char>(data[i]));
        num++;
    }
    uint8_t** output = new uint8_t*;
    size_t webp = WebPEncodeRGB(pixels, width, height, width * 3, 100, output);
    WebPFree(*output);
}

uint64_t processCIFF(char* data, uint64_t index) {
    if (data[index] != 0x3) {
        cout << "Animation not found!";
        return 0x0;;
    }
    uint64_t length = 0x0;
    int num = 0;
    for (uint64_t i = index + 1; i < index + 9; i++) {
        length |= static_cast<uint64_t>(static_cast<unsigned char>(data[i]) << num * 8);
        num++;
    }
    cout << "Animation length: ";
    cout << length;
    cout << "\n";
    index += 9;
    uint64_t anim_dur = 0x0;
    num = 0;
    for (uint64_t i = index; i < index + 8; i++) {
        anim_dur |= static_cast<uint64_t>(static_cast<unsigned char>(data[i]) << num * 8);
        num++;
    }
    cout << "Duration of the animation: ";
    cout << anim_dur;
    cout << " ms\n";
    index += 8;
    processCIFFData(data, index);
    return length + uint64_t(9);
}

int main(int argc, char* argv[])
{
    string path = argv[0];
    path = "C:\\Users\\ticka\\Documents\\BME\\Szoftverbiztonsag\\caff_files\\1.caff";
    char* data;
    loadFile(path, data);
    if (data != NULL) {
        uint64_t length = determineLength(data);
        uint64_t num_anim = processHeader(data, length);
        uint64_t index = length + 9;
        uint64_t size = processCreator(data, index);
        index += size;
        for (int i = 0; i < num_anim; i++) {
            size = processCIFF(data, index);
            index += size;
        }
    }
    delete[] data;
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
