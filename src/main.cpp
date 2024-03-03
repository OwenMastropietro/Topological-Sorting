#include "Graph.hpp"
#include <cassert>
#include <fstream>
#include <iostream>

#define DEBUG false

void output_results(std::ostream &output, Graph &rental_clients) {
    output << "There are " << rental_clients.size()
           << " clients in this file\n\n";

    const auto [max_rev, max_path] = rental_clients.max_revenue();
    output << "Optimal revenue earned is " << max_rev << "\n";
    output << "Clients contributing to this optimal revenue: ";
    for (size_t i = 0; i < max_path.size(); i++) {
        output << max_path.at(i) + 1 << (i < max_path.size() - 1 ? ", " : " ");
    }
    output << std::endl;
}

int main(int argc, char const *argv[]) {
    assert(argc == 1);

    // rental_clients->print_v();
    // rental_clients->print_e();

    std::string mode("console out");

    if (mode == "console out") {
        std::cout << "enter input filename: ";
        std::string filename;
        std::cin >> filename;

        Graph rental_clients(filename);

        output_results(std::cout, rental_clients);
    } else if (mode == "test") {
        int num_files = 6;
        for (int i = 1; i <= num_files; i++) {
            std::string filename("data_" + std::to_string(i) + ".txt");

            Graph rental_clients(filename);

            std::ofstream ostrm(filename);
            assert(ostrm.is_open());

            output_results(ostrm, rental_clients);

            ostrm.close();
        }

    } else {
        std::cout << "enter input filename: ";
        std::string filename;
        std::cin >> filename;

        Graph rental_clients(filename);

        std::ofstream ostrm("output/" + filename);
        assert(ostrm.is_open());

        output_results(ostrm, rental_clients);
        ostrm.close();
    }

    return 0;
}
