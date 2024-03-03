#include "Graph.hpp"
#include <cassert>
#include <fstream>
#include <iostream>
#include <queue>

/*
AdjacencyList::AdjacencyList(size_t size) {
    list = std::vector<std::forward_list<unsigned int>>(
        size, std::forward_list<unsigned int>());
    in_degrees = std::vector<unsigned int>(size, 0);
    out_degrees = std::vector<unsigned int>(size, 0);
}

const size_t AdjacencyList::size() { return list.size(); }

void AdjacencyList::add_neighbor(unsigned int current, unsigned int neighbor) {
    list.at(current).push_front(neighbor);
    in_degrees.at(neighbor)++;
    out_degrees.at(current)++;
}

const std::vector<unsigned int> &AdjacencyList::get_in_degrees() {
    return in_degrees;
}

const std::vector<unsigned int> &AdjacencyList::get_out_degrees() {
    return out_degrees;
}

const std::forward_list<unsigned int> &
AdjacencyList::get_neighbors(unsigned int node) {
    return list.at(node);
}

AdjacencyList::~AdjacencyList() {}
*/

Graph::Graph(const std::string &filename) {
    std::ifstream istrm(filename);
    assert(istrm.is_open());

    unsigned int start, end, revenue;
    while (istrm >> start >> end >> revenue) {
        assert(start < end);
        _clients.push_back(new Client{start, end, revenue});
    }

    istrm.close();

    _adj_list = new AdjacencyList(_clients);

    // todo: AdjacencyList(_clients)
    // Initialize adjacency list
    // _adj_list = new AdjacencyList(_clients.size());
    // for (size_t i = 0; i < _adj_list->size(); i++) {
    // for (size_t j = 0; j < _adj_list->size(); j++) {
    // if (_clients.at(i)->end <= _clients.at(j)->start) {
    // _adj_list->add_neighbor(i, j);
    // }
    // }
    // }

    // Initialize sources and leaves (start and finish)
    for (size_t i = 0; i < _adj_list->size(); i++) {
        if (!_adj_list->get_in_degrees().at(i)) {
            _sources.push_front(i);
        }
        if (!_adj_list->get_out_degrees().at(i)) {
            _leaves.push_front(i);
        }
    }
}

const size_t Graph::size() {
    assert(_clients.size() == _adj_list->size());
    return _clients.size();
}

const void Graph::print_v() {
    std::cout << "PRINTING VERTICES\n";
    assert(_clients.size() == _adj_list->size());
    for (size_t i = 0; i < _clients.size(); i++) {
        std::cout << "Vertex " << i << " start: " << _clients.at(i)->start
                  << " end: " << _clients.at(i)->end
                  << " revenue: " << _clients.at(i)->revenue << "\n";
    }
    std::cout << std::endl;
}

const void Graph::print_e() {
    std::cout << "PRINTING EDGES\n";
    for (size_t i = 0; i < _adj_list->size(); i++) {
        std::cout << "Vertex " << i << " has "
                  << _adj_list->get_out_degrees().at(i)
                  << " outgoing edges and " << _adj_list->get_in_degrees().at(i)
                  << " incoming edges.\n";
        if (_adj_list->get_out_degrees().at(i)) {
            std::forward_list<unsigned int> current_neighbors =
                _adj_list->get_neighbors(i);
            std::cout << "Its neighbors are: ";
            for (auto &&edge : current_neighbors) {
                std::cout << edge << " ";
            }
            std::cout << "\n";
        }
        std::cout << std::endl;
    }
}

std::vector<unsigned int> Graph::topological_sort() {
    if (_sorted_state) {
        return _toposorted_path;
    }

    std::vector<unsigned int> in_degrees(_adj_list->get_in_degrees());

    std::queue<unsigned int, std::list<unsigned>> source_queue{_sources};
    std::vector<unsigned int> toposorted(_adj_list->size());

    size_t i = 0;
    while (!source_queue.empty()) {
        toposorted.at(i) = source_queue.front();
        source_queue.pop();
        for (auto &&current_node : _adj_list->get_neighbors(toposorted.at(i))) {
            in_degrees.at(current_node)--;
            if (!in_degrees.at(current_node)) {
                source_queue.push(current_node);
            }
        }
        i++;
    }

    _toposorted_path = toposorted;
    _sorted_state = true;
    return _toposorted_path;
}

std::tuple<unsigned int, std::vector<unsigned int>> Graph::max_revenue() {
    if (!_sorted_state) {
        topological_sort();
    }
    if (_pathed_state) {
        return std::make_tuple(_maximum_revenue, _maximum_path);
    }
    assert(_clients.size() == _adj_list->size());

    std::vector<unsigned int> f_values(_adj_list->size(), 0);
    std::vector<unsigned int> max_neighbors(_adj_list->size(), _clients.size());

    for (long i = _toposorted_path.size() - 1; i >= 0; i--) {
        unsigned int current_node = _toposorted_path.at(i);
        unsigned int max_f_value = 0;
        for (auto &&neighbor : _adj_list->get_neighbors(current_node)) {
            if (f_values.at(neighbor) > max_f_value) {
                max_f_value = f_values.at(neighbor);
                max_neighbors.at(current_node) = neighbor;
            }
        }
        f_values.at(current_node) =
            _clients.at(current_node)->revenue + max_f_value;
    }

    unsigned int high_node = _sources.front();
    for (auto &&current_source : _sources) {
        if (f_values.at(current_source) > f_values.at(high_node)) {
            high_node = current_source;
        }
    }

    do {
        _maximum_revenue += _clients.at(high_node)->revenue;
        _maximum_path.push_back(high_node);
        high_node = max_neighbors.at(high_node);
    } while (high_node < _clients.size());

    _pathed_state = true;
    return std::make_tuple(_maximum_revenue, _maximum_path);
}

Graph::~Graph() {
    for (auto &&client : _clients) {
        delete client;
    }
    delete _adj_list;
}
