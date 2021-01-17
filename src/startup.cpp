#include "process_data_entry.h"

#include <cstdlib>
#include <iostream>


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

    read_data_entry(argv[1]);
    run();


    return EXIT_SUCCESS;
}
