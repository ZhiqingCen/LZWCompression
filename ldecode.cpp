#include <iostream>
#include <fstream>
#include <bitset>
#include <map>
#include <vector>
#include <algorithm>

using namespace std;

class Decoder {
    public:
        void decode(int argc, char **argv);
    private:
        ifstream infile;
        ofstream outfile;
        map<int, string> dict;
        string p;
        string pc;
        char c;
        char c2;
        int key;
        int keyIndex;
        vector<string> values;
};

void Decoder::decode(int argc, char **argv) {
    if (argc != 3) {
        cout << "Error: ldecode <input_file_path> <output_file_path>" << endl;
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
        bitset<8> readChar(c);

        if (readChar[7] == 0) {
            // 8 bits read in is a char
            if (p.empty()) {
                // first char
                outfile << c;
                p = c;
                continue;
            }
            pc = p + c;
            if (find(values.begin(), values.end(), pc) == values.end()) {
                // pc not found in dictionary
                if (keyIndex < 32768) {
                    values.push_back(pc);
                    dict.insert(pair<int, string>(keyIndex++, pc));
                }
                outfile << c;
                p = c;
            } else {
                // pc found in dictionary
                outfile << c;
                p = pc;
            }
        } else {
            // 8 bits read in is an integer
            readChar.set(7, 0);
            // read another 8 bits
            infile >> noskipws >> c2;
            bitset<8> readChar2(c2);
            bitset<16> binaryInt(readChar.to_string() + readChar2.to_string());
            key = binaryInt.to_ulong();

            if (dict.find(key) == dict.end()) {
                // integer not found in dictionary
                pc = p + p[0];
                if (keyIndex < 32768) {
                    values.push_back(pc);
                    dict.insert(pair<int, string>(keyIndex++, pc));
                }
                outfile << pc;
            } else {
                // integer found in dictionary
                pc = dict[key];
                outfile << pc;
                if (keyIndex < 32768) {
                    values.push_back(p + pc[0]);
                    dict.insert(pair<int, string>(keyIndex++, p + pc[0]));
                }
            }
            p = pc;
        }
    }

    infile.close();
    outfile.close();
}

int main(int argc, char **argv) {
    Decoder decoder;
    decoder.decode(argc, argv);
    return 0;
}