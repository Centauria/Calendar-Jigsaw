//
// Created by Centauria V. CHEN on 2023/4/11.
//

#ifndef ANTPUZZLE_CONCEPT_H
#define ANTPUZZLE_CONCEPT_H

#include <vector>
#include <string>
#include "Vector2D.h"

const std::string display[] = {
        "JAN", "FEB", "MAR", "APR", "MAY", "JUN",
        "JUL", "AUG", "SEP", "OCT", "NOV", "DEC",
        "1", "2", "3", "4", "5", "6", "7",
        "8", "9", "10", "11", "12", "13", "14",
        "15", "16", "17", "18", "19", "20", "21",
        "22", "23", "24", "25", "26", "27", "28",
        "29", "30", "31",
};
const int widths[] = {6, 6, 7, 7, 7, 7, 3};

int name_to_index(const std::string &name);

Index linear_to_coord(int index);


struct Cell {
    int y = 0;
    int x = 0;
    int v = -1;
    std::string label;

    explicit operator std::string() const;
};

struct Block {
    Block() : id(-1) {};

    explicit Block(int id) : id(id) {};

    Block(int id, const std::vector<Index> &pos) : id(id), pos(pos) {};

    [[nodiscard]] Block anchor(int index) const;

    [[nodiscard]] Block rotate(int n) const;

    [[nodiscard]] Block flip(int n) const;

    Block &anchor_inplace(int index);

    Block &rotate_inplace(int n);

    Block &flip_inplace();

    int id;
    std::vector<Index> pos;
};

const Block blocks[] = {
        Block{0, std::vector<Index>{{0, 0},
                                    {0, 1},
                                    {0, 2},
                                    {1, 0},
                                    {1, 1},
                                    {1, 2}}},
        Block{1, std::vector<Index>{{0, 0},
                                    {0, 2},
                                    {1, 0},
                                    {1, 1},
                                    {1, 2}}},
        Block{2, std::vector<Index>{{0, 0},
                                    {0, 1},
                                    {0, 2},
                                    {1, 1},
                                    {1, 2}}},
        Block{3, std::vector<Index>{{0, 0},
                                    {0, 1},
                                    {0, 2},
                                    {0, 3},
                                    {1, 3}}},
        Block{4, std::vector<Index>{{0, 0},
                                    {0, 1},
                                    {0, 2},
                                    {1, 2},
                                    {2, 2}}},
        Block{5, std::vector<Index>{{0, 0},
                                    {0, 1},
                                    {0, 2},
                                    {1, 2},
                                    {1, 3}}},
        Block{6, std::vector<Index>{{0, 0},
                                    {0, 1},
                                    {1, 1},
                                    {2, 1},
                                    {2, 2}}},
        Block{7, std::vector<Index>{{0, 0},
                                    {0, 1},
                                    {0, 2},
                                    {1, 2},
                                    {0, 3}}},
};

struct Board {
    Board();

    Cell &operator()(int index);

    Cell &operator()(int y, int x);

    Cell *pointer(int y, int x);

    bool put(const Block &block, Index yx);

    void pick(int block_id);

    std::vector<int> index_available();

    explicit operator std::string();

    bool operator==(const Board &board) const;

    bool operator!=(const Board &board) const;

    bool operator<(const Board &board) const;

    std::vector<Cell> cells;
};

#endif //ANTPUZZLE_CONCEPT_H
