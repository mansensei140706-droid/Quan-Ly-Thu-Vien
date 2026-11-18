#ifndef MODELS_H
#define MODELS_H
#include <string>

// Du lieu sach
struct Sach {
    std::string ISBN;
    std::string tenSach;
    std::string tacGia;
    std::string theLoai;
    int namXB;
    int soLuong;
};

// Du lieu doc gia
struct DocGia {
    std::string CMND;
    std::string hoTen;
    std::string email;
    std::string soDienThoai;
    std::string loaiThe;
};

// Du lieu phieu muon
struct PhieuMuon {
    std::string maPhieu;
    std::string CMND;
    std::string ISBN;
    std::string ngayMuon;
    std::string ngayHenTra;
    std::string ngayTra;
    bool daTra;
    double tienPhat;
};

// Du lieu lich su giao dich.
struct GiaoDich {
    std::string maPhieu;
    std::string CMND;
    std::string ISBN;
    std::string loaiGiaoDich;
    std::string ngayGiaoDich;
};
#endif
