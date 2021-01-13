#include <algorithm>
#include <bitset>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

typedef struct Code
{
    vector<string> codeparts;
    long PC;
    size_t LC;
} Code;

typedef map<string, long> table;
typedef vector<Code> Codevec;

typedef bool (*OpCodeFuncs)(Code &, table &, ostream &);
bool pass1(ifstream &fin, table &SymbolTable, Codevec &code);

bool CodeSeparate(string codeline, vector<string> &codeparts);

bool CreateSymbolTable(vector<string> &codeparts, table &SymbolTable, size_t LC, long &PC, Codevec &code);

bool pass2(table SymbolTable, Codevec &code, ostream &fout);

bool STRINGZ_judge(vector<string> &codeparts, long &PC, Codevec &code, size_t LC);

bool FILL_judge(vector<string> &codeparts, long &PC, Codevec &code, size_t LC);

bool BLKW_judge(vector<string> &codeparts, long &PC, Codevec &code, size_t LC);

bool ImmToBinary(string imm, int bits, string &bimm, long &dimm);

bool OffsetInRange(long offset, int bits);

bool LabelOrImm(Code &Code, table &SymbolTable, ostream &fout, int bitlimit);

bool ADD(Code &, table &SymbolTable, ostream &fout);
bool AND(Code &, table &SymbolTable, ostream &fout);
bool BR(Code &, table &SymbolTable, ostream &fout);
bool JMP(Code &, table &SymbolTable, ostream &fout);
bool JSR(Code &, table &SymbolTable, ostream &fout);
bool LD(Code &, table &SymbolTable, ostream &fout);
bool JSRR(Code &, table &SymbolTable, ostream &fout);
bool LDR(Code &, table &SymbolTable, ostream &fout);
bool LEA(Code &, table &SymbolTable, ostream &fout);
bool NOT(Code &, table &SymbolTable, ostream &fout);
bool RET(Code &, table &SymbolTable, ostream &fout);
bool TRAP(Code &, table &SymbolTable, ostream &fout);
bool STRINGZ(Code &, table &SymbolTable, ostream &fout);
bool FILL(Code &, table &SymbolTable, ostream &fout);
bool BLKW(Code &, table &SymbolTable, ostream &fout);
bool ORIG(Code &, table &SymbolTable, ostream &fout);
bool HALT(Code &, table &SymbolTable, ostream &fout);


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

//  all Opcodes including pseudo opcodes
vector<string> OpCode = {"ADD", "AND", "JMP", "JSR", "JSRR", "LD", "LDI", "LDR", "LEA",
                         "NOT", "RET", "RTI", "ST", "STI", "STR", "TRAP",
                         "BR", "BRz", "BRn", "BRp", "BRzp", "BRnz", "BRnp", "BRnzp",
                         "HALT", "OUT", "GETC", "PUTS", "IN", "PUTSP"};

//  map between assembly to binary code
map<string, string> mapAsmBin = {
    {"ADD", "0001"},
    {"AND", "0101"},
    {"BR", "0000111"},
    {"JMP", "1100000"},
    {"JSR", "01001"},
    {"JSRR", "0100000"},
    {"LD", "0010"},
    {"LDI", "1010"},
    {"LDR", "0110"},
    {"LEA", "1110"},
    {"NOT", "1001"},
    {"RET", "1100000111000000"},
    {"RTI", "1000000000000000"},
    {"ST", "0011"},
    {"STI", "1011"},
    {"STR", "0111"},
    {"TRAP", "11110000"},
    {"BRz", "0000010"},
    {"BRn", "0000100"},
    {"BRp", "0000001"},
    {"BRzp", "0000011"},
    {"BRnz", "0000110"},
    {"BRnp", "0000101"},
    {"BRnzp", "0000111"},
    {"BR", "0000111"},
    {"GETC", "1111000000100000"},
    {"OUT", "1111000000100001"},
    {"PUTS", "1111000000100010"},
    {"IN", "1111000000100011"},
    {"PUTSP", "1111000000100100"},
    {"HALT", "1111000000100101"}};

