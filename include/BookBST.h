#ifndef BOOK_BST_H
#define BOOK_BST_H
#include "Models.h"
#include <string>

struct NutBSTSach {
    Sach duLieu;
    NutBSTSach* trai;
    NutBSTSach* phai;
    explicit NutBSTSach(const Sach& sach);
};

class BookBST {
private:
    NutBSTSach* goc;
    static bool nhoHon(const Sach& sachA, const Sach& sachB);
    NutBSTSach* insertNode(NutBSTSach* nut, const Sach& sach);
    NutBSTSach* removeNode(NutBSTSach* nut, const Sach& sach, bool& daXoa);
    NutBSTSach* minNode(NutBSTSach* nut) const;
    NutBSTSach* searchNode(NutBSTSach* nut, const std::string& tenSach) const;
    void inorderNode(NutBSTSach* nut) const;
    void clearNode(NutBSTSach* nut);
public:
    BookBST();
    ~BookBST();
    BookBST(const BookBST&) = delete;
    BookBST& operator=(const BookBST&) = delete;
    bool insert(const Sach& sach);
    bool remove(const Sach& sach);
    Sach* search(const std::string& tenSach) const;
    void printInorder() const;
    void clear();
};
#endif
