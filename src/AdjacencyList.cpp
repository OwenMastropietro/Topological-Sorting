#include "AdjacencyList.hpp"

AdjacencyList::AdjacencyList(std::vector<Client *> &clients) {
    size_t num_clients = clients.size();

    list = std::vector<std::forward_list<unsigned int>>(
        num_clients, std::forward_list<unsigned int>());

    in_degrees = std::vector<unsigned int>(num_clients, 0);
    out_degrees = std::vector<unsigned int>(num_clients, 0);

    for (size_t i = 0; i < num_clients; i++) {
        for (size_t j = 0; j < num_clients; j++) {
            if (clients.at(i)->end <= clients.at(j)->start) {
                list.at(i).push_front(j);
                in_degrees.at(j)++;
                out_degrees.at(i)++;
            }
        }
    }
}

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
