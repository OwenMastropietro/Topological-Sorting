#include "AdjacencyList.hpp"
#include <forward_list>
#include <list>
#include <string>
#include <tuple>
#include <vector>

#ifndef GRAPH_HPP
#define GRAPH_HPP





class Graph {
  private:
    AdjacencyList *_adj_list;

    std::vector<Client *> _clients;

    std::list<unsigned int> _sources;
    std::forward_list<unsigned int> _leaves;

    bool _sorted_state{false};
    bool _pathed_state{false};

    std::vector<unsigned int> _toposorted_path;
    std::vector<unsigned int> _maximum_path;
    unsigned int _maximum_revenue{0};

  public:
    Graph(const std::string &file_name);
    ~Graph();

    const size_t size();
    const void print_v();
    const void print_e();

    std::vector<unsigned int> topological_sort();
    std::tuple<unsigned int, std::vector<unsigned int>> max_revenue();
};

#endif
