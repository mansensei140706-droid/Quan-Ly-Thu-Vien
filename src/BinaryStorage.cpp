#include "BinaryStorage.h"
#include <fstream>
#include <cstdint>
using namespace std;

namespace {
    bool ghiChuoi(ofstream& tep, const string& chuoi) {
        uint32_t doDai = static_cast<uint32_t>(chuoi.size());
        tep.write(reinterpret_cast<const char*>(&doDai), sizeof(doDai));
        if (doDai) tep.write(chuoi.data(), doDai);
        return static_cast<bool>(tep);
    }
    bool docChuoi(ifstream& tep, string& chuoi) {
        uint32_t doDai = 0;
        tep.read(reinterpret_cast<char*>(&doDai), sizeof(doDai));
        if (!tep || doDai > 1000000) return false;
        chuoi.assign(doDai, '\0');
        if (doDai) tep.read(&chuoi[0], doDai);
        return static_cast<bool>(tep);
    }
    template<class T> bool ghiSo(ofstream& tep, const T& giaTri) {
        tep.write(reinterpret_cast<const char*>(&giaTri), sizeof(T));
        return static_cast<bool>(tep);
    }
    template<class T> bool docSo(ifstream& tep, T& giaTri) {
        tep.read(reinterpret_cast<char*>(&giaTri), sizeof(T));
        return static_cast<bool>(tep);
    }
    bool ghiSach(ofstream& tep, const Sach& s) {
        return ghiChuoi(tep, s.ISBN) && ghiChuoi(tep, s.tenSach) &&
            ghiChuoi(tep, s.tacGia) && ghiChuoi(tep, s.theLoai) &&
            ghiSo(tep, s.namXB) && ghiSo(tep, s.soLuong);
    }
    bool docSach(ifstream& tep, Sach& s) {
        return docChuoi(tep, s.ISBN) && docChuoi(tep, s.tenSach) &&
            docChuoi(tep, s.tacGia) && docChuoi(tep, s.theLoai) &&
            docSo(tep, s.namXB) && docSo(tep, s.soLuong);
    }
    bool ghiDocGia(ofstream& tep, const DocGia& d) {
        return ghiChuoi(tep, d.CMND) && ghiChuoi(tep, d.hoTen) &&
            ghiChuoi(tep, d.email) && ghiChuoi(tep, d.soDienThoai) && ghiChuoi(tep, d.loaiThe);
    }
    bool docDocGia(ifstream& tep, DocGia& d) {
        return docChuoi(tep, d.CMND) && docChuoi(tep, d.hoTen) &&
            docChuoi(tep, d.email) && docChuoi(tep, d.soDienThoai) && docChuoi(tep, d.loaiThe);
    }
}

bool BinaryStorage::saveBooks(const string& duongDan, const vector<Sach>& danhSachSach) {
    ofstream tep(duongDan.c_str(), ios::binary);
    if (!tep) return false;
    uint32_t soLuong = static_cast<uint32_t>(danhSachSach.size());
    if (!ghiSo(tep, soLuong)) return false;
    for (const Sach& s : danhSachSach) if (!ghiSach(tep, s)) return false;
    return true;
}
bool BinaryStorage::loadBooks(const string& duongDan, vector<Sach>& danhSachSach) {
    ifstream tep(duongDan.c_str(), ios::binary);
    if (!tep) return false;
    uint32_t soLuong = 0;
    if (!docSo(tep, soLuong) || soLuong > 1000000) return false;
    vector<Sach> duLieuMoi;
    duLieuMoi.reserve(soLuong);
    for (uint32_t i = 0; i < soLuong; ++i) {
        Sach s;
        if (!docSach(tep, s)) return false;
        duLieuMoi.push_back(s);
    }
    danhSachSach.swap(duLieuMoi);
    return true;
}
bool BinaryStorage::saveReaders(const string& duongDan, const vector<DocGia>& danhSachDocGia) {
    ofstream tep(duongDan.c_str(), ios::binary);
    if (!tep) return false;
    uint32_t soLuong = static_cast<uint32_t>(danhSachDocGia.size());
    if (!ghiSo(tep, soLuong)) return false;
    for (const DocGia& d : danhSachDocGia) if (!ghiDocGia(tep, d)) return false;
    return true;
}
bool BinaryStorage::loadReaders(const string& duongDan, vector<DocGia>& danhSachDocGia) {
    ifstream tep(duongDan.c_str(), ios::binary);
    if (!tep) return false;
    uint32_t soLuong = 0;
    if (!docSo(tep, soLuong) || soLuong > 1000000) return false;
    vector<DocGia> duLieuMoi;
    duLieuMoi.reserve(soLuong);
    for (uint32_t i = 0; i < soLuong; ++i) {
        DocGia d;
        if (!docDocGia(tep, d)) return false;
        duLieuMoi.push_back(d);
    }
    danhSachDocGia.swap(duLieuMoi);
    return true;
}


