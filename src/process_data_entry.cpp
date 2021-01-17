#include "process_data_entry.h"

#include <iostream>
#include <fstream>

void read_data_entry(std::string filename)
{
    std::fstream input_file;
    input_file.open(filename, std::ios::in);

    if (!input_file.is_open())
    {
        std::cout << filename << " cannot be opend." << std::endl;
        return;
    }

     std::string irrelevant_tmp;

    getline(input_file, irrelevant_tmp);
    std::cout << irrelevant_tmp << std::endl;
    getline(input_file, irrelevant_tmp);
    std::cout << irrelevant_tmp << std::endl;
    getline(input_file, irrelevant_tmp);
    std::cout << irrelevant_tmp << std::endl;
    getline(input_file, irrelevant_tmp);
    std::cout << irrelevant_tmp << std::endl;

    //TODO: values from file instead of magics
    geometry_data_store geometry_data_store(400, 400, 400);

    // TODO: Remove me
    int counter = 0;
    unsigned val;
    uint16_t tmp_size_to_remove = 400;

    for (int height = 0; height < tmp_size_to_remove; ++height)
    {
        for (int length = 0; length < tmp_size_to_remove; ++ length)
        {
            for (int width = 0; width < tmp_size_to_remove; ++width)
            {
                input_file >> val;

                if (val == 1)
                {
                    geometry_data_store.bitset3d_[height][length][width] = 1;
                }
                else
                {
                    geometry_data_store.bitset3d_[height][length][width] = 0;
                }
                counter++;
            }
        }
    }

    //TODO: Remove me
    for (int i=0; i<1; ++i)
    {
        for (int j=0; j<4; ++j)
        {
            for (int k=0; k< 400; ++k)
            {
                std::cout << geometry_data_store.bitset3d_[i][j][k] << " ";
            }

            std::cout << std::endl;
        }

        std::cout << std::endl;
    }

    std::cout << counter << std::endl;

    input_file.close();
}
