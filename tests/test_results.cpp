#include "Graph.hpp"
#include <cassert>
#include <iostream>
#include <tuple>

#define INPUT_DIR "../input/"
#define OUTPUT_DIR "output/"

struct TestCase {
    std::string filename;
    std::tuple<unsigned int, std::vector<unsigned int>> expected;
};

const std::vector<TestCase> test_cases = {
    // {filename, {expected max revenue, expected max path}}
    {"data_1.txt", {220, {1, 3}}},
    {"data_2.txt", {450, {2, 3, 5}}},
    {"data_3.txt", {1765, {6, 11, 2}}},
    {"data_4.txt", {379, {21, 10, 13, 11, 5, 12}}},
    {"data_5.txt", {666, {45, 44, 9, 98, 48, 63, 6, 51, 74, 62}}},
    {"data_6.txt",
     {689957,
      {1479, 5826, 556,  4604, 7348, 3451, 9177, 2532, 9638, 4418, 9635, 2646,
       3016, 9240, 5671, 1610, 4712, 6736, 737,  7112, 9607, 5118, 1447, 8968,
       8544, 9986, 9627, 5792, 796,  6207, 9640, 6849, 5068, 9217, 3118, 6841,
       7270, 7619, 5711, 2661, 6568, 8310, 2621, 4203, 9754, 86,   3984, 4558,
       8147, 4539, 8550, 9499, 1631, 2755, 6900, 1676, 3427, 8990, 1034, 2394,
       5592, 6186, 4369, 4109, 4666, 1653, 4159, 1699, 790,  7076, 4498, 4948,
       9934, 4996, 7155, 9207, 385,  2491, 8161, 4669, 7555, 5279, 5359, 7997,
       9069, 4544, 8808, 459,  6526, 7910, 7002, 2629, 3229, 776,  9329, 7042,
       3728, 4086, 3555, 5490, 4165, 9761}}},

};

int main() {
    
    for (const auto &tc : test_cases) {
        Graph rental_clients(INPUT_DIR + tc.filename);

        const auto [max_rev, max_path] = rental_clients.max_revenue();

        assert(max_rev == std::get<0>(tc.expected));
        // assert(max_path == std::get<1>(tc.expected));
        for (size_t i = 0; i < max_path.size(); i++) {
            assert(max_path.at(i) + 1 == std::get<1>(tc.expected).at(i));
        }
    }

    std::cout << "All tests passed!" << std::endl;

    return 0;
}
