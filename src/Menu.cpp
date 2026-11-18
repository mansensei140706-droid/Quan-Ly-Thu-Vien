#include "Menu.h"
#include "CayAVL.h"
#include "Trie.h"
#include "ReaderHashTable.h"
#include "BorrowList.h"
#include "TransactionArray.h"
#include <iostream>
#include <limits>
#include <string>
#include <fstream>
#include <sstream>
#include <cctype>
#include <map>
#include <algorithm>
using namespace std;
void xuLyNhapSai();
static AVLTree cayAVL;
static BookTrie trieSach;
static ReaderHashTable bangDocGia;
static BorrowList dsPhieuMuon;
static TransactionArray dsGiaoDich;
static string nhapChuoi(const string& loiNhac)
{
    string s;
    cout << loiNhac;
    getline(cin, s);
    return s;
}

static void themSach()
{
    Sach s;
    s.ISBN = nhapChuoi("Nhap ISBN: ");
    s.tenSach = nhapChuoi("Nhap ten sach: ");
    s.tacGia = nhapChuoi("Nhap tac gia: ");
    s.theLoai = nhapChuoi("Nhap the loai: ");
    cout << "Nhap nam XB: ";  cin >> s.namXB;
    cout << "Nhap so luong: "; cin >> s.soLuong;
    xuLyNhapSai();

    if (cayAVL.insert(s))
    {
        trieSach.insert(s.tenSach);
        cout << "Them sach thanh cong.\n";
    }
    else
        cout << "ISBN da ton tai (hoac rong), da cap nhat thong tin.\n";
}

static void timSachTheoISBN()
{
    string isbn = nhapChuoi("Nhap ISBN can tim: ");
    Sach* s = cayAVL.search(isbn);
    if (s)
        cout << s->ISBN << " | " << s->tenSach << " | " << s->tacGia << " | "
        << s->theLoai << " | " << s->namXB << " | " << s->soLuong << '\n';
    else
        cout << "Khong tim thay sach.\n";
}
static void xoaSach()
{
    string isbn = nhapChuoi("Nhap ISBN can xoa: ");
    Sach* s = cayAVL.search(isbn);
    if (!s)
    {
        cout << "Khong tim thay sach.\n";
        return;
    }

    string tenSach = s->tenSach;   // copy truoc, vi sau remove con tro s khong con hop le
    cayAVL.remove(isbn);
    trieSach.remove(tenSach);
    cout << "Da xoa sach.\n";
}

static void suaSach()
{
    string isbn = nhapChuoi("Nhap ISBN can sua: ");
    Sach* s = cayAVL.search(isbn);
    if (!s)
    {
        cout << "Khong tim thay sach.\n";
        return;
    }

    cout << "(Bo trong de giu nguyen gia tri cu. ISBN khong duoc sua.)\n";
    string tenCu = s->tenSach;
    string v;

    v = nhapChuoi("Ten sach [" + s->tenSach + "]: ");
    if (!v.empty()) s->tenSach = v;

    v = nhapChuoi("Tac gia [" + s->tacGia + "]: ");
    if (!v.empty()) s->tacGia = v;

    v = nhapChuoi("The loai [" + s->theLoai + "]: ");
    if (!v.empty()) s->theLoai = v;

    v = nhapChuoi("Nam XB [" + to_string(s->namXB) + "]: ");
    if (!v.empty())
    {
        try { s->namXB = stoi(v); }
        catch (...) { cout << "Nam XB khong hop le, giu nguyen.\n"; }
    }

    v = nhapChuoi("So luong [" + to_string(s->soLuong) + "]: ");
    if (!v.empty())
    {
        try { s->soLuong = stoi(v); }
        catch (...) { cout << "So luong khong hop le, giu nguyen.\n"; }
    }

    // Neu doi ten thi cap nhat lai Trie (autocomplete)
    if (tenCu != s->tenSach)
    {
        trieSach.remove(tenCu);
        trieSach.insert(s->tenSach);
    }
    cout << "Da cap nhat sach.\n";
}
static void timTheoTienTo()
{
    string tienTo = nhapChuoi("Nhap tien to ten sach: ");
    vector<string> ketQua = trieSach.searchPrefix(tienTo);
    if (ketQua.empty())
        cout << "Khong co sach nao khop.\n";
    else
        for (const string& ten : ketQua) cout << "- " << ten << '\n';
}
static string catKhoangTrang(const string& s)
{
    size_t dau = s.find_first_not_of(" \t\r\n");
    if (dau == string::npos) return "";
    size_t cuoi = s.find_last_not_of(" \t\r\n");
    return s.substr(dau, cuoi - dau + 1);
}

