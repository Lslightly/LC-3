#include <algorithm>
#include <iostream>
#include <map>
#include <stdlib.h>
#include <string>
#include <bitset>
using namespace std;

map<string, string> HtoB = {
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
bool ImmToBinary(string imm, int bits, string &bimm, long &dimm);

bool OffsetInRange(long offset, int bits);
int main(void)
{
    int n = -2;
    string imm = "x800", bimm;
    int bits = 0;
    long dimm;
    cin >> imm >> bits;
    ImmToBinary(imm, bits, bimm, dimm);
    cout << bimm <<" " << dimm << endl;
    return 0;
}

bool ImmToBinary(string imm, int bits, string &bimm, long &dimm)
{
    dimm = 0;
    bimm.clear();
    if (imm[0] != '#' && imm[0] != 'x' && imm[0] != 'b')
        return false;
    else //  确定dimm
    {
        long bmax = 1;
        string true_imm = imm.substr(1);
        switch (imm[0])
        {
        case '#':
            for (int i = 0; i < true_imm.length(); i++)
            {
                if ((true_imm[i] > '9' || true_imm[i] < '0') && true_imm[i] != '-')
                    return false;
            }
            dimm = stoi(true_imm);
            break;
        case 'x':
            for (int i = 0; i < true_imm.length(); i++)
            {
                if (HtoB.find(true_imm.substr(i, 1)) == HtoB.end())
                    return false;
                else
                    bimm += HtoB[true_imm.substr(i, 1)];
            }
            for (int i = 0; i < bimm.length(); i++)
            {
                dimm = 2 * dimm + bimm[i] - '0';
                bmax *= 2;
            }
            if (bimm[0] == '1')
            {
                dimm -= bmax;
            }
            break;
        case 'b':
            for (int i = 0; i < true_imm.length(); i++)
            {
                if (true_imm[i] != '0' && true_imm[i] != '1')
                    return false;
            }
            bimm = true_imm;
            for (int i = 0; i < bimm.length(); i++)
            {
                dimm = 2 * dimm + bimm[i] - '0';
                bmax *= 2;
            }
            if (bimm[0] == '1')
                dimm -= bmax;
            break;
        default:
            break;
        }
    }

    if (!OffsetInRange(dimm, bits))
        return false;
    else
    {
        bimm = bitset<16>(dimm).to_string();
        bimm = bimm.substr(bimm.length() - bits);
        return true;
    }
}
bool OffsetInRange(long offset, int bits)
{
    long min_offset = -1;
    long max_offset = 1;
    for (int i = 0; i < bits - 1; i++)
    {
        min_offset *= 2;
        max_offset *= 2;
    }
    max_offset -= 1;
    return !(offset < min_offset || offset > max_offset);
}