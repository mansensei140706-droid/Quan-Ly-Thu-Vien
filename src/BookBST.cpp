#include "BookBST.h"
#include <iostream>
using namespace std;

NutBSTSach::NutBSTSach(const Sach& sach) : duLieu(sach), trai(nullptr), phai(nullptr) {}
BookBST::BookBST() : goc(nullptr) {}
BookBST::~BookBST() { clear(); }

bool BookBST::nhoHon(const Sach& a, const Sach& b) {
    if (a.tenSach != b.tenSach) return a.tenSach < b.tenSach;
    return a.ISBN < b.ISBN;
}
NutBSTSach* BookBST::insertNode(NutBSTSach* nut, const Sach& sach) {
    if (!nut) return new NutBSTSach(sach);
    if (sach.ISBN == nut->duLieu.ISBN) { nut->duLieu = sach; return nut; }
    if (nhoHon(sach, nut->duLieu)) nut->trai = insertNode(nut->trai, sach);
    else nut->phai = insertNode(nut->phai, sach);
    return nut;
}
NutBSTSach* BookBST::minNode(NutBSTSach* nut) const {
    while (nut && nut->trai) nut = nut->trai;
    return nut;
}
NutBSTSach* BookBST::removeNode(NutBSTSach* nut, const Sach& sach, bool& daXoa) {
    if (!nut) return nullptr;
    if (nhoHon(sach, nut->duLieu)) nut->trai = removeNode(nut->trai, sach, daXoa);
    else if (nhoHon(nut->duLieu, sach)) nut->phai = removeNode(nut->phai, sach, daXoa);
    else if (nut->duLieu.ISBN == sach.ISBN) {
        daXoa = true;
        if (!nut->trai) { NutBSTSach* p = nut->phai; delete nut; return p; }
        if (!nut->phai) { NutBSTSach* t = nut->trai; delete nut; return t; }
        NutBSTSach* keTiep = minNode(nut->phai);
        nut->duLieu = keTiep->duLieu;
        bool boQua = false;
        nut->phai = removeNode(nut->phai, keTiep->duLieu, boQua);
    }
    return nut;
}
NutBSTSach* BookBST::searchNode(NutBSTSach* nut, const string& tenSach) const {
    // Tim theo ten sach; neu co nhieu sach cung ten, tra ve ket qua dau tien.
    while (nut) {
        if (tenSach == nut->duLieu.tenSach) return nut;
        // Cay sap xep theo ten, sau do ISBN; tim ten chi can di theo ten.
        if (tenSach < nut->duLieu.tenSach) nut = nut->trai;
        else nut = nut->phai;
    }
    return nullptr;
}
void BookBST::inorderNode(NutBSTSach* nut) const {
    if (!nut) return;
    inorderNode(nut->trai);
    cout << nut->duLieu.tenSach << " | " << nut->duLieu.ISBN << " | "
        << nut->duLieu.tacGia << " | " << nut->duLieu.theLoai << " | "
        << nut->duLieu.namXB << " | " << nut->duLieu.soLuong << '\n';
    inorderNode(nut->phai);
}
void BookBST::clearNode(NutBSTSach* nut) {
    if (!nut) return;
    clearNode(nut->trai); clearNode(nut->phai); delete nut;
}
bool BookBST::insert(const Sach& sach) {
    if (sach.ISBN.empty() || sach.tenSach.empty()) return false;
    NutBSTSach* nut = goc;
    while (nut) {
        if (nut->duLieu.ISBN == sach.ISBN) { nut->duLieu = sach; return false; }
        nut = nhoHon(sach, nut->duLieu) ? nut->trai : nut->phai;
    }
    goc = insertNode(goc, sach); return true;
}
bool BookBST::remove(const Sach& sach) {
    bool daXoa = false; goc = removeNode(goc, sach, daXoa); return daXoa;
}
Sach* BookBST::search(const string& tenSach) const {
    NutBSTSach* nut = searchNode(goc, tenSach);
    return nut ? &nut->duLieu : nullptr;
}
void BookBST::printInorder() const {
    cout << "Ten sach | ISBN | Tac gia | The loai | Nam XB | So luong\n";
    inorderNode(goc);
}
void BookBST::clear() { clearNode(goc); goc = nullptr; }