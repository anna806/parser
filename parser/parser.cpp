#include <iostream>
#include <fstream>
#include <string>
#include "webp/encode.h"

using namespace std;

void loadFile(string path, char*& data) {
    streampos size;
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

uint64_t getInteger(char* data, uint64_t index) {
    uint64_t Integer = 0x0;
    int num = 0;
    for (uint64_t i = index; i < index + 8; i++) {
        Integer |= static_cast<uint64_t>(static_cast<unsigned char>(data[i]) << num * 8);
        num++;
    }
    return Integer;
}

uint64_t determineLength(char* data) {
    uint64_t length = 0x0;
    if (data[0] == 0x1) {
        length = getInteger(data, 1);
        cout << "Size of the header: ";
        cout << length;
        cout << "\n";
    }
    else {
        cout << "Header not found!\n";
        std::exit(1);
    }
    return length;

}

void processHeader(char* data, uint64_t length) {
    if (data[0] != 0x1) {
        return;
    }
    cout << "Magic ";
    for (int i = 9; i < 13; i++) {
        cout << data[i];
    }
    cout << "\n";
    uint64_t headerSize = 0x0;
    headerSize = getInteger(data, 13);
    //int index = 0;
    /*for (int i = 13; i < 21; i++) {
        headerSize |= static_cast<int>(static_cast<unsigned char>(data[i]) << index * 8);
        index++;
    }*/
    cout << "Size of the header: ";
    cout << headerSize;
    cout << "\n";
    if (length != headerSize) {
        return;
    }
    uint64_t num_anim = 0x0;
    //index = 0;
    num_anim = getInteger(data, 21);
    /*for (int i = 21; i < 29; i++) {
        num_anim |= static_cast<int>(static_cast<unsigned char>(data[i]) << index * 8);
        index++;
    }*/
    cout << "Number of animations: ";
    cout << num_anim;
    cout << "\n";
    return;
}

uint64_t processCreator(char* data, uint64_t index) {
    if (data[index] != 0x2) {
        cout << "Creator not found!";
        return 0x0;
    }
    index++;
    uint64_t length = 0x0;
    length = getInteger(data, index);
    /*int num = 0;
    for (uint64_t i = index + 1; i < index + 9; i++) {
        length |= static_cast<uint64_t>(static_cast<unsigned char>(data[i]) << num * 8);
        num++;
    }*/
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
    creatorLength = getInteger(data, index);
    /*int num = 0;
    for (uint64_t i = index; i < index + 8; i++) {
        creatorLength |= static_cast<uint64_t>(static_cast<unsigned char>(data[i]) << num * 8);
        num++;
    }*/
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

int processCIFFData(char* data, uint64_t index, string fileName) {
    cout << "Magic ";
    for (uint64_t i = index; i < index + 4; i++) {
        cout << data[i];
    }
    cout << "\n";
    index += 4;
    uint64_t headerSize = 0x0;
    headerSize = getInteger(data, index);
    /*int num = 0;
    for (uint64_t i = index; i < index + 8; i++) {
        headerSize |= static_cast<uint64_t>(static_cast<unsigned char>(data[i]) << num * 8);
        num++;
    }*/
    cout << "Header size : " << headerSize << endl;
    index += 8;
    uint64_t contentSize = 0x0;
    contentSize = getInteger(data, index);
    /*num = 0;
    for (uint64_t i = index; i < index + 8; i++) {
        contentSize |= static_cast<uint64_t>(static_cast<unsigned char>(data[i]) << num * 8);
        num++;
    }*/
    cout << "Content size : " << contentSize << endl;
    index += 8;
    uint64_t width = 0x0;
    width = getInteger(data, index);
    /*num = 0;
    for (uint64_t i = index; i < index + 8; i++) {
        width |= static_cast<uint64_t>(static_cast<unsigned char>(data[i]) << num * 8);
        num++;
    }*/
    cout << "Width: " << width << endl;
    index += 8;
    uint64_t height = 0x0;
    height = getInteger(data, index);
    /*num = 0;
    for (uint64_t i = index; i < index + 8; i++) {
        height |= static_cast<uint64_t>(static_cast<unsigned char>(data[i]) << num * 8);
        num++;
    }*/
    cout << "Height: " << height << endl;
    index += 8;
    if (contentSize != width * height * 3) {
        cout << "Content condition not met!" << endl;
        return 1;
    }
    int num = 0;
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
    uint8_t* output;
    size_t webp = WebPEncodeRGB(pixels, width, height, width * 3, 100, &output);  
    for (size_t i = 0; i < webp; i++) {
        output[i] = (static_cast<unsigned char>(output[i]));
    }
    /*FILE* file;
    errno_t error;
    if ((error = fopen_s(&file, "image.webp", "w")) != 0) {
        //file = fopen_s("image.webp", "w");
        cout << "Could not open file!" << endl;
    }
    else {
        fwrite(output, 1, webp, file);
        fclose(file);
    }*/
    
    
    ofstream image(fileName + ".webp");
    if (image.is_open()) {
        for (size_t i = 0; i < webp; i++) {
            image << output[i];
        }
        image.close();
    }
    else {
        WebPFree(output);
        return 1;
    }
    WebPFree(output);
    return 0;
}

int processCIFF(char* data, uint64_t index, string fileName) {
    if (data[index] != 0x3) {
        cout << "Animation not found!";
        return 1;
    }
    index++;
    uint64_t length = 0x0;
    length = getInteger(data, index);
    /*int num = 0;
    for (uint64_t i = index + 1; i < index + 9; i++) {
        length |= static_cast<uint64_t>(static_cast<unsigned char>(data[i]) << num * 8);
        num++;
    }*/
    cout << "Animation length: ";
    cout << length;
    cout << "\n";
    index += 9;
    uint64_t anim_dur = 0x0;
    anim_dur = getInteger(data, index);
    /*num = 0;
    for (uint64_t i = index; i < index + 8; i++) {
        anim_dur |= static_cast<uint64_t>(static_cast<unsigned char>(data[i]) << num * 8);
        num++;
    }*/
    cout << "Duration of the animation: ";
    cout << anim_dur;
    cout << " ms\n";
    index += 8;
    int result = processCIFFData(data, index, fileName);
    return result;
}

string filename(string name) {
    string file = name.substr(name.find_last_of("/\\") + 1);
    string::size_type const p(file.find_last_of("."));
    return file.substr(0, p);
}

int main(int argc, char* argv[])
{
    if (argc != 2) {
        return 1;
    }
    bool caff;
    bool same = false;
    const char* first = "-caff";
    const char* second = "-ciff";
    for (int i = 0; argv[0][i] != '\0'; i++) {
        if (argv[0][i] == first[i]) {
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
        for (int i = 0; argv[1][i] != '\0'; i++) {
            if (argv[1][i] == second[i]) {
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
            return 1;
        }
    }
    if (argv[1] == NULL) {
        return 1;
    }
    string path = argv[1];
    string fileName = filename(path);
    char* data;
    loadFile(path, data);
    int result = 1;
    if (data != NULL) {
        if (caff) {
            uint64_t length = determineLength(data);
            processHeader(data, length);
            uint64_t index = length + 9;
            uint64_t size = processCreator(data, index);
            index += size;
            size = processCIFF(data, index, fileName);

        }
        else {
            result = processCIFFData(data, 0, fileName);
        }
    }
    delete[] data;
    return result;
}