//  map between Opcode and functions to operate
map<string, OpCodeFuncs> Funcs = {
    {"ADD", &ADD},
    {"AND", &ADD},
    {"BR", &BR},
    {"BRn", &BR},
    {"BRz", &BR},
    {"BRp", &BR},
    {"BRnz", &BR},
    {"BRnp", &BR},
    {"BRzp", &BR},
    {"BRnzp", &BR},
    {"JMP", &JMP},
    {"JSR", &JSR},
    {"JSRR", &JSRR},
    {"LD", &LD},
    {"LDI", &LD},
    {"LDR", &LDR},
    {"LEA", &LEA},
    {"NOT", &NOT},
    {"RET", &RET},
    {"RTI", &RET},
    {"ST", &LD},
    {"STI", &LD},
    {"STR", &LDR},
    {"TRAP", &TRAP},
    {".STRINGZ", &STRINGZ},
    {".FILL", &FILL},
    {".BLKW", &BLKW},
    {".ORIG", &ORIG},
    {"HALT", &HALT},
    {"OUT", &HALT},
    {"GETC", &HALT},
    {"PUTSP", &HALT},
    {"PUTS", &HALT},
    {"IN", &HALT}
};

//  map between registers to binary code
map<string, string> RegisterMap = {
    {"R1", "001"},
    {"R2", "010"},
    {"R3", "011"},
    {"R4", "100"},
    {"R5", "101"},
    {"R6", "110"},
    {"R7", "111"},
    {"R0", "000"},
};
int main(int argc, char **args)
{
    table SymbolTable;
    Codevec code;
    ifstream fin;
    ofstream fout;
    string source = args[1];
    string dest = source;
    dest.replace(source.size() - 4, 4, ".bin");

    fin.open(source, ios::in);
    if (!pass1(fin, SymbolTable, code))
    {
        cout << "pass1 failed\n";
        return 0;
    }
    fin.close();
    fin.open(source, ios::in);
    fout.open(dest, ios::out);
    if (!pass2(SymbolTable, code, fout))
    {
        cout << "pass2 failed.\n";
        return 0;
    }
}

bool pass1(ifstream &fin, table &SymbolTable, Codevec &code)
{
    string codeline;          //  代码行
    vector<string> codeparts; //  代码各部分
    size_t LC = 0;            //  行數
    long PC = 0;              //  真PC
    bool pro_start = false, pro_end = false;
    while (fin.peek() != EOF)
    {
        getline(fin, codeline);
        ++LC;
        size_t notspacei = codeline.find_first_not_of(" \n\r");
        if (notspacei == string::npos) //  空白行
            continue;
        if (codeline[notspacei] != ';') //  指令或者伪操作
        {
            size_t comment;
            if ((comment = codeline.find_first_of(';')) != string::npos)
            {
                codeline = codeline.substr(notspacei, comment - notspacei);
            }
            else
            {
                codeline = codeline.substr(notspacei);
            }

            if (!CodeSeparate(codeline, codeparts)) return false;
            if (codeparts[0] == ".ORIG")
                pro_start = true;
            if (codeparts[0] == ".END")
            {
                pro_end = true;
                break;
            }
            if (!CreateSymbolTable(codeparts, SymbolTable, LC, PC, code))
            {
                return false;
            }
        }
        else //  注释行
            continue;
    }
    if (!pro_start) //  没有.ORIG
    {
        cout << "Please clarify where your program starts\n";
        return false;
    }
    else if (!pro_end) //  没有.END
    {
        cout << "Please clarify where your program ends\n";
        return false;
    }
    return true;
}

bool CodeSeparate(string codeline, vector<string> &codeparts) //  以空格和,为间隔分开字符串到parts
{
    codeparts.clear();
    string separators = " ,\t\r\n";
    string part;
    size_t start = codeline.find_first_not_of(separators);
    size_t end, max_size;

    while (start != string::npos)
    {
        end = codeline.find_first_of(separators, start + 1);
        if (end == string::npos)
            end = codeline.length();

        part = codeline.substr(start, end - start);
        codeparts.push_back(part);
        start = codeline.find_first_not_of(separators, end + 1);
        if (codeline[start] == '\"')
        {
            if (count(codeline.begin() + start, codeline.end(), '\"') == 1)
            {
                cout << "Invalid string.\n";
                return false;
            }
            else
            {
                end = codeline.find_last_of('\"');
                part = codeline.substr(start, end-start+1);
                codeparts.push_back(part);
                return true;
            }
        }
    }
    return true;
}

