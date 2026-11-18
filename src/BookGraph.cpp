#include "BookGraph.h"
#include <iostream>
#include <queue>
#include <vector>
using namespace std;

NutKeSach::NutKeSach(int dinh) : dinhKe(dinh), tiepTheo(nullptr) {}
BookGraph::BookGraph() {}
BookGraph::~BookGraph() { clear(); }

int BookGraph::findIndex(const string& ISBN) const {
    for (size_t i = 0; i < danhSachISBN.size(); ++i)
        if (danhSachISBN[i] == ISBN) return static_cast<int>(i);
    return -1;
}
int BookGraph::addVertex(const string& ISBN) {
    if (ISBN.empty()) return -1;
    int viTri = findIndex(ISBN);
    if (viTri >= 0) return viTri;
    danhSachISBN.push_back(ISBN);
    danhSachKe.push_back(nullptr);
    return static_cast<int>(danhSachISBN.size() - 1);
}
bool BookGraph::addEdge(const string& ISBN1, const string& ISBN2) {
    int a = addVertex(ISBN1), b = addVertex(ISBN2);
    if (a < 0 || b < 0 || a == b) return false;
    for (NutKeSach* nut = danhSachKe[a]; nut; nut = nut->tiepTheo)
        if (nut->dinhKe == b) return false;
    NutKeSach* canhA = new NutKeSach(b);
    canhA->tiepTheo = danhSachKe[a]; danhSachKe[a] = canhA;
    NutKeSach* canhB = new NutKeSach(a);
    canhB->tiepTheo = danhSachKe[b]; danhSachKe[b] = canhB;
    return true;
}
void BookGraph::BFS(const string& ISBNBatDau) const {
    int batDau = findIndex(ISBNBatDau);
    if (batDau < 0) { cout << "Khong tim thay ISBN trong do thi.\n"; return; }
    vector<bool> daTham(danhSachISBN.size(), false);
    queue<int> hangDoi; hangDoi.push(batDau); daTham[batDau] = true;
    cout << "BFS: ";
    while (!hangDoi.empty()) {
        int dinh = hangDoi.front(); hangDoi.pop();
        cout << danhSachISBN[dinh] << ' ';
        for (NutKeSach* nut = danhSachKe[dinh]; nut; nut = nut->tiepTheo) {
            if (!daTham[nut->dinhKe]) {
                daTham[nut->dinhKe] = true; hangDoi.push(nut->dinhKe);
            }
        }
    }
    cout << '\n';
}
void BookGraph::DFS(const string& ISBNBatDau) const {
    int batDau = findIndex(ISBNBatDau);
    if (batDau < 0) { cout << "Khong tim thay ISBN trong do thi.\n"; return; }
    vector<bool> daTham(danhSachISBN.size(), false);
    vector<int> nganXep(1, batDau);
    cout << "DFS: ";
    while (!nganXep.empty()) {
        int dinh = nganXep.back(); nganXep.pop_back();
        if (daTham[dinh]) continue;
        daTham[dinh] = true; cout << danhSachISBN[dinh] << ' ';
        vector<int> cacDinhKe;
        for (NutKeSach* nut = danhSachKe[dinh]; nut; nut = nut->tiepTheo)
            cacDinhKe.push_back(nut->dinhKe);
        for (vector<int>::reverse_iterator it = cacDinhKe.rbegin(); it != cacDinhKe.rend(); ++it)
            if (!daTham[*it]) nganXep.push_back(*it);
    }
    cout << '\n';
}
size_t BookGraph::vertexCount() const { return danhSachISBN.size(); }
void BookGraph::clearEdges() {
    for (NutKeSach*& dau : danhSachKe)
        while (dau) { NutKeSach* xoa = dau; dau = dau->tiepTheo; delete xoa; }
}
void BookGraph::clear() {
    clearEdges(); danhSachISBN.clear(); danhSachKe.clear();
}