static vector<string> tachTruong(const string& dong, char phanCach)
{
    vector<string> ketQua;
    string truong;
    stringstream ss(dong);
    while (getline(ss, truong, phanCach)) ketQua.push_back(truong);
    return ketQua;
}

// Dinh dang moi dong: ISBN|Ten sach|Tac gia|The loai|Nam XB|So luong
static void importSach()
{
    string duongDan = nhapChuoi("Nhap ten file can import (vd: sach.txt): ");
    ifstream tep(duongDan);
    if (!tep)
    {
        cout << "Khong mo duoc file.\n";
        return;
    }

    string dong;
    int soDong = 0, themMoi = 0, capNhat = 0, loi = 0;

    while (getline(tep, dong))
    {
        ++soDong;
        // Bo BOM UTF-8 (neu file luu tu Notepad)
        if (soDong == 1 && dong.size() >= 3 &&
            (unsigned char)dong[0] == 0xEF && (unsigned char)dong[1] == 0xBB &&
            (unsigned char)dong[2] == 0xBF)
            dong = dong.substr(3);

        dong = catKhoangTrang(dong);
        if (dong.empty()) continue;

        vector<string> t = tachTruong(dong, '|');
        if (t.size() != 6)
        {
            cout << "Dong " << soDong << ": sai dinh dang, bo qua.\n";
            ++loi;
            continue;
        }

        Sach s;
        s.ISBN = catKhoangTrang(t[0]);
        s.tenSach = catKhoangTrang(t[1]);
        s.tacGia = catKhoangTrang(t[2]);
        s.theLoai = catKhoangTrang(t[3]);

        try
        {
            s.namXB = stoi(catKhoangTrang(t[4]));
            s.soLuong = stoi(catKhoangTrang(t[5]));
        }
        catch (...)
        {
            cout << "Dong " << soDong << ": nam XB hoac so luong khong phai so, bo qua.\n";
            ++loi;
            continue;
        }

        if (s.ISBN.empty() || s.tenSach.empty())
        {
            cout << "Dong " << soDong << ": thieu ISBN hoac ten sach, bo qua.\n";
            ++loi;
            continue;
        }

        Sach* cu = cayAVL.search(s.ISBN);
        string tenCu = cu ? cu->tenSach : "";   // copy truoc khi insert

        if (cayAVL.insert(s))
        {
            trieSach.insert(s.tenSach);
            ++themMoi;
        }
        else
        {
            // ISBN da co -> AVL da ghi de, cap nhat lai Trie neu doi ten
            if (tenCu != s.tenSach)
            {
                trieSach.remove(tenCu);
                trieSach.insert(s.tenSach);
            }
            ++capNhat;
        }
    }

    cout << "Import xong: them moi " << themMoi << ", cap nhat " << capNhat
        << ", loi " << loi << ".\n";
}

static void exportSach()
{
    string duongDan = nhapChuoi("Nhap ten file can export (vd: sach_out.txt): ");
    ofstream tep(duongDan);
    if (!tep)
    {
        cout << "Khong tao duoc file.\n";
        return;
    }

    vector<Sach> ds;
    cayAVL.toVector(ds);   // da sap xep theo ISBN

    for (const Sach& s : ds)
        tep << s.ISBN << '|' << s.tenSach << '|' << s.tacGia << '|'
        << s.theLoai << '|' << s.namXB << '|' << s.soLuong << '\n';

    cout << "Da export " << ds.size() << " sach ra file " << duongDan << ".\n";
}