bool CreateSymbolTable(vector<string> &codeparts, table &SymbolTable, size_t LC, long &PC, Codevec &code)
{
    vector<string>::iterator iter;
    if (codeparts[0] == ".ORIG") //  要求后面是x3000的形式
    {
        stringstream ss;
        ss << std::hex << codeparts[1].substr(1);
        ss >> PC;
        PC = PC - 1;
        code.push_back({codeparts, PC, LC});
        return true;
    }
    else if (codeparts[0] == ".END")
    {
        PC = 0;
        return true;
    }
    else if (codeparts[0] == ".STRINGZ")
    {
        PC += 1;
        return STRINGZ_judge(codeparts, PC, code, LC);
    }
    else if (codeparts[0] == ".FILL")
    {
        PC += 1;
        return FILL_judge(codeparts, PC, code, LC);
    }
    else if (codeparts[0] == ".BLKW")
    {
        PC += 1;
        return BLKW_judge(codeparts, PC, code, LC);
    }

    PC += 1;
    if ((iter = find(OpCode.begin(), OpCode.end(), codeparts[0])) == OpCode.end()) //  不是操作码而是标签
    {
        table ::iterator symboliter = SymbolTable.find(codeparts[0]);
        if (symboliter != SymbolTable.end())
        {
            cout << "line " << LC << ": " << codeparts[0] << " have defined twice\n";
            return false;
        }
        else if (symboliter == SymbolTable.end())
        {
            if (codeparts.size() == 1)
            {
                cout << "line " << LC << ": what do you want to label?\n";
                return false;
            }
            else if (codeparts[1] == ".STRINGZ")
            {
                SymbolTable.insert(pair<string, long>(codeparts[0], PC));
                if (!STRINGZ_judge(codeparts, PC, code, LC))
                    return false;
            }
            else if (codeparts[1] == ".FILL")
            {
                SymbolTable.insert(pair<string, long>(codeparts[0], PC));
                if (!FILL_judge(codeparts, PC, code, LC))
                    return false;
            }
            else if (codeparts[1] == ".BLKW")
            {
                SymbolTable.insert(pair<string, long>(codeparts[0], PC));
                if (!BLKW_judge(codeparts, PC, code, LC))
                    return false;
            }
            else //  加入标签表并且出去将除去标签的代码插入code vector
            {
                SymbolTable.insert(pair<string, long>(codeparts[0], PC));
                vector<string> true_code;
                copy(codeparts.begin() + 1, codeparts.end(), back_inserter(true_code));
                code.push_back({true_code, PC, LC});
            }
        }
    }
    else //  没有标签的代码
    {
        code.push_back({codeparts, PC, LC});
    }

    return true;
}

bool STRINGZ_judge(vector<string> &codeparts, long &PC, Codevec &code, size_t LC)
{
    vector<string>::iterator stringz_start = find(codeparts.begin(), codeparts.end(), ".STRINGZ");

    if (codeparts.end() - stringz_start != 2) //  不是两个参数
    {
        cout << "line " << LC << ": please input right format like .STRINGZ \"ABCD\".\n";
        return false;
    }
    else if ((*((stringz_start + 1).base()))[0] != '\"' || (*(stringz_start + 1).base())[0] != '\"') //  不是字符串
    {
        cout << "line " << LC << ": please input right string with two \"\n";
        return false;
    }
    else
    {
        vector<string> true_code;
        copy(stringz_start, codeparts.end(), back_inserter(true_code));
        code.push_back({true_code, PC, LC});
        PC += (*(stringz_start + 1).base()).length() - 2;
        return true;
    }
}

bool FILL_judge(vector<string> &codeparts, long &PC, Codevec &code, size_t LC)
{
    vector<string>::iterator fill_start = find(codeparts.begin(), codeparts.end(), ".FILL");

    vector<string> true_code;
    copy(fill_start, codeparts.end(), back_inserter(true_code));
    if (codeparts.end() - fill_start != 2) //  不是两个字符串
    {
        cout << "line " << LC << ": please input right format like .FILL x000A.\n";
        return false;
    }
    code.push_back({true_code, PC, LC});
    return true;
}