namespace {
    bool ghiPhieuMuon(ofstream& tep, const PhieuMuon& p) {
        return ghiChuoi(tep, p.maPhieu) && ghiChuoi(tep, p.CMND) &&
            ghiChuoi(tep, p.ISBN) && ghiChuoi(tep, p.ngayMuon) &&
            ghiChuoi(tep, p.ngayHenTra) && ghiChuoi(tep, p.ngayTra) &&
            ghiSo(tep, p.daTra) && ghiSo(tep, p.tienPhat);
    }
    bool docPhieuMuon(ifstream& tep, PhieuMuon& p) {
        return docChuoi(tep, p.maPhieu) && docChuoi(tep, p.CMND) &&
            docChuoi(tep, p.ISBN) && docChuoi(tep, p.ngayMuon) &&
            docChuoi(tep, p.ngayHenTra) && docChuoi(tep, p.ngayTra) &&
            docSo(tep, p.daTra) && docSo(tep, p.tienPhat);
    }
    bool ghiGiaoDich(ofstream& tep, const GiaoDich& g) {
        return ghiChuoi(tep, g.maPhieu) && ghiChuoi(tep, g.CMND) &&
            ghiChuoi(tep, g.ISBN) && ghiChuoi(tep, g.loaiGiaoDich) &&
            ghiChuoi(tep, g.ngayGiaoDich);
    }
    bool docGiaoDich(ifstream& tep, GiaoDich& g) {
        return docChuoi(tep, g.maPhieu) && docChuoi(tep, g.CMND) &&
            docChuoi(tep, g.ISBN) && docChuoi(tep, g.loaiGiaoDich) &&
            docChuoi(tep, g.ngayGiaoDich);
    }
}

bool BinaryStorage::saveLoans(const string& duongDan, const vector<PhieuMuon>& danhSachPhieu) {
    ofstream tep(duongDan.c_str(), ios::binary);
    if (!tep) return false;
    uint32_t soLuong = static_cast<uint32_t>(danhSachPhieu.size());
    if (!ghiSo(tep, soLuong)) return false;
    for (const PhieuMuon& p : danhSachPhieu)
        if (!ghiPhieuMuon(tep, p)) return false;
    return true;
}

bool BinaryStorage::loadLoans(const string& duongDan, vector<PhieuMuon>& danhSachPhieu) {
    ifstream tep(duongDan.c_str(), ios::binary);
    if (!tep) return false;
    uint32_t soLuong = 0;
    if (!docSo(tep, soLuong) || soLuong > 1000000) return false;
    vector<PhieuMuon> duLieuMoi;
    duLieuMoi.reserve(soLuong);
    for (uint32_t i = 0; i < soLuong; ++i) {
        PhieuMuon p;
        if (!docPhieuMuon(tep, p)) return false;
        duLieuMoi.push_back(p);
    }
    danhSachPhieu.swap(duLieuMoi);
    return true;
}

bool BinaryStorage::saveTransactions(const string& duongDan, const vector<GiaoDich>& danhSachGiaoDich) {
    ofstream tep(duongDan.c_str(), ios::binary);
    if (!tep) return false;
    uint32_t soLuong = static_cast<uint32_t>(danhSachGiaoDich.size());
    if (!ghiSo(tep, soLuong)) return false;
    for (const GiaoDich& g : danhSachGiaoDich)
        if (!ghiGiaoDich(tep, g)) return false;
    return true;
}

bool BinaryStorage::loadTransactions(const string& duongDan, vector<GiaoDich>& danhSachGiaoDich) {
    ifstream tep(duongDan.c_str(), ios::binary);
    if (!tep) return false;
    uint32_t soLuong = 0;
    if (!docSo(tep, soLuong) || soLuong > 1000000) return false;
    vector<GiaoDich> duLieuMoi;
    duLieuMoi.reserve(soLuong);
    for (uint32_t i = 0; i < soLuong; ++i) {
        GiaoDich g;
        if (!docGiaoDich(tep, g)) return false;
        duLieuMoi.push_back(g);
    }
    danhSachGiaoDich.swap(duLieuMoi);
    return true;
}