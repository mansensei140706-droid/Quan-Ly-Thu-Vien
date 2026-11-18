#ifndef AVL_TREE_H
#define AVL_TREE_H
#include "Models.h"
#include <string>
#include <vector>

struct NutAVL {
    Sach duLieu;
    int chieuCao;
    NutAVL* trai;
    NutAVL* phai;
    explicit NutAVL(const Sach& sach);
};

class AVLTree {
private:
    NutAVL* goc;
    int getHeight(NutAVL* nut) const;
    int getBalance(NutAVL* nut) const;
    NutAVL* rotateRight(NutAVL* nut);
    NutAVL* rotateLeft(NutAVL* nut);
    NutAVL* insertNode(NutAVL* nut, const Sach& sach);
    NutAVL* removeNode(NutAVL* nut, const std::string& ISBN);
    NutAVL* minValueNode(NutAVL* nut);
    NutAVL* searchNode(NutAVL* nut, const std::string& ISBN) const;
    void inorderNode(NutAVL* nut) const;
    void clearNode(NutAVL* nut);
    void collectNode(NutAVL* nut, std::vector<Sach>& ds) const;
public:
    AVLTree();
    ~AVLTree();
    AVLTree(const AVLTree&) = delete;
    AVLTree& operator=(const AVLTree&) = delete;
    bool insert(const Sach& sach);
    bool remove(const std::string& ISBN);
    Sach* search(const std::string& ISBN) const;
    void printInorder() const;
    void clear();
    void toVector(std::vector<Sach>& ds) const;
};
#endif