static void themDocGia()
{
    DocGia d;
    d.CMND = nhapChuoi("Nhap CMND: ");
    if (d.CMND.empty())
    {
        cout << "CMND khong duoc rong.\n";
        return;
    }
    if (bangDocGia.search(d.CMND))
    {
        cout << "CMND da ton tai.\n";
        return;
    }
    d.hoTen = nhapChuoi("Nhap ho ten: ");
    d.email = nhapChuoi("Nhap email: ");
    d.soDienThoai = nhapChuoi("Nhap so dien thoai: ");
    d.loaiThe = nhapChuoi("Nhap loai the (vd: Thuong, VIP): ");

    bangDocGia.insert(d);
    cout << "Them doc gia thanh cong.\n";
}

static void xoaDocGia()
{
    string cmnd = nhapChuoi("Nhap CMND can xoa: ");
    if (bangDocGia.remove(cmnd))
        cout << "Da xoa doc gia.\n";
    else
        cout << "Khong tim thay doc gia.\n";
}

static void suaDocGia()
{
    string cmnd = nhapChuoi("Nhap CMND can sua: ");
    DocGia* d = bangDocGia.search(cmnd);
    if (!d)
    {
        cout << "Khong tim thay doc gia.\n";
        return;
    }

    cout << "(Bo trong de giu nguyen. CMND khong duoc sua.)\n";
    string v;

    v = nhapChuoi("Ho ten [" + d->hoTen + "]: ");
    if (!v.empty()) d->hoTen = v;

    v = nhapChuoi("Email [" + d->email + "]: ");
    if (!v.empty()) d->email = v;

    v = nhapChuoi("So dien thoai [" + d->soDienThoai + "]: ");
    if (!v.empty()) d->soDienThoai = v;

    v = nhapChuoi("Loai the [" + d->loaiThe + "]: ");
    if (!v.empty()) d->loaiThe = v;

    cout << "Da cap nhat doc gia.\n";
}

static void timDocGia()
{
    string cmnd = nhapChuoi("Nhap CMND can tim: ");
    DocGia* d = bangDocGia.search(cmnd);
    if (d)
        cout << d->CMND << " | " << d->hoTen << " | " << d->email << " | "
        << d->soDienThoai << " | " << d->loaiThe << '\n';
    else
        cout << "Khong tim thay doc gia.\n";
}

// ===================== NGAY THANG (dd/mm/yyyy) =====================
static const double TIEN_PHAT_MOI_NGAY = 5000;
static int soPhieuTuTang = 0;

static bool laNamNhuan(int y) { return (y % 4 == 0 && y % 100 != 0) || y % 400 == 0; }

