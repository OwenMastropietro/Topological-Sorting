#include <forward_list>
#include <vector>
#include <tuple>
#include <list>

#ifndef DAG_HPP
#define DAG_HPP

class Adjacency_List
{
private:
    // Vector to store vertices and their neighbors in a forward list. [Renamed _list to _adj_list?
    std::vector<std::forward_list<unsigned int>> _adj_list;
    // Vector to store number of incoming edges for vertices/clients.
    std::vector<unsigned int> _incoming_degrees_by_vertex;
    // Vector to store number of outgoing edges for vertices/clients.
    std::vector<unsigned int> _outgoing_degrees_by_vertex;

public:
    Adjacency_List(size_t size);
    ~Adjacency_List();
    const size_t size();
    void add_neighbor(unsigned int start, unsigned int end);

    const std::vector<unsigned int>& get_incoming_degrees();
    const std::vector<unsigned int>& get_outgoing_degrees();
    const std::forward_list<unsigned int>& get_neighbors(unsigned int node);
};

struct Client
{
    // TODO: utilize id functionality;
    unsigned int id, start, end, revenue;
};

class DAG
{
private:
    // Vector to store clients accessable/identifiable by index.
    std::vector<Client *> _clients;
    // Initialize _clients vector from input file.
    void _initialize_clients(const std::string &input_file_name);
    // Adacency List to represent DAG structure. [Renamed from _adj_list].
    Adjacency_List *_DAG;
    std::list<unsigned int> _sources;
    std::forward_list<unsigned int> _leaves;

    bool _sorted_state{false};
    bool _pathed_state{false};
    // [Renamed from _adj_state].
    bool _finalized_DAG_state{false};
    
    // [Renamed from _initialize_adj_list].
    void _initialize_DAG();
    std::vector<unsigned int> _toposorted;
    std::vector<unsigned int> _maximum_path;
    unsigned int _maximum_revenue{0};

public:
    DAG(const std::string &file_name);
    ~DAG();

    const size_t size();
    const void print_v();
    const void print_e();

    std::vector<unsigned int> topological_sort();
    std::tuple<unsigned int, std::vector<unsigned int>> max_revenue();
};

#endif
