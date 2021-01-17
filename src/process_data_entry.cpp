#include "app_context.h"

#include <iostream>
#include <fstream>

bool read_data_entry(std::string filename)
{
    std::fstream input_file;
    input_file.open(filename, std::ios::in);

    if (!input_file.is_open())
    {
        std::cout << filename << " cannot be opend." << std::endl;
        return false;
    }

    std::string irrelevant_data;
    getline(input_file, irrelevant_data);
    getline(input_file, irrelevant_data);

    input_file >> ctx.geometry_data_store_.width_;
    input_file >> ctx.geometry_data_store_.length_;
    input_file >> ctx.geometry_data_store_.height_;
    ctx.geometry_data_store_.resize();

    getline(input_file, irrelevant_data);
    getline(input_file, irrelevant_data);

    unsigned tmp_val;

    for (int height = 0; height < ctx.geometry_data_store_.height_; ++height)
    {
        for (int length = 0; length < ctx.geometry_data_store_.length_; ++ length)
        {
            for (int width = 0; width < ctx.geometry_data_store_.width_; ++width)
            {
                input_file >> tmp_val;

                if (tmp_val == 1)
                {
                    ctx.geometry_data_store_.bitset3d_[height][length][width] = 1;
                }
                else
                {
                    ctx.geometry_data_store_.bitset3d_[height][length][width] = 0;
                }
                counter++;
            }
        }
    }

    //TODO: Remove me
   // for (int i=0; i<1; ++i)
   // {
   //     for (int j=0; j<4; ++j)
   //     {
   //         for (int k=0; k< 400; ++k)
   //         {
   //             std::cout << ctx.geometry_data_store_.bitset3d_[i][j][k] << " ";
   //         }

   //         std::cout << std::endl;
   //     }

   //     std::cout << std::endl;
   // }

   // std::cout << counter << std::endl;

    input_file.close();
    return true;
}