bool BLKW_judge(vector<string> &codeparts, long &PC, Codevec &code, size_t LC)
{
    vector<string>::iterator blkw_start = find(codeparts.begin(), codeparts.end(), ".BLKW");

    vector<string> true_code;
    copy(blkw_start, codeparts.end(), back_inserter(true_code));
    if (true_code.size() != 2) //  不是两个字符串
    {
        cout << "line " << LC << ": please input right format like .BLKW #2.\n";
        return false;
    }
    code.push_back({true_code, PC, LC});
    string bimm;
    long dimm;
    ImmToBinary(true_code[1], 16, bimm, dimm);
    PC += dimm - 1;
    return true;
}

bool pass2(table SymbolTable, Codevec &code, ostream &fout)
{
    long CurrentPC = 0;
    for (int i = 0; i < code.size(); i++)
    {
        if (Funcs.find(code[i].codeparts[0]) == Funcs.end()) //  发现除去标签之后的代码不是正常代码
        {
            cout << "line " << code[i].LC << ": no such operand.\n";
            return false;
        }
        if (!Funcs[code[i].codeparts[0]](code[i], SymbolTable, fout)) //  译码错误
        {
            return false;
        }
    }
    return true;
}

bool ADD(Code &code, table &SymbolTable, ostream &fout)
{
    if (code.codeparts.size() != 4) //  不是4个操作数
    {
        cout << "line " << code.LC << ": incorrect number of parameters.\n";
        return false;
    }
    else if (RegisterMap.find(code.codeparts[1]) == RegisterMap.end() || RegisterMap.find(code.codeparts[2]) == RegisterMap.end()) //  前两个不是寄存器
    {
        cout << "line " << code.LC << ": please input right register like R1.\n";
        return false;
    }
    else if (RegisterMap.find(code.codeparts[3]) != RegisterMap.end()) //  寄存器加法
    {
        fout << mapAsmBin[code.codeparts[0]] << RegisterMap[code.codeparts[1]] << RegisterMap[code.codeparts[2]] << "000" << RegisterMap[code.codeparts[3]] << endl;
        return true;
    }
    else //  立即数
    {
        string bimm;
        long dimm;
        if (ImmToBinary(code.codeparts[3], 5, bimm, dimm)) //  立即数符合范围
        {
            fout << mapAsmBin[code.codeparts[0]] << RegisterMap[code.codeparts[1]] << RegisterMap[code.codeparts[2]] << "1" << bimm << endl;
            return true;
        }
        else
        {
            cout << "line " << code.LC << ": invalid imm.\n";
            return false;
        }
    }
}
bool BR(Code &code, table &SymbolTable, ostream &fout)
{
    if (code.codeparts.size() != 2)
    {
        cout << "line " << code.LC << ": incorrect number of parameters.\n";
        return false;
    }
    else
    {
        return LabelOrImm(code, SymbolTable, fout, 9);
    }
}
bool JMP(Code &code, table &SymbolTable, ostream &fout)
{
    if (code.codeparts.size() != 2)
    {
        cout << "line " << code.LC << ": incorrect number of parameters.\n";
        return false;
    }
    else if (RegisterMap.find(code.codeparts[1]) == RegisterMap.end())
    {
        cout << "line " << code.LC << ": please input right register like R1.\n";
        return false;
    }
    else
    {
        fout << mapAsmBin[code.codeparts[0]] << RegisterMap[code.codeparts[1]] << "000000\n";
        return true;
    }
}
bool JSR(Code &code, table &SymbolTable, ostream &fout)
{
    if (code.codeparts.size() != 2)
    {
        cout << "line " << code.LC << ": incorrect number of parameters.\n";
        return false;
    }
    else
    {
        return LabelOrImm(code, SymbolTable, fout, 11);
    }
}
bool JSRR(Code &code, table &SymbolTable, ostream &fout)
{
    if (code.codeparts.size() != 2)
    {
        cout << "line " << code.LC << ": incorrect number of parameters.\n";
        return false;
    }
    else if (RegisterMap.find(code.codeparts[1]) == RegisterMap.end()) //  前一个不是寄存器
    {
        cout << "line " << code.LC << ": please input right register like R1.\n";
        return false;
    }
    else
    {
        fout << mapAsmBin[code.codeparts[0]] << RegisterMap[code.codeparts[1]] << "000000\n";
        return true;
    }
}
bool LD(Code &code, table &SymbolTable, ostream &fout)
{
    if (code.codeparts.size() != 3)
    {
        cout << "line " << code.LC << ": incorrect number of parameters.\n";
        return false;
    }
    else if (RegisterMap.find(code.codeparts[1]) == RegisterMap.end())
    {
        cout << "line " << code.LC << ": please input right register like R1.\n";
        return false;
    }
    else
    {
        return LabelOrImm(code, SymbolTable, fout, 9);
    }
}
bool LDR(Code &code, table &SymbolTable, ostream &fout)
{
    if (code.codeparts.size() != 4)
    {
        cout << "line " << code.LC << ": incorrect number of parameters.\n";
        return false;
    }
    else if (RegisterMap.find(code.codeparts[1]) == RegisterMap.end() || RegisterMap.find(code.codeparts[2]) == RegisterMap.end()) //  前两个不是寄存器
    {
        cout << "line " << code.LC << ": please input right register like R1.\n";
        return false;
    }
    else
    {
        string bimm;
        long dimm;
        if (ImmToBinary(code.codeparts[3], 6, bimm, dimm))
        {
            fout << mapAsmBin[code.codeparts[0]] << RegisterMap[code.codeparts[1]] << RegisterMap[code.codeparts[2]] << bimm << endl;
            return true;
        }
        else
        {
            cout << "line " << code.LC << ": PCoffset out of range.\n";
            return false;
        }
    }
}
bool LEA(Code &code, table &SymbolTable, ostream &fout)
{
    if (code.codeparts.size() != 3)
    {
        cout << "line " << code.LC << ": incorrect number of parameters.\n";
        return false;
    }
    else if (RegisterMap.find(code.codeparts[1]) == RegisterMap.end())
    {
        cout << "line " << code.LC << ": please input right register like R1.\n";
        return false;
    }
    else
    {
        return LabelOrImm(code, SymbolTable, fout, 9);
    }
}
bool NOT(Code &code, table &SymbolTable, ostream &fout)
{
    if (code.codeparts.size() != 3)
    {
        cout << "line " << code.LC << ": incorrect number of parameters.\n";
        return false;
    }
    else if (RegisterMap.find(code.codeparts[1]) == RegisterMap.end() || RegisterMap.find(code.codeparts[2]) == RegisterMap.end())
    {
        cout << "line " << code.LC << ": please input right register like R1.\n";
        return false;
    }
    else
    {
        fout << mapAsmBin[code.codeparts[0]] << RegisterMap[code.codeparts[1]] << RegisterMap[code.codeparts[2]] << "111111" << endl;
        return true;
    }
}
bool RET(Code &code, table &SymbolTable, ostream &fout)
{
    if (code.codeparts.size() != 1)
    {
        cout << "line " << code.LC << ": incorrect number of parameters.\n";
        return false;
    }
    else
        fout << mapAsmBin[code.codeparts[0]] << endl;
    return true;
}
bool TRAP(Code &code, table &SymbolTable, ostream &fout)
{
    if (code.codeparts.size() != 2)
    {
        cout << "line " << code.LC << ": incorrect number of parameters.\n";
        return false;
    }
    else
        return LabelOrImm(code, SymbolTable, fout, 8);
}
bool STRINGZ(Code &code, table &SymbolTable, ostream &fout)
{
    if (code.codeparts.size() != 2)
    {
        cout << "line " << code.LC << ": incorrect number of parameters.\n";
        return false;
    }
    else
    {
        string temp;
        for (int i = 1; i < code.codeparts[1].length()-1; i++)
        {
                temp = bitset<16>(code.codeparts[1][i]).to_string();
            fout << temp << endl;
        }
        fout << "0000000000000000" << endl;
        return true;
    }
}
bool FILL(Code &code, table &SymbolTable, ostream &fout)
{
    if (code.codeparts.size() != 2)
    {
        cout << "line " << code.LC << ": incorrect number of parameters.\n";
        return false;
    }
    else
    {
        string bimm;
        long dimm;
        if (!ImmToBinary(code.codeparts[1], 16, bimm, dimm))
        {
            cout << "line " << code.LC << ": incorrect FILL format.\n";
        }
        fout << bimm << endl;
        return true;
    }
}
bool BLKW(Code &code, table &SymbolTable, ostream &fout)
{
    if (code.codeparts.size() != 2)
    {
        cout << "line " << code.LC << ": incorrect number of parameters.\n";
        return false;
    }
    else
    {
        string bimm;
        long dimm;
        ImmToBinary(code.codeparts[1], 16, bimm, dimm);
        for (int i = 0; i < dimm; i++)
        {
            fout << "0000000000000000" << endl;
        }
        return true;
    }
}
bool ORIG(Code &code, table &SymbolTable, ostream &fout)
{
    string bimm;
    long dimm;
    if (!ImmToBinary(code.codeparts[1], 16, bimm, dimm))
    {
        cout << "line " << code.LC << ": invalid addr.\n";
        return false;
    }
    fout << bimm << endl;
    return true;
}
//  imm为任意类型的数字字符串,需要有前缀，bits为限制位，包括符号位，bimm为转化之后的bit位的字符串，dimm为十进制实际值
//  如果没有超出bits位的返回，则返回true
bool ImmToBinary(string imm, int bits, string &bimm, long &dimm)
{
    dimm = 0;
    long bmax = 0;
    bimm.clear();
    if (imm[0] != '#' && imm[0] != 'x' && imm[0] != 'b')
        return false;
    else //  确定dimm
    {
        string true_imm;
        if (imm[1] != '-')
            true_imm = imm.substr(1);
        else
            true_imm = imm.substr(2);

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
            bmax = 1;
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
            break;
        case 'b':
            bmax = 1;
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
            break;
        default:
            break;
        }
    }
    if (imm[1] == '-')
        dimm = -dimm;
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

