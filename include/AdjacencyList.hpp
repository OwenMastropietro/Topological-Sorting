#include <forward_list>
#include <list>
#include <string>
#include <tuple>
#include <vector>

#pragma once

struct Client {
    unsigned int start, end, revenue;
};

class AdjacencyList {
  private:
    std::vector<std::forward_list<unsigned int>> list;
    std::vector<unsigned int> in_degrees;
    std::vector<unsigned int> out_degrees;

  public:
    // todo: AdjacencyList(_clients)
    AdjacencyList(std::vector<Client *> &clients);
    AdjacencyList(size_t size);
    const size_t size();
    void add_neighbor(unsigned int start, unsigned int end);
    ~AdjacencyList();

    const std::vector<unsigned int> &get_in_degrees();
    const std::vector<unsigned int> &get_out_degrees();
    const std::forward_list<unsigned int> &get_neighbors(unsigned int node);
};
