#include "process_data_entry.h"
#include "app_context.h"

#include <cstdlib>
#include <iostream>
#include <string>

void run_partitioning()
{

}

int main(int argc, char* argv[])
{
    if (argc == 1)
    {
        std::cout << "Missing path to data." << std::endl;
        return EXIT_FAILURE;
    }

    std::string filename = argv[1];

    process_data_entry(filename);
//if (!process_data_entry(filename))
    {
        return EXIT_FAILURE;
    }

    run_partitioning();

    return EXIT_SUCCESS;
}
