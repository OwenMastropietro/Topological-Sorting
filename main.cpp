#include <iostream>
#include <cassert>
#include <fstream>
#include "DAG.hpp"

// Create Directed Acyclic Graph...
// - TODO: Move "rental_clients" functionality out we can create(DAG), toposort it, provide output...
//         ... aka pull out toposort functionality from DAG
// - TODO: Look back at unique_ptr to drop explicit "new" and "delete."
// DAG *rental_clients = new DAG(chosem_sample_data);

int valid(std::string output_mode)
{
    return output_mode == "s" || output_mode == "f" || output_mode == "q";
}

void output_to_screen(DAG *rental_clients)
{
    // Testing...
    // rental_clients->print_v();
    // rental_clients->print_e();

    std::cout << "\nThere are " << rental_clients->size() << " clients in this file\n\n";

    const auto [max_rev, max_path] = rental_clients->max_revenue();
    std::cout << "Optimal revenue earned is " << max_rev << "\n";
    std::cout << "Clients contributing to this optimal revenue: ";
    for (size_t i = 0; i < max_path.size(); i++)
    {
        std::cout << max_path.at(i) + 1 << (i < max_path.size() - 1 ? ", " : " ");
    }
    std::cout << std::endl;

    delete rental_clients;
}

void output_to_file(DAG *rental_clients, const std::string &input_file_id)
{
    // Testing...
    // rental_clients->print_v();
    // rental_clients->print_e();

    // Changing output stream to specified output files...
    std::string output_path = "output-data/sample" + input_file_id + ".txt";
    std::ofstream output_file(output_path);
    assert(output_file.is_open());

    output_file << "There are " << rental_clients->size() << " clients in this file\n\n";

    const auto [max_rev, max_path] = rental_clients->max_revenue();
    output_file << "Optimal revenue earned is " << max_rev << "\n";
    output_file << "Clients contributing to this optimal revenue: ";
    for (size_t i = 0; i < max_path.size(); i++)
    {
        output_file << max_path.at(i) + 1 << (i < max_path.size() - 1 ? ", " : " ");
    }
    output_file << std::endl;

    delete rental_clients;

    output_file.close();

    std::cout << "\nSee " << output_path << " for output." << std::endl;
}

int main(int argc, char const *argv[])
{
    std::string output_mode;
    do
    {
        std::cout << "\n/////////////////////////////////////" << std::endl;
        std::cout << "Enter 's' to output to screen." << std::endl;
        std::cout << "Enter 'f' to output to files in output-data." << std::endl;
        std::cout << "\nPlease select a location for output: ";
        std::cin >> output_mode;
        assert(valid(output_mode));

        std::string input_file_id;
        std::cout << "Enter the file id (01-06) to read data: ";
        std::cin >> input_file_id;
        std::cout << "-------------------------------------" << std::endl;
        std::string chosem_sample_data = "input-data/sample" + input_file_id + ".txt";

        if (output_mode == "s")
        {
            output_to_screen(new DAG(chosem_sample_data));
        }
        else if (output_mode == "f")
        {
            output_to_file(new DAG(chosem_sample_data), input_file_id);
        }

    } while (valid(output_mode));

    return 0;
}
