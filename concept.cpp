//
// Created by Centauria V. CHEN on 2023/4/11.
//

#include "concept.h"

#include <sstream>

int name_to_index(const std::string &name) {
    try {
        auto value = std::stoi(name);
        return value + 11;
    } catch (std::invalid_argument const &e) {
        int i = 0;
        while (i < 12) {
            if (display[i] == name) return i;
            i++;
        }
        throw std::invalid_argument("name not found");
    }
}

inline int coord_to_linear(int y, int x) {
    return widths_cumsum[y] + x;
}

Index linear_to_coord(int index) {
    int i = 0, j = 0, a = 0;
    for (int width: widths) {
        if (a + width <= index) {
            j++;
            a += width;
        } else {
            i = index - a;
            break;
        }
    }
    return {j, i};
}

Index rotate(Index index, int n) {
    switch (n % 4) {
        case 0:
            return index;
        case 1:
            return Index{index.x, -index.y};
        case 2:
            return Index{-index.y, -index.x};
        case 3:
            return Index{-index.x, index.y};
    }
    throw std::runtime_error("not possible");
}

Cell::operator std::string() const {
    return label;
}

Board::Board() {
    cells = std::vector<Cell>(43);
    int index = 0;
    for (int j = 0; j < 7; j++) {
        for (int i = 0; i < widths[j]; i++) {
            cells[index].y = j;
            cells[index].x = i;
            cells[index].label = display[index];
            index++;
        }
    }
}

Cell &Board::operator()(int index) {
    return cells[index];
}

Cell &Board::operator()(int y, int x) {
    return cells[coord_to_linear(y, x)];
}

Cell *Board::pointer(int y, int x) {
    if (0 <= y && y < 7) {
        if (0 <= x && x < widths[y]) {
            return &cells[coord_to_linear(y, x)];
        }
    }
    return nullptr;
}

Board::operator std::string() {
    std::stringstream ss;
    for (int j = 0; j < 7; j++) {
        for (int i = 0; i < widths[j]; i++) {
            auto k = cells[coord_to_linear(j, i)].v;
            if (k == -1) {
                ss << "*";
            } else {
                ss << k;
            }
        }
        ss << std::endl;
    }
    return ss.str();
}

bool Board::put(const Block &block, Index yx) {
    auto result = true;
    Cell *cs[6] = {nullptr};
    for (int i = 0; i < block.pos.size(); i++) {
        auto cell_p = pointer(yx.y + block.pos[i].y, yx.x + block.pos[i].x);
        if (!cell_p || cell_p->v != -1) {
            result = false;
            break;
        }
        cs[i] = cell_p;
    }
    if (result) {
        for (int i = 0; i < block.pos.size(); i++) {
            cs[i]->v = block.id;
        }
    }
    return result;
}

void Board::pick(int block_id) {
    for (auto &c: cells) {
        if (c.v == block_id) c.v = -1;
    }
}

std::vector<int> Board::index_available() {
    std::vector<int> result;
    result.reserve(43);
    for (int i = 0; i < cells.size(); i++) {
        if (cells[i].v == -1) {
            result.emplace_back(i);
        }
    }
    return result;
}

bool Board::operator==(const Board &board) const {
    for (int i = 0; i < cells.size(); ++i) {
        if (cells[i].v != board.cells[i].v) return false;
    }
    return true;
}

bool Board::operator!=(const Board &board) const {
    for (int i = 0; i < cells.size(); ++i) {
        if (cells[i].v != board.cells[i].v) return true;
    }
    return false;
}

bool Board::operator<(const Board &board) const {
    for (int i = 0; i < cells.size(); ++i) {
        if (cells[i].v >= board.cells[i].v) return false;
    }
    return true;
}

Block Block::anchor(int index) const {
    std::vector<Index> r;
    r.reserve(pos.size());
    std::transform(pos.begin(), pos.end(), std::back_inserter(r), [index, this](auto c) {
        return c - pos[index];
    });
    return Block{id, r};
}

Block Block::rotate(int n) const {
    std::vector<Index> r;
    r.reserve(pos.size());
    std::transform(pos.begin(), pos.end(), std::back_inserter(r), [n](auto c) {
        return ::rotate(c, n);
    });
    return Block{id, r};
}

Block Block::flip(int n) const {
    if (n % 2) {
        std::vector<Index> r;
        r.reserve(pos.size());
        std::transform(pos.begin(), pos.end(), std::back_inserter(r), [](auto c) {
            return Index{-c.y, c.x};
        });
        return Block{id, r};
    }
    return {*this};
}

Block &Block::anchor_inplace(int index) {
    auto temp = Index{pos[index]};
    std::for_each(pos.begin(), pos.end(), [temp, this](auto &c) {
        c = c - temp;
    });
    return *this;
}

Block &Block::rotate_inplace(int n) {
    std::for_each(pos.begin(), pos.end(), [n](auto &c) {
        c = ::rotate(c, n);
    });
    return *this;
}

Block &Block::flip_inplace() {
    std::for_each(pos.begin(), pos.end(), [](auto &c) {
        c.y = -c.y;
    });
    return *this;
}
