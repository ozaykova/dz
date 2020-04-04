#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include <unordered_map>
using namespace std;

class ShannonFano
{
public:
    void build() {
        fano(' ', "", 0, number);
    }

    void addChance (int chance) {
        Tree.push_back(chance);
        result.push_back("");
        number++;
    }

    string get (int i) {
        return result[i];
    }

    void fano(char branch, string all, int start, int end) {
        string tmp = all;
        if (branch != ' ') {
            tmp += branch;
        }

        if ((start + 1) == end) {
            result[start] = tmp;
        }

        else {
            int med = medium(start, end);
            fano('0', tmp, start, med);
            fano('1', tmp, med, end);
        }
    }

    int medium(int start, int end)
    {
        int left = 0, right = 0, i = start, j = end - 1;

        while(i <= j) {
            if (left < right) {
                left += Tree[i];
                ++i;
            }
            else {
                right += Tree[j];
                --j;
            }
        }
        return i;
    }

    int number = 0;
    vector<int> Tree;
    vector<string> result;
};


bool cmp(pair<int, int> a, pair<int, int> b) {
    return a.first > b.first;
}

char bitToChar (string bit) {
    int ans = 0;
    for (int i = 0; i < 8; ++i) {
        if (bit[i] == '1') {
            ans += 1 << (7 - i);
        }
    }
    return char(ans);
}

string charToBit (char a) {
    string bit = "";
    int code = (int)a;

    for (int i = 0; i < 8; ++i) {
        if ((code - (1 << (7 - i))) >= 0) {
            bit += "1";
            code -= (1 << (7 - i));
        } else {
            bit += "0";
        }
    }
    return bit;
}


int main() {
    ifstream fin("input.txt", ios_base::binary);
   // freopen("output.txt", "w", stdout);

    int counter[256];
    for (int i = 0; i < 256; ++i)
        counter[i] = 0;
    string s;
    fin >> s;
    for (size_t i = 0; i < s.size(); ++i) {
        ++counter[int(s[i])];
    }
    fin.close();

    vector<pair<int, int>> symbolSet;
    int noZero = 0;
    for (int i = 0; i < 256; ++i) {
        if (counter[i] > 0) {
            ++noZero;
            symbolSet.push_back({counter[i], i});
        }
    }

    sort(symbolSet.begin(), symbolSet.end(), cmp);

    ShannonFano* shf = new ShannonFano();

    for (auto a: symbolSet) {
        shf->addChance(a.first);
    }
    
    shf->build();

    unordered_map<int, string> code;
    unordered_map<string, int> decode;

    string fanoResult;
    for (int i = 0; i < noZero; i++) {
       // fanoResult += shf->get(i);
        code[symbolSet[i].second] = shf->get(i);
        decode[shf->get(i)] = symbolSet[i].second;
    }

    for (size_t i = 0; i < s.size(); ++i) {
        fanoResult += code[s[i]];
    }

    fstream fout;
    fout.open("output.txt", ios::out);

    for (size_t i = 0; i < fanoResult.size(); i += 8) {
        fout << bitToChar(fanoResult.substr(i, 8));
    }

    int letter_count = fanoResult.size() / 8;

    if (8 * letter_count <= fanoResult.size())
        fout << fanoResult.substr(8 * letter_count);
    fout.close();

    string decoder = "";

    ifstream dec("output.txt", ios_base::binary);
    string curr;
    getline(dec, curr);
    dec.close();


    for (int i = 0 ; i < letter_count; i++) {
        decoder += charToBit(curr[i]);
    }

    if (letter_count <= curr.size()) {
        decoder += curr.substr(letter_count + 1);
    }

    string buff = "";
    for (int i = 0; i < decoder.size(); ++i) {
        buff += decoder[i];
        if (decode.find(buff) != decode.cend()) {
            cout << char(decode[buff]);
            buff = "";
        }
    }

    delete shf;

    return 0;
}
