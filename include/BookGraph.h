#ifndef BOOK_GRAPH_H
#define BOOK_GRAPH_H
#include <string>
#include <vector>
#include <cstddef>

struct NutKeSach {
    int dinhKe;
    NutKeSach* tiepTheo;
    explicit NutKeSach(int dinh);
};

class BookGraph {
private:
    std::vector<std::string> danhSachISBN;
    std::vector<NutKeSach*> danhSachKe;
    int findIndex(const std::string& ISBN) const;
    void clearEdges();
public:
    BookGraph();
    ~BookGraph();
    BookGraph(const BookGraph&) = delete;
    BookGraph& operator=(const BookGraph&) = delete;
    int addVertex(const std::string& ISBN);
    bool addEdge(const std::string& ISBN1, const std::string& ISBN2);
    void BFS(const std::string& ISBNBatDau) const;
    void DFS(const std::string& ISBNBatDau) const;
    std::size_t vertexCount() const;
    void clear();
};
#endif