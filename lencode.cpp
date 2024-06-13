#include <iostream>
#include <fstream>
#include <bitset>
#include <map>

using namespace std;

class Encoder {
    public:
        void encode(int argc, char **argv);
        void getBinaryInteger(short int num);
    private:
        ifstream infile;
        ofstream outfile;
        map<string, int> dict;
        string p;
        string pc;
        char c;
        int keyIndex;
};

void Encoder::encode(int argc, char **argv) {
    if (argc != 3) {
        cout << "Error: lencode <input_file_path> <output_file_path>" << endl;
    }
    infile.open(argv[1]);
    if (!infile) {
        cout << "Error: no such file: " + string(argv[1]) << endl;
    }
    outfile.open(argv[2]);
    if (!outfile) {
        cout << "Error: no such file: " + string(argv[2]) << endl;
    }
    keyIndex = 0;
    while (infile >> noskipws >> c) {
        pc = p + c;
        if (p.empty()) {
            // first char
            p = c;
            continue;
        }

        map<string, int>::iterator key = dict.find(pc);
        if (key == dict.end()) {
            // pc not found in dictionary
            if (keyIndex < 32768) {
                dict.insert(pair<string, int>(pc, keyIndex++));
            }
            if (p.length() >= 3) {
                getBinaryInteger(dict.find(p)->second);
            } else {
                outfile << p;
            }
            p = c;
        } else {
            // pc found in dictionary
            p = pc;
        }
    }

    // last char read in
    if (dict.find(p) != dict.end() && p.length() > 3) {
        getBinaryInteger(dict.find(p)->second);
    } else {
        outfile << p;
    }

    infile.close();
    outfile.close();
}

void Encoder::getBinaryInteger(short int index) {
    // get an integer in 16bits binary format
    bitset<16> binaryInt(index);
    bitset<16> separator(0xff);

    // set the first bit to 1, split to 2 8bits
    binaryInt.set(15, 1);
    bitset<8> byte1(((binaryInt >> 8) & separator).to_ulong());
    bitset<8> byte2((binaryInt & separator).to_ulong());

    outfile << char(byte1.to_ulong()) << char(byte2.to_ulong());
}

int main(int argc, char **argv) {
    Encoder encoder;
    encoder.encode(argc, argv);
    return 0;
}