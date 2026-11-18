#ifndef READER_HASH_TABLE_H
#define READER_HASH_TABLE_H
#include "Models.h"
#include <string>
#include <vector>
#include <cstddef>

struct NutDocGia {
    DocGia duLieu;
    NutDocGia* tiepTheo;
    explicit NutDocGia(const DocGia& docGia);
};

class ReaderHashTable {
private:
    std::vector<NutDocGia*> bang;
    std::size_t soLuong;
    std::size_t hashKey(const std::string& CMND) const;
public:
    explicit ReaderHashTable(std::size_t kichThuoc = 101);
    ~ReaderHashTable();
    ReaderHashTable(const ReaderHashTable&) = delete;
    ReaderHashTable& operator=(const ReaderHashTable&) = delete;
    bool insert(const DocGia& docGia);
    bool remove(const std::string& CMND);
    DocGia* search(const std::string& CMND) const;
    std::size_t size() const;
    void countByCardType() const;
    void clear();
};
#endif