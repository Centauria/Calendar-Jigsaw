#include <iostream>
#include <deque>
#include <string>
#include <chrono>
#include "concept.h"

std::vector<Block> cache(8 * 2 * 4 * 6);

void build_cache() {
    for (int b = 0; b < 8; b++) {
        auto block = Block{blocks[b]};
        for (int flip = 0; flip < 2; flip++) {
            for (int rot = 0; rot < 4; rot++) {
                for (int m = 0; m < blocks[b].pos.size(); m++) {
                    auto k = ((b * 2 + flip) * 4 + rot) * 6 + m;
                    cache[k] = block.anchor_inplace(m);
                }
                block.rotate_inplace(1);
            }
            block.flip_inplace();
        }
    }
}

void try_best(Board &board, const std::vector<int> &blocks_remained, const int empty_index[2],
              std::vector<Board> &result_boards) {
    if (blocks_remained.empty()) {
        if (std::all_of(result_boards.begin(), result_boards.end(), [&board](auto &b) { return board != b; })) {
            result_boards.emplace_back(board);
        }
        return;
    };
    auto ids = board.index_available();
    for (auto i: ids) {
        if (i == empty_index[0] || i == empty_index[1]) continue;
        auto yx = linear_to_coord(i);
        for (auto b: blocks_remained) {
            for (int flip = 0; flip < 2; flip++) {
                for (int rot = 0; rot < 4; rot++) {
                    for (int m = 0; m < blocks[b].pos.size(); m++) {
                        auto k = ((b * 2 + flip) * 4 + rot) * 6 + m;
                        if (board.put(cache[k], yx)) {
                            if (board(empty_index[0]).v == -1 && board(empty_index[1]).v == -1) {
                                std::vector<int> br{blocks_remained};
                                br.erase(std::remove(br.begin(), br.end(), b), br.end());
                                try_best(board, br, empty_index, result_boards);
                            }
                            board.pick(b);
                        }
                    }
                }
            }
        }
        return;
    }
}

int main() {
    int month, day;
    std::cin >> month >> day;
    auto m_name = display[month - 1], d_name = display[day + 11];
    auto m_index = month - 1, d_index = day + 11;
    int indexes[2] = {m_index, d_index};
    Board board;
    std::vector<Board> results{};
    auto t1 = std::chrono::steady_clock::now();
    build_cache();
    try_best(board,
             std::vector<int>{0, 1, 2, 3, 4, 5, 6, 7},
             indexes,
             results);
    auto t2 = std::chrono::steady_clock::now();
    for (auto b: results) {
        std::cout << std::string(b) << std::endl;
    }
    std::cout << results.size() << " possible result(s) " << std::endl;
    std::cout << "time: " << std::chrono::duration<double>(t2 - t1).count() << "s" << std::endl;
    return 0;
}
