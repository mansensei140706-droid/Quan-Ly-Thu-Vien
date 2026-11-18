#include "CayAVL.h"
#include <iostream>
#include <algorithm>
using namespace std;

NutAVL::NutAVL(const Sach& sach) : duLieu(sach), chieuCao(1), trai(nullptr), phai(nullptr) {}
AVLTree::AVLTree() : goc(nullptr) {}
AVLTree::~AVLTree() { clear(); }

int AVLTree::getHeight(NutAVL* nut) const { return nut == nullptr ? 0 : nut->chieuCao; }
int AVLTree::getBalance(NutAVL* nut) const {
    return nut == nullptr ? 0 : getHeight(nut->trai) - getHeight(nut->phai);
}
NutAVL* AVLTree::rotateRight(NutAVL* nut) {
    NutAVL* nutMoi = nut->trai;
    NutAVL* cayPhu = nutMoi->phai;
    nutMoi->phai = nut; nut->trai = cayPhu;
    nut->chieuCao = 1 + max(getHeight(nut->trai), getHeight(nut->phai));
    nutMoi->chieuCao = 1 + max(getHeight(nutMoi->trai), getHeight(nutMoi->phai));
    return nutMoi;
}
NutAVL* AVLTree::rotateLeft(NutAVL* nut) {
    NutAVL* nutMoi = nut->phai;
    NutAVL* cayPhu = nutMoi->trai;
    nutMoi->trai = nut; nut->phai = cayPhu;
    nut->chieuCao = 1 + max(getHeight(nut->trai), getHeight(nut->phai));
    nutMoi->chieuCao = 1 + max(getHeight(nutMoi->trai), getHeight(nutMoi->phai));
    return nutMoi;
}
NutAVL* AVLTree::insertNode(NutAVL* nut, const Sach& sach) {
    if (!nut) return new NutAVL(sach);
    if (sach.ISBN < nut->duLieu.ISBN) nut->trai = insertNode(nut->trai, sach);
    else if (sach.ISBN > nut->duLieu.ISBN) nut->phai = insertNode(nut->phai, sach);
    else { nut->duLieu = sach; return nut; }
    nut->chieuCao = 1 + max(getHeight(nut->trai), getHeight(nut->phai));
    int lech = getBalance(nut);
    if (lech > 1 && sach.ISBN < nut->trai->duLieu.ISBN) return rotateRight(nut);
    if (lech < -1 && sach.ISBN > nut->phai->duLieu.ISBN) return rotateLeft(nut);
    if (lech > 1 && sach.ISBN > nut->trai->duLieu.ISBN) {
        nut->trai = rotateLeft(nut->trai); return rotateRight(nut);
    }
    if (lech < -1 && sach.ISBN < nut->phai->duLieu.ISBN) {
        nut->phai = rotateRight(nut->phai); return rotateLeft(nut);
    }
    return nut;
}
NutAVL* AVLTree::minValueNode(NutAVL* nut) {
    while (nut && nut->trai) nut = nut->trai;
    return nut;
}
NutAVL* AVLTree::removeNode(NutAVL* nut, const string& ISBN) {
    if (!nut) return nullptr;
    if (ISBN < nut->duLieu.ISBN) nut->trai = removeNode(nut->trai, ISBN);
    else if (ISBN > nut->duLieu.ISBN) nut->phai = removeNode(nut->phai, ISBN);
    else {
        if (!nut->trai || !nut->phai) {
            NutAVL* con = nut->trai ? nut->trai : nut->phai;
            if (!con) { delete nut; return nullptr; }
            *nut = *con; delete con;
        }
        else {
            NutAVL* keTiep = minValueNode(nut->phai);
            nut->duLieu = keTiep->duLieu;
            nut->phai = removeNode(nut->phai, keTiep->duLieu.ISBN);
        }
    }
    nut->chieuCao = 1 + max(getHeight(nut->trai), getHeight(nut->phai));
    int lech = getBalance(nut);
    if (lech > 1 && getBalance(nut->trai) >= 0) return rotateRight(nut);
    if (lech > 1 && getBalance(nut->trai) < 0) {
        nut->trai = rotateLeft(nut->trai); return rotateRight(nut);
    }
    if (lech < -1 && getBalance(nut->phai) <= 0) return rotateLeft(nut);
    if (lech < -1 && getBalance(nut->phai) > 0) {
        nut->phai = rotateRight(nut->phai); return rotateLeft(nut);
    }
    return nut;
}
NutAVL* AVLTree::searchNode(NutAVL* nut, const string& ISBN) const {
    while (nut) {
        if (ISBN == nut->duLieu.ISBN) return nut;
        nut = ISBN < nut->duLieu.ISBN ? nut->trai : nut->phai;
    }
    return nullptr;
}
void AVLTree::inorderNode(NutAVL* nut) const {
    if (!nut) return;
    inorderNode(nut->trai);
    cout << nut->duLieu.ISBN << " | " << nut->duLieu.tenSach << " | "
        << nut->duLieu.tacGia << " | " << nut->duLieu.theLoai << " | "
        << nut->duLieu.namXB << " | " << nut->duLieu.soLuong << '\n';
    inorderNode(nut->phai);
}
void AVLTree::clearNode(NutAVL* nut) {
    if (!nut) return;
    clearNode(nut->trai); clearNode(nut->phai); delete nut;
}
bool AVLTree::insert(const Sach& sach) {
    if (sach.ISBN.empty()) return false;
    bool daCo = searchNode(goc, sach.ISBN) != nullptr;
    goc = insertNode(goc, sach);
    return !daCo;
}
bool AVLTree::remove(const string& ISBN) {
    if (!searchNode(goc, ISBN)) return false;
    goc = removeNode(goc, ISBN); return true;
}
Sach* AVLTree::search(const string& ISBN) const {
    NutAVL* nut = searchNode(goc, ISBN);
    return nut ? &nut->duLieu : nullptr;
}
void AVLTree::printInorder() const {
    cout << "ISBN | Ten sach | Tac gia | The loai | Nam XB | So luong\n";
    inorderNode(goc);
}
void AVLTree::clear() { clearNode(goc); goc = nullptr; }
void AVLTree::collectNode(NutAVL* nut, vector<Sach>& ds) const {
    if (!nut) return;
    collectNode(nut->trai, ds);
    ds.push_back(nut->duLieu);
    collectNode(nut->phai, ds);
}
void AVLTree::toVector(vector<Sach>& ds) const {
    ds.clear();
    collectNode(goc, ds);
}