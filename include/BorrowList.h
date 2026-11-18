#ifndef BORROW_LIST_H
#define BORROW_LIST_H
#include "Models.h"
#include <string>
#include <cstddef>
#include <vector>   

struct NutPhieuMuon {
    PhieuMuon duLieu;
    NutPhieuMuon* tiepTheo;
    explicit NutPhieuMuon(const PhieuMuon& phieuMuon);
};

class BorrowList {
private:
    NutPhieuMuon* dau;
    NutPhieuMuon* cuoi;
    std::size_t soLuong;
public:
    BorrowList();
    ~BorrowList();
    BorrowList(const BorrowList&) = delete;
    BorrowList& operator=(const BorrowList&) = delete;
    bool insert(const PhieuMuon& phieuMuon);
    bool remove(const std::string& maPhieu);
    PhieuMuon* search(const std::string& maPhieu) const;
    void printAll() const;
    std::size_t size() const;
    void clear();
    void toVector(std::vector<PhieuMuon>& ds) const;
};
#endif
