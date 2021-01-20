#include "geometry_data_store.h"
#include "app_context.h"

#include <iostream>
#include <fstream>

app_context process_data_entry(std::string filename)
{
    std::fstream input_file;
    input_file.open(filename, std::ios::in);

    if (!input_file.is_open())
    {
        std::cout << filename << " cannot be opend." << std::endl;
        //return false;
    }

    geometry_data_store geometry_data_store = {};

// read header
    std::string irrelevant_data;
    std::string dimension;
    getline(input_file, irrelevant_data);
    getline(input_file, dimension);


    geometry_data_store.dimension_ = dimension[0];

    input_file >> geometry_data_store.width_;
    input_file >> geometry_data_store.length_;

    if (geometry_data_store.dimension_ == 3)
    {
        input_file >> geometry_data_store.height_;
        bitset3d_t bitset3d = decltype(bitset3d)(geometry_data_store.height_, bitset2d_t(geometry_data_store.length_, boost::dynamic_bitset<>(geometry_data_store.width_)));
        geometry_data_store.bitset_variant_ = bitset3d;

    }
    else
    {
        bitset2d_t bitset2d = decltype(bitset2d)(geometry_data_store.length_, boost::dynamic_bitset<>(geometry_data_store.width_));
        geometry_data_store.bitset_variant_ = bitset2d;
    }


    getline(input_file, irrelevant_data);
    getline(input_file, irrelevant_data);


// can return just geometry_data store not all app context


// read payload

    app_context ctx{true, geometry_data_store};


// read header
// should use boost::apply_visitor
//    unsigned input_value;

//    for (int height = 0; height < ctx.geometry_data_store_.height_; ++height)
//    {
//        for (int length = 0; length < ctx.geometry_data_store_.length_; ++ length)
//        {
//            for (int width = 0; width < ctx.geometry_data_store_.width_; ++width)
//            {
//                input_file >> input_value;
//                ctx.geometry_data_store_.bitset_variant_[height][length].set(width, input_value);
//            }
//        }
//    }
//
//    //TODO: Remove me
//    for (int i=0; i<1; ++i)
//    {
//        for (int j=0; j<4; ++j)
//        {
//            for (int k=0; k< 400; ++k)
//            {
//                std::cout << ctx.geometry_data_store_.bitset_variant_[i][j][k] << " ";
//            }
//
//            std::cout << std::endl;
//        }
//
//        std::cout << std::endl;
//    }

    input_file.close();
   // return true;
   return app_context {};
}
