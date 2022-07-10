#include <iostream>
#include <cassert>
#include <fstream>
#include <queue>
#include "DAG.hpp"

///////////////////////////////////////////////////////////////////////////////
// ADJACENCY LIST
Adjacency_List::Adjacency_List(size_t size)
{
    _adj_list = std::vector<std::forward_list<unsigned int>>(size, std::forward_list<unsigned int>());
    _incoming_degrees_by_vertex = std::vector<unsigned int>(size, 0);
    _outgoing_degrees_by_vertex = std::vector<unsigned int>(size, 0);
}

Adjacency_List::~Adjacency_List() {}

const size_t Adjacency_List::size() { return _adj_list.size(); }

void Adjacency_List::add_neighbor(unsigned int vertex, unsigned int neighbor)
{
    _adj_list.at(vertex).push_front(neighbor);
    _outgoing_degrees_by_vertex.at(vertex)++;
    _incoming_degrees_by_vertex.at(neighbor)++;
}
const std::vector<unsigned int> &Adjacency_List::get_incoming_degrees() { return _incoming_degrees_by_vertex; }
const std::vector<unsigned int> &Adjacency_List::get_outgoing_degrees() { return _outgoing_degrees_by_vertex; }
const std::forward_list<unsigned int> &Adjacency_List::get_neighbors(unsigned int node) { return _adj_list.at(node); }
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// DAG
DAG::DAG(const std::string &input_file_name)
{
    _initialize_clients(input_file_name);

    _DAG = new Adjacency_List(_clients.size());
    assert(_clients.size() == _DAG->size());

    if (!_finalized_DAG_state)
    {
        // Populate _DAG by finding neighbors, j, of a vertex, i.
        for (size_t vertex = 0; vertex < _clients.size(); vertex++)
        {
            for (size_t potential_neighbor = 0; potential_neighbor < _clients.size(); potential_neighbor++)
            {
                if (_clients.at(vertex)->end <= _clients.at(potential_neighbor)->start)
                {
                    _DAG->add_neighbor(vertex, potential_neighbor);
                }
            }
        }

        // Account for the first and last vertices in the graph.
        for (size_t i = 0; i < _DAG->size(); i++)
        {
            // If there are no incoming edges, must be start vertex, add it to list of _sources.
            if (!_DAG->get_incoming_degrees().at(i))
            {
                _sources.push_front(i);
            }
            // If there are no outgoing edges, must be end vertex, add it to list of _leaves.
            if (!_DAG->get_outgoing_degrees().at(i))
            {
                _leaves.push_front(i);
            }
        }
        // TODO: Possibly asserts for _sources and _leaves?

        _finalized_DAG_state = true;
    }

    assert(_finalized_DAG_state);
}

void DAG::_initialize_clients(const std::string &input_file_name)
{
    // OPEN input_file
    std::ifstream input_file(input_file_name);
    assert(input_file.is_open());

    // Read three pieces of data into each vertex/client.
    unsigned int current_start, current_end, current_revenue;
    unsigned int id = 0;
    while (input_file >> current_start >> current_end >> current_revenue)
    {
        _clients.push_back(new Client{id, current_start, current_end, current_revenue});
        id++;
    }

    // CLOSE input_file
    input_file.close();
}

DAG::~DAG()
{
    for (auto &&client : _clients)
    {
        delete client;
    }
    delete _DAG;
}

const size_t DAG::size()
{
    assert(_clients.size() == _DAG->size());
    return _clients.size();
}

const void DAG::print_v()
{
    std::cout << "PRINTING VERTICES\n";
    assert(_clients.size() == _DAG->size());
    for (size_t vertex = 0; vertex < _clients.size(); vertex++)
    {
        std::cout << "Vertex " << vertex << " start: " << _clients.at(vertex)->start << " end: " << _clients.at(vertex)->end << " revenue: " << _clients.at(vertex)->revenue << "\n";
    }
    std::cout << std::endl;
}