static bool docNgay(const string& s, int& d, int& m, int& y)
{
    vector<string> t = tachTruong(catKhoangTrang(s), '/');
    if (t.size() != 3) return false;
    try { d = stoi(t[0]); m = stoi(t[1]); y = stoi(t[2]); }
    catch (...) { return false; }
    if (y < 1900 || y > 2100 || m < 1 || m > 12 || d < 1) return false;
    int ngayTrongThang[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
    int toiDa = ngayTrongThang[m - 1];
    if (m == 2 && laNamNhuan(y)) toiDa = 29;
    return d <= toiDa;
}

// Doi ngay ra so ngay tuyet doi de tinh khoang cach giua 2 ngay.
static bool ngayThanhSo(const string& s, int& kq)
{
    int d, m, y;
    if (!docNgay(s, d, m, y)) return false;
    y -= (m <= 2);
    int era = (y >= 0 ? y : y - 399) / 400;
    int yoe = y - era * 400;
    int doy = (153 * (m + (m > 2 ? -3 : 9)) + 2) / 5 + d - 1;
    int doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;
    kq = era * 146097 + doe - 719468;
    return true;
}

static bool laVIP(const string& loaiThe)
{
    string u = loaiThe;
    for (size_t i = 0; i < u.size(); ++i) u[i] = (char)toupper((unsigned char)u[i]);
    return u == "VIP";
}

// ===================== MUON - TRA =====================
static void ghiGiaoDich(const string& ma, const string& cmnd, const string& isbn,
    const string& loai, const string& ngay)
{
    GiaoDich g;
    g.maPhieu = ma; g.CMND = cmnd; g.ISBN = isbn;
    g.loaiGiaoDich = loai; g.ngayGiaoDich = ngay;
    dsGiaoDich.pushBack(g);
}

static bool kiemTraRangBuoc(const string& cmnd, const string& isbn,
    const string& ngayMuon, string& lyDo)
{
    DocGia* dg = bangDocGia.search(cmnd);
    if (!dg) { lyDo = "Doc gia khong ton tai."; return false; }
    Sach* s = cayAVL.search(isbn);
    if (!s) { lyDo = "Sach khong ton tai."; return false; }
    if (s->soLuong <= 0) { lyDo = "Sach da het (so luong = 0)."; return false; }

    int ngayM;
    if (!ngayThanhSo(ngayMuon, ngayM)) { lyDo = "Ngay muon sai dinh dang dd/mm/yyyy."; return false; }

    vector<PhieuMuon> ds;
    dsPhieuMuon.toVector(ds);
    int dangMuon = 0;
    for (size_t i = 0; i < ds.size(); ++i)
    {
        const PhieuMuon& p = ds[i];
        if (p.CMND != cmnd || p.daTra) continue;
        ++dangMuon;
        int han;
        if (ngayThanhSo(p.ngayHenTra, han) && han < ngayM)
        {
            lyDo = "Doc gia dang giu sach qua han (phieu " + p.maPhieu + ").";
            return false;
        }
    }
    int toiDa = laVIP(dg->loaiThe) ? 5 : 3;
    if (dangMuon >= toiDa)
    {
        lyDo = "Da muon toi da " + to_string(toiDa) + " cuon.";
        return false;
    }
    return true;
}

static void lapPhieuMuon()
{
    string cmnd = nhapChuoi("Nhap CMND doc gia: ");
    string isbn = nhapChuoi("Nhap ISBN sach: ");
    string ngayMuon = nhapChuoi("Nhap ngay muon (dd/mm/yyyy): ");
    string ngayHen = nhapChuoi("Nhap ngay hen tra (dd/mm/yyyy): ");

    string lyDo;
    if (!kiemTraRangBuoc(cmnd, isbn, ngayMuon, lyDo))
    {
        cout << "Khong the muon: " << lyDo << '\n';
        return;
    }

    int a, b;
    if (!ngayThanhSo(ngayHen, b)) { cout << "Ngay hen tra sai dinh dang.\n"; return; }
    ngayThanhSo(ngayMuon, a);
    if (b <= a) { cout << "Ngay hen tra phai sau ngay muon.\n"; return; }

    PhieuMuon p;
    do {
        ++soPhieuTuTang;
        string so = to_string(soPhieuTuTang);
        p.maPhieu = "PM" + string(so.size() < 4 ? 4 - so.size() : 0, '0') + so;
    } while (dsPhieuMuon.search(p.maPhieu));
    p.CMND = cmnd;
    p.ISBN = isbn;
    p.ngayMuon = catKhoangTrang(ngayMuon);
    p.ngayHenTra = catKhoangTrang(ngayHen);
    p.ngayTra = "";
    p.daTra = false;
    p.tienPhat = 0;

    dsPhieuMuon.insert(p);
    cayAVL.search(isbn)->soLuong -= 1;
    ghiGiaoDich(p.maPhieu, cmnd, isbn, "MUON", p.ngayMuon);
    cout << "Lap phieu thanh cong. Ma phieu: " << p.maPhieu << '\n';
}

static void traSach()
{
    string ma = nhapChuoi("Nhap ma phieu: ");
    PhieuMuon* p = dsPhieuMuon.search(ma);
    if (!p) { cout << "Khong tim thay phieu.\n"; return; }
    if (p->daTra) { cout << "Phieu nay da tra roi.\n"; return; }

    string ngayTra = catKhoangTrang(nhapChuoi("Nhap ngay tra (dd/mm/yyyy): "));
    int nTra, nMuon, nHen;
    if (!ngayThanhSo(ngayTra, nTra)) { cout << "Ngay tra sai dinh dang.\n"; return; }
    ngayThanhSo(p->ngayMuon, nMuon);
    ngayThanhSo(p->ngayHenTra, nHen);
    if (nTra < nMuon) { cout << "Ngay tra khong the truoc ngay muon.\n"; return; }

    int soNgayTre = nTra - nHen;
    p->daTra = true;
    p->ngayTra = ngayTra;
    p->tienPhat = soNgayTre > 0 ? soNgayTre * TIEN_PHAT_MOI_NGAY : 0;

    Sach* s = cayAVL.search(p->ISBN);
    if (s) s->soLuong += 1;
    ghiGiaoDich(p->maPhieu, p->CMND, p->ISBN, "TRA", ngayTra);

    if (soNgayTre > 0)
        cout << "Tra tre " << soNgayTre << " ngay. Tien phat: " << p->tienPhat << " dong.\n";
    else
        cout << "Tra dung han. Khong bi phat.\n";
}

static void kiemTraRangBuocMuon()
{
    string cmnd = nhapChuoi("Nhap CMND doc gia: ");
    string isbn = nhapChuoi("Nhap ISBN sach: ");
    string ngay = nhapChuoi("Nhap ngay muon du kien (dd/mm/yyyy): ");
    string lyDo;
    if (kiemTraRangBuoc(cmnd, isbn, ngay, lyDo)) cout << "Du dieu kien muon sach.\n";
    else cout << "Khong du dieu kien: " << lyDo << '\n';
}

// ===================== THONG KE - BAO CAO =====================
static void thongKeTheoThang(ostream& out)
{
    vector<PhieuMuon> ds;
    dsPhieuMuon.toVector(ds);
    map<string, int> dem;
    for (size_t i = 0; i < ds.size(); ++i)
    {
        int d, m, y;
        if (!docNgay(ds[i].ngayMuon, d, m, y)) continue;
        dem[to_string(y) + "/" + (m < 10 ? "0" : "") + to_string(m)]++;
    }
    out << "--- Luot muon theo thang (yyyy/mm) ---\n";
    if (dem.empty()) out << "(Chua co du lieu)\n";
    for (map<string, int>::iterator it = dem.begin(); it != dem.end(); ++it)
        out << it->first << ": " << it->second << '\n';
}

static void thongKeTheoTheLoai(ostream& out)
{
    vector<PhieuMuon> ds;
    dsPhieuMuon.toVector(ds);
    map<string, int> dem;
    for (size_t i = 0; i < ds.size(); ++i)
    {
        Sach* s = cayAVL.search(ds[i].ISBN);
        dem[s ? s->theLoai : string("(Sach da bi xoa)")]++;
    }
    out << "--- Luot muon theo the loai ---\n";
    if (dem.empty()) out << "(Chua co du lieu)\n";
    for (map<string, int>::iterator it = dem.begin(); it != dem.end(); ++it)
        out << it->first << ": " << it->second << '\n';
}

static void thongKeTheoDocGia(ostream& out)
{
    vector<PhieuMuon> ds;
    dsPhieuMuon.toVector(ds);
    map<string, int> dem;
    for (size_t i = 0; i < ds.size(); ++i) dem[ds[i].CMND]++;
    out << "--- Luot muon theo doc gia ---\n";
    if (dem.empty()) out << "(Chua co du lieu)\n";
    for (map<string, int>::iterator it = dem.begin(); it != dem.end(); ++it)
    {
        DocGia* d = bangDocGia.search(it->first);
        out << it->first << " (" << (d ? d->hoTen : string("?")) << "): " << it->second << '\n';
    }
}

static bool soSanhTop(const pair<string, int>& a, const pair<string, int>& b)
{
    if (a.second != b.second) return a.second > b.second;
    return a.first < b.first;
}

static void top10Sach(ostream& out)
{
    vector<PhieuMuon> ds;
    dsPhieuMuon.toVector(ds);
    map<string, int> dem;
    for (size_t i = 0; i < ds.size(); ++i) dem[ds[i].ISBN]++;
    vector<pair<string, int> > v(dem.begin(), dem.end());
    sort(v.begin(), v.end(), soSanhTop);
    out << "--- Top 10 sach duoc muon nhieu nhat ---\n";
    if (v.empty()) out << "(Chua co du lieu)\n";
    for (size_t i = 0; i < v.size() && i < 10; ++i)
    {
        Sach* s = cayAVL.search(v[i].first);
        out << (i + 1) << ". " << v[i].first << " | " << (s ? s->tenSach : string("?"))
            << " | " << v[i].second << " luot\n";
    }
}

static void top10DocGia(ostream& out)
{
    vector<PhieuMuon> ds;
    dsPhieuMuon.toVector(ds);
    map<string, int> dem;
    for (size_t i = 0; i < ds.size(); ++i) dem[ds[i].CMND]++;
    vector<pair<string, int> > v(dem.begin(), dem.end());
    sort(v.begin(), v.end(), soSanhTop);
    out << "--- Top 10 doc gia muon nhieu nhat ---\n";
    if (v.empty()) out << "(Chua co du lieu)\n";
    for (size_t i = 0; i < v.size() && i < 10; ++i)
    {
        DocGia* d = bangDocGia.search(v[i].first);
        out << (i + 1) << ". " << v[i].first << " | " << (d ? d->hoTen : string("?"))
            << " | " << v[i].second << " luot\n";
    }
}

static void xuatBaoCao()
{
    string duongDan = nhapChuoi("Nhap ten file bao cao (vd: baocao.txt): ");
    ofstream tep(duongDan);
    if (!tep) { cout << "Khong tao duoc file.\n"; return; }
    tep << "========== BAO CAO THU VIEN ==========\n";
    thongKeTheoThang(tep);   tep << '\n';
    thongKeTheoTheLoai(tep); tep << '\n';
    thongKeTheoDocGia(tep);  tep << '\n';
    top10Sach(tep);          tep << '\n';
    top10DocGia(tep);
    cout << "Da xuat bao cao ra file " << duongDan << ".\n";
}

// Xoa trang thai loi va cac ky tu con lai trong bo dem nhap.
void xuLyNhapSai()
{
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}


// Nhap lua chon trong khoang cho phep.
int nhapLuaChon(int luaChonNhoNhat, int luaChonLonNhat)
{
    int luaChon;

    while (true)
    {
        cout << "Nhap lua chon: ";

        if (cin >> luaChon)
        {
            xuLyNhapSai();

            if (luaChon >= luaChonNhoNhat && luaChon <= luaChonLonNhat)
            {
                return luaChon;
            }

            cout << "Lua chon khong hop le. Vui long nhap lai.\\n";
        }
        else
        {
            xuLyNhapSai();
            cout << "Vui long nhap mot so nguyen.\\n";
        }
    }
}

// ============================================================
// MENU CHINH
// ============================================================
void hienThiMenuChinh()
{
    int luaChon;

    do
    {
        cout << "\n========== QUAN LY THU VIEN ==========\n";
        cout << "1. Quan ly Sach\n";
        cout << "2. Quan ly Doc gia\n";
        cout << "3. Muon - Tra sach\n";
        cout << "4. Thong ke - Bao cao\n";
        cout << "0. Thoat chuong trinh\n";
        cout << "======================================\n";

        luaChon = nhapLuaChon(0, 4);

        switch (luaChon)
        {
        case 1:
            menuQuanLySach();
            break;

        case 2:
            menuQuanLyDocGia();
            break;

        case 3:
            menuMuonTraSach();
            break;

        case 4:
            menuThongKeBaoCao();
            break;

        case 0:
            cout << "Da thoat chuong trinh.\\n";
            break;
        }
    } while (luaChon != 0);
}

// ============================================================
//      QUAN LY SACH
// ============================================================
void menuQuanLySach()
{
    int luaChon;

    do
    {
        cout << "\n============= QUAN LY SACH =============\n";
        cout << "1. Them sach\n";
        cout << "2. Xoa sach\n";
        cout << "3. Sua sach\n";
        cout << "4. Tim sach theo ma ISBN\n";
        cout << "5. In danh muc sach sap xep theo ISBN\n";
        cout << "6. Tim sach theo tien to ten (Autocomplete)\n";
        cout << "7. Import danh muc sach tu file van ban\n";
        cout << "8. Export danh muc sach ra file van ban\n";
        cout << "0. Quay lai menu chinh\n";
        cout << "=========================================\n";

        luaChon = nhapLuaChon(0, 8);

        switch (luaChon)
        {
        case 1:
            themSach();
            break;

        case 2:
            xoaSach();
            break;

        case 3:
            suaSach();
            break;

        case 4:
            timSachTheoISBN();
            break;

        case 5:
            cayAVL.printInorder();
            break;

        case 6:
            timTheoTienTo();            
            break;

        case 7:
            importSach();
            break;

        case 8:
            exportSach();
            break;

        case 0:
            break;
        }
    } while (luaChon != 0);
}

// ============================================================
//      QUAN LY DOC GIA
// ============================================================
void menuQuanLyDocGia()
{
    int luaChon;

    do
    {
        cout << "\n============ QUAN LY DOC GIA ============\n";
        cout << "1. Them doc gia\n";
        cout << "2. Xoa doc gia\n";
        cout << "3. Sua doc gia\n";
        cout << "4. Tim doc gia theo CMND\n";
        cout << "5. Thong ke doc gia theo loai the\n";
        cout << "0. Quay lai menu chinh\n";
        cout << "=========================================\n";

        luaChon = nhapLuaChon(0, 5);

        switch (luaChon)
        {
        case 1:
            themDocGia();
            break;

        case 2:
            xoaDocGia();
            break;

        case 3:
            suaDocGia();
            break;

        case 4:
            timDocGia();
            break;

        case 5:
            bangDocGia.countByCardType();
            break;

        case 0:
            break;
        }
    } while (luaChon != 0);
}

// ============================================================
//            MUON - TRA SACH
// ============================================================
void menuMuonTraSach()
{
    int luaChon;

    do
    {
        cout << "\n============== MUON - TRA SACH ==============\n";
        cout << "1. Lap phieu muon (Ma phieu, CMND, ISBN, ngay muon, ngay hen tra)\n";
        cout << "2. Tra sach, cap nhat trang thai va tinh tien phat qua han\n";
        cout << "3. Kiem tra rang buoc muon sach\n";
        cout << "0. Quay lai menu chinh\n";
        cout << "=============================================\n";

        luaChon = nhapLuaChon(0, 3);

        switch (luaChon)
        {
        case 1:
            lapPhieuMuon();
            break;

        case 2:
            traSach();
            break;

        case 3:
            kiemTraRangBuocMuon();
            break;

        case 0:
            break;
        }
    } while (luaChon != 0);
}

// ============================================================
//         THONG KE - BAO CAO
// ============================================================
void menuThongKeBaoCao()
{
    int luaChon;

    do
    {
        cout << "\n============= THONG KE - BAO CAO =============\n";
        cout << "1. Dem luot muon theo thang\n";
        cout << "2. Dem luot muon theo the loai\n";
        cout << "3. Dem luot muon theo doc gia\n";
        cout << "4. Top 10 sach duoc muon nhieu nhat\n";
        cout << "5. Top 10 doc gia muon nhieu nhat\n";
        cout << "6. Xuat bao cao ra file van ban\n";
        cout << "0. Quay lai menu chinh\n";
        cout << "==============================================\n";

        luaChon = nhapLuaChon(0, 6);

        switch (luaChon)
        {
        case 1:
            thongKeTheoThang(cout);
            break;

        case 2:
            thongKeTheoTheLoai(cout);
            break;

        case 3:
            thongKeTheoDocGia(cout);
            break;

        case 4:
            top10Sach(cout);
            break;

        case 5:
            top10DocGia(cout);
            break;

        case 6:
            xuatBaoCao();
            break;

        case 0:
            break;
        }
    } while (luaChon != 0);
}