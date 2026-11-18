#include "ReaderHashTable.h"
#include <iostream>
#include <functional>
#include <map>
using namespace std;

NutDocGia::NutDocGia(const DocGia& docGia) : duLieu(docGia), tiepTheo(nullptr) {}
ReaderHashTable::ReaderHashTable(size_t kichThuoc)
    : bang(kichThuoc == 0 ? 101 : kichThuoc, nullptr), soLuong(0) {
}
ReaderHashTable::~ReaderHashTable() { clear(); }

size_t ReaderHashTable::hashKey(const string& CMND) const {
    return hash<string>{}(CMND) % bang.size();
}
bool ReaderHashTable::insert(const DocGia& docGia) {
    if (docGia.CMND.empty()) return false;
    size_t viTri = hashKey(docGia.CMND);
    for (NutDocGia* nut = bang[viTri]; nut; nut = nut->tiepTheo) {
        if (nut->duLieu.CMND == docGia.CMND) { nut->duLieu = docGia; return false; }
    }
    NutDocGia* nutMoi = new NutDocGia(docGia);
    nutMoi->tiepTheo = bang[viTri];
    bang[viTri] = nutMoi;
    ++soLuong;
    return true;
}
bool ReaderHashTable::remove(const string& CMND) {
    size_t viTri = hashKey(CMND);
    NutDocGia* nut = bang[viTri];
    NutDocGia* truoc = nullptr;
    while (nut) {
        if (nut->duLieu.CMND == CMND) {
            if (!truoc) bang[viTri] = nut->tiepTheo;
            else truoc->tiepTheo = nut->tiepTheo;
            delete nut; --soLuong; return true;
        }
        truoc = nut; nut = nut->tiepTheo;
    }
    return false;
}
DocGia* ReaderHashTable::search(const string& CMND) const {
    size_t viTri = hashKey(CMND);
    for (NutDocGia* nut = bang[viTri]; nut; nut = nut->tiepTheo)
        if (nut->duLieu.CMND == CMND) return &nut->duLieu;
    return nullptr;
}
size_t ReaderHashTable::size() const { return soLuong; }
void ReaderHashTable::countByCardType() const {
    map<string, size_t> thongKe;
    for (NutDocGia* dau : bang)
        for (NutDocGia* nut = dau; nut; nut = nut->tiepTheo)
            ++thongKe[nut->duLieu.loaiThe];
    cout << "Thong ke doc gia theo loai the:\n";
    for (const auto& muc : thongKe) cout << muc.first << ": " << muc.second << '\n';
}
void ReaderHashTable::clear() {
    for (NutDocGia*& dau : bang) {
        while (dau) { NutDocGia* xoa = dau; dau = dau->tiepTheo; delete xoa; }
    }
    soLuong = 0;
}