const void DAG::print_e()
{
    std::cout << "PRINTING EDGES\n";
    for (size_t vertex = 0; vertex < _DAG->size(); vertex++)
    {
        std::cout << "Vertex " << vertex << " has " << _DAG->get_outgoing_degrees().at(vertex) << " outgoing edges and " << _DAG->get_incoming_degrees().at(vertex) << " incoming edges.\n";
        if (_DAG->get_outgoing_degrees().at(vertex))
        {
            std::forward_list<unsigned int> current_neighbors = _DAG->get_neighbors(vertex);
            std::cout << "Its neighbors are: ";
            for (auto &&edge : current_neighbors)
            {
                std::cout << edge << " ";
            }
            std::cout << "\n";
        }
        std::cout << std::endl;
    }
}

std::vector<unsigned int> DAG::topological_sort()
{
    // if (_sorted_state)
    // {
    //     return _toposorted;
    // }

    // Vector of input degrees for each vertex/client.
    std::vector<unsigned int> incoming_degree(_DAG->get_incoming_degrees());

    // Queue of vertices/clients to represent a subset.
    // - Manually set base container for queue to std::list in order to conveniently
    //   initialize a queue to the values of an existing std::list.
    // - https://iq.opengenus.org/different-ways-to-initialize-queue-cpp/
    std::queue<unsigned int, std::list<unsigned>> source_queue{_sources}; // B)

    // Vector to store optimal subset of vertices/clients.
    std::vector<unsigned int> toposorted(_DAG->size());

    // Go through the queue/possible subset, ...
    size_t i = 0;
    while (!source_queue.empty())
    {
        toposorted.at(i) = source_queue.front();
        source_queue.pop();

        // Remove source and decrement the neighbor count for affected vertices/clients.
        for (auto &&vertex : _DAG->get_neighbors(toposorted.at(i)))
        {
            incoming_degree.at(vertex)--;
            if (!incoming_degree.at(vertex))
            {
                source_queue.push(vertex);
            }
        }
        i++;
    }

    _toposorted = toposorted;
    _sorted_state = true;
    return _toposorted;
}

std::tuple<unsigned int, std::vector<unsigned int>> DAG::max_revenue()
{
    if (!_sorted_state)
    {
        topological_sort();
    }
    if (_pathed_state)
    {
        return std::make_tuple(_maximum_revenue, _maximum_path);
    }
    assert(_clients.size() == _DAG->size());

    // Vector of f_values for vertices/clients in _DAG.
    std::vector<unsigned int> f_values(_DAG->size(), 0);
    // ...
    std::vector<unsigned int> max_neighbors(_DAG->size(), _clients.size());

    // Find f-values for topologically sorted vertices/clients and their neighbors.
    // - For larger sample datasets, will need adjust i's datatype.
    for (int i = _toposorted.size() - 1; i >= 0; i--)
    {
        unsigned int current_node = _toposorted.at(i);
        unsigned int locally_max_f_value = 0;
        for (auto &&neighbor : _DAG->get_neighbors(current_node))
        {
            if (f_values.at(neighbor) > locally_max_f_value)
            {
                locally_max_f_value = f_values.at(neighbor);
                max_neighbors.at(current_node) = neighbor;
            }
        }
        f_values.at(current_node) = _clients.at(current_node)->revenue + locally_max_f_value;
    }

    // ...
    unsigned int high_node = _sources.front();
    for (auto &&current_source : _sources)
    {
        if (f_values.at(current_source) > f_values.at(high_node))
        {
            high_node = current_source;
        }
    }

    // ...
    do
    {
        _maximum_revenue += _clients.at(high_node)->revenue;
        _maximum_path.push_back(high_node);
        high_node = max_neighbors.at(high_node);
    } while (high_node < _clients.size());

    _pathed_state = true;
    return std::make_tuple(_maximum_revenue, _maximum_path);
}
///////////////////////////////////////////////////////////////////////////////