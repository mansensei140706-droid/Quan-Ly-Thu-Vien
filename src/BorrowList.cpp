#include "BorrowList.h"
#include <iostream>
using namespace std;

NutPhieuMuon::NutPhieuMuon(const PhieuMuon& p) : duLieu(p), tiepTheo(nullptr) {}
BorrowList::BorrowList() : dau(nullptr), cuoi(nullptr), soLuong(0) {}
BorrowList::~BorrowList() { clear(); }

bool BorrowList::insert(const PhieuMuon& phieuMuon) {
    if (phieuMuon.maPhieu.empty() || search(phieuMuon.maPhieu)) return false;
    NutPhieuMuon* moi = new NutPhieuMuon(phieuMuon);
    if (!dau) dau = cuoi = moi;
    else { cuoi->tiepTheo = moi; cuoi = moi; }
    ++soLuong;
    return true;
}
bool BorrowList::remove(const string& maPhieu) {
    NutPhieuMuon* nut = dau;
    NutPhieuMuon* truoc = nullptr;
    while (nut) {
        if (nut->duLieu.maPhieu == maPhieu) {
            if (!truoc) dau = nut->tiepTheo;
            else truoc->tiepTheo = nut->tiepTheo;
            if (cuoi == nut) cuoi = truoc;
            delete nut; --soLuong; return true;
        }
        truoc = nut; nut = nut->tiepTheo;
    }
    return false;
}
PhieuMuon* BorrowList::search(const string& maPhieu) const {
    for (NutPhieuMuon* nut = dau; nut; nut = nut->tiepTheo)
        if (nut->duLieu.maPhieu == maPhieu) return &nut->duLieu;
    return nullptr;
}
void BorrowList::printAll() const {
    cout << "Ma phieu | CMND | ISBN | Ngay muon | Han tra | Da tra | Tien phat\n";
    for (NutPhieuMuon* nut = dau; nut; nut = nut->tiepTheo) {
        const PhieuMuon& p = nut->duLieu;
        cout << p.maPhieu << " | " << p.CMND << " | " << p.ISBN << " | "
            << p.ngayMuon << " | " << p.ngayHenTra << " | "
            << (p.daTra ? "Co" : "Chua") << " | " << p.tienPhat << '\n';
    }
}
size_t BorrowList::size() const { return soLuong; }
void BorrowList::clear() {
    while (dau) { NutPhieuMuon* xoa = dau; dau = dau->tiepTheo; delete xoa; }
    cuoi = nullptr; soLuong = 0;
}
void BorrowList::toVector(vector<PhieuMuon>& ds) const {
    ds.clear();
    for (NutPhieuMuon* nut = dau; nut; nut = nut->tiepTheo) ds.push_back(nut->duLieu);
}