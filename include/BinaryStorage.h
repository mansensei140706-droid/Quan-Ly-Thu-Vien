#ifndef BINARY_STORAGE_H
#define BINARY_STORAGE_H
#include "Models.h"
#include <string>
#include <vector>

// Doc/ghi sach va doc gia bang file nhi phan.
class BinaryStorage {
public:
    static bool saveBooks(const std::string& duongDan, const std::vector<Sach>& danhSachSach);
    static bool loadBooks(const std::string& duongDan, std::vector<Sach>& danhSachSach);
    static bool saveReaders(const std::string& duongDan, const std::vector<DocGia>& danhSachDocGia);
    static bool loadReaders(const std::string& duongDan, std::vector<DocGia>& danhSachDocGia);
    static bool saveLoans(const std::string& duongDan, const std::vector<PhieuMuon>& danhSachPhieu);
    static bool loadLoans(const std::string& duongDan, std::vector<PhieuMuon>& danhSachPhieu);
    static bool saveTransactions(const std::string& duongDan, const std::vector<GiaoDich>& danhSachGiaoDich);
    static bool loadTransactions(const std::string& duongDan, std::vector<GiaoDich>& danhSachGiaoDich);
};
#endif
