#include "app_context.h"
#include "geometry_data_store.h"

#include <iostream>
#include <fstream>

bool process_data_entry(std::string filename)
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
    std::string dimension;
    getline(input_file, dimension);

    std::cout << "DIMENSION IS "<< dimension[0] <<std::endl;
    ctx.geometry_data_store_.dimension_ = dimension[0];

    input_file >> ctx.geometry_data_store_.width_;
    input_file >> ctx.geometry_data_store_.length_;
    input_file >> ctx.geometry_data_store_.height_;
    ctx.geometry_data_store_.resize();

    getline(input_file, irrelevant_data);
    getline(input_file, irrelevant_data);

    unsigned input_value;

    for (int height = 0; height < ctx.geometry_data_store_.height_; ++height)
    {
        for (int length = 0; length < ctx.geometry_data_store_.length_; ++ length)
        {
            for (int width = 0; width < ctx.geometry_data_store_.width_; ++width)
            {
                input_file >> input_value;
                ctx.geometry_data_store_.bitset3d_[height][length].set(width, input_value);
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
                std::cout << ctx.geometry_data_store_.bitset3d_[i][j][k] << " ";
            }

            std::cout << std::endl;
        }

        std::cout << std::endl;
    }

    input_file.close();
    return true;
}
