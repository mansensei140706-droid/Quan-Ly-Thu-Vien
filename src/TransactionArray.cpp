#include "TransactionArray.h"
#include <iostream>
using namespace std;

TransactionArray::TransactionArray() : duLieu(nullptr), soLuong(0), sucChua(0) {}
TransactionArray::~TransactionArray() { delete[] duLieu; }
void TransactionArray::moRong() {
    size_t moi = sucChua == 0 ? 4 : sucChua * 2;
    GiaoDich* mangMoi = new GiaoDich[moi];
    for (size_t i = 0; i < soLuong; ++i) mangMoi[i] = duLieu[i];
    delete[] duLieu; duLieu = mangMoi; sucChua = moi;
}
bool TransactionArray::pushBack(const GiaoDich& giaoDich) {
    if (soLuong == sucChua) moRong();
    duLieu[soLuong++] = giaoDich; return true;
}
bool TransactionArray::removeAt(size_t viTri) {
    if (viTri >= soLuong) return false;
    for (size_t i = viTri; i + 1 < soLuong; ++i) duLieu[i] = duLieu[i + 1];
    --soLuong; return true;
}
GiaoDich* TransactionArray::searchByReceipt(const string& maPhieu) const {
    for (size_t i = 0; i < soLuong; ++i)
        if (duLieu[i].maPhieu == maPhieu) return &duLieu[i];
    return nullptr;
}
GiaoDich* TransactionArray::at(size_t viTri) const {
    return viTri < soLuong ? &duLieu[viTri] : nullptr;
}
size_t TransactionArray::size() const { return soLuong; }
void TransactionArray::printAll() const {
    cout << "Ma phieu | CMND | ISBN | Loai giao dich | Ngay giao dich\n";
    for (size_t i = 0; i < soLuong; ++i)
        cout << duLieu[i].maPhieu << " | " << duLieu[i].CMND << " | "
        << duLieu[i].ISBN << " | " << duLieu[i].loaiGiaoDich
        << " | " << duLieu[i].ngayGiaoDich << '\n';
}
void TransactionArray::clear() {
    delete[] duLieu; duLieu = nullptr; soLuong = 0; sucChua = 0;
}