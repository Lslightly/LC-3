#include <map>
#include <iostream>
#include <fstream>

using namespace std;

map<string, string> HtoB = { //  Hex to Binary string
    {"0", "0000"},
    {"1", "0001"},
    {"2", "0010"},
    {"3", "0011"},
    {"4", "0100"},
    {"5", "0101"},
    {"6", "0110"},
    {"7", "0111"},
    {"8", "1000"},
    {"9", "1001"},
    {"a", "1010"},
    {"b", "1011"},
    {"c", "1100"},
    {"d", "1101"},
    {"e", "1110"},
    {"f", "1111"},
    {"A", "1010"},
    {"B", "1011"},
    {"C", "1100"},
    {"D", "1101"},
    {"E", "1110"},
    {"F", "1111"}};

int main(void)
{
    fstream fout;
    fout.open("true.bin", ios::out);
    string hexstring;
    char ch = 0;
    int i = 0;
    while ((ch = getchar()) && ch != 'q')
    {
        hexstring = ch;
        fout << HtoB[hexstring];
        i++;
        if (i == 4)
        {
            fout << endl;
            i = 0;
            getchar();
        }
    }
    fout.close();
    return 0;
}