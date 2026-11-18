#include "Trie.h"
using namespace std;

NutTrie::NutTrie() : laKetThuc(false), tenGoc("") {}
BookTrie::BookTrie() : goc(new NutTrie()) {}
BookTrie::~BookTrie() { clear(); delete goc; goc = nullptr; }

bool BookTrie::insert(const string& tenSach) {
    if (tenSach.empty()) return false;
    NutTrie* nut = goc;
    for (char kyTu : tenSach) {
        if (nut->con.find(kyTu) == nut->con.end()) nut->con[kyTu] = new NutTrie();
        nut = nut->con[kyTu];
    }
    bool moi = !nut->laKetThuc;
    nut->laKetThuc = true; nut->tenGoc = tenSach;
    return moi;
}
bool BookTrie::search(const string& tenSach) const {
    NutTrie* nut = goc;
    for (char kyTu : tenSach) {
        map<char, NutTrie*>::const_iterator it = nut->con.find(kyTu);
        if (it == nut->con.end()) return false;
        nut = it->second;
    }
    return nut->laKetThuc;
}
void BookTrie::collectWords(NutTrie* nut, vector<string>& ketQua) const {
    if (!nut) return;
    if (nut->laKetThuc) ketQua.push_back(nut->tenGoc);
    for (const auto& cap : nut->con) collectWords(cap.second, ketQua);
}
vector<string> BookTrie::searchPrefix(const string& tienTo) const {
    vector<string> ketQua;
    NutTrie* nut = goc;
    for (char kyTu : tienTo) {
        map<char, NutTrie*>::const_iterator it = nut->con.find(kyTu);
        if (it == nut->con.end()) return ketQua;
        nut = it->second;
    }
    collectWords(nut, ketQua);
    return ketQua;
}
bool BookTrie::remove(const string& tenSach) {
    NutTrie* nut = goc;
    for (char kyTu : tenSach) {
        map<char, NutTrie*>::iterator it = nut->con.find(kyTu);
        if (it == nut->con.end()) return false;
        nut = it->second;
    }
    if (!nut->laKetThuc) return false;
    nut->laKetThuc = false; nut->tenGoc.clear();
    return true;
}
void BookTrie::clearNode(NutTrie* nut) {
    if (!nut) return;
    for (auto& cap : nut->con) { clearNode(cap.second); delete cap.second; }
    nut->con.clear(); nut->laKetThuc = false; nut->tenGoc.clear();
}
void BookTrie::clear() { if (goc) clearNode(goc); }