#ifndef TRANSACTION_ARRAY_H
#define TRANSACTION_ARRAY_H
#include "Models.h"
#include <string>
#include <cstddef>

class TransactionArray {
private:
    GiaoDich* duLieu;
    std::size_t soLuong;
    std::size_t sucChua;
    void moRong();
public:
    TransactionArray();
    ~TransactionArray();
    TransactionArray(const TransactionArray&) = delete;
    TransactionArray& operator=(const TransactionArray&) = delete;
    bool pushBack(const GiaoDich& giaoDich);
    bool removeAt(std::size_t viTri);
    GiaoDich* searchByReceipt(const std::string& maPhieu) const;
    GiaoDich* at(std::size_t viTri) const;
    std::size_t size() const;
    void printAll() const;
    void clear();
};
#endif