bool LabelOrImm(Code &code, table &SymbolTable, ostream &fout, int bitlimit)
{
    string offset;
    long dimm;
    long num_offset;
    if (SymbolTable.find(code.codeparts[code.codeparts.size() - 1]) == SymbolTable.end()) //  是数字形式
    {
        string temp;
        ImmToBinary(code.codeparts[code.codeparts.size() - 1], 16, temp, num_offset); //  首先得到dimm，即目标
        if (!OffsetInRange(num_offset, bitlimit))
        {
            cout << "line " << code.LC << ": PCoffset out of range.\n";
            return false;
        }
        else
        {
            string bstring = bitset<16>(num_offset).to_string();
            offset = bstring.substr(bstring.length() - bitlimit);
            if (code.codeparts.size() == 2)
                fout << mapAsmBin[code.codeparts[0]] << offset << endl;
            else if (code.codeparts.size() == 3)
                fout << mapAsmBin[code.codeparts[0]] << RegisterMap[code.codeparts[1]] << offset << endl;
            else if (code.codeparts.size() == 4)
                fout << mapAsmBin[code.codeparts[0]] << RegisterMap[code.codeparts[1]] << RegisterMap[code.codeparts[2]] << offset << endl;
            return true;
        }
    }
    else //  label形式
    {
        num_offset = SymbolTable[code.codeparts[code.codeparts.size() - 1]] - (code.PC + 1);
        if (!OffsetInRange(num_offset, bitlimit))
        {
            cout << "line " << code.LC << ": PCoffset out of range.\n";
            return false;
        }
        else
        {
            string bstring = bitset<16>(num_offset).to_string();
            offset = bstring.substr(bstring.length() - bitlimit);
            if (code.codeparts.size() == 2)
                fout << mapAsmBin[code.codeparts[0]] << offset << endl;
            else if (code.codeparts.size() == 3)
                fout << mapAsmBin[code.codeparts[0]] << RegisterMap[code.codeparts[1]] << offset << endl;
            else if (code.codeparts.size() == 4)
                fout << mapAsmBin[code.codeparts[0]] << RegisterMap[code.codeparts[1]] << RegisterMap[code.codeparts[2]] << offset << endl;
            return true;
        }
    }
}
bool HALT(Code &code, table &SymbolTable, ostream &fout)
{
    if (code.codeparts.size() != 1)
    {
        cout << "line " << code.LC << ": incorrect number of parameters.\n";
        return false;
    }
    else
    {
        fout << mapAsmBin[code.codeparts[0]] << endl;
        return true;
    }
    
}
