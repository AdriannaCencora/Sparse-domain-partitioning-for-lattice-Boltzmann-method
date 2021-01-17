#include "process_data_entry.h"

#include <cstdlib>
#include <iostream>
#include <string>

void run()
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

    if (!read_data_entry(filename))
    {
        return EXIT_FAILURE;
    }

    run();

    return EXIT_SUCCESS;
}
