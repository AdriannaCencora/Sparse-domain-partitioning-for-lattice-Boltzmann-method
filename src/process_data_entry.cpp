#include "process_data_entry.h"
#include "geometry_data_store.h"
#include "app_context.h"

#include <iostream>
#include <fstream>

//void bitset_builder::operator()(bitset3d_t bitset_variant)
//{//    for (int height = 0; height < ctx.geometry_data_store_.height_; ++height)
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


//}

//void bitset_builder::operator()(bitset2d_t bitset_variant)
//{
//
//}

//process_data_entry should take app_ctx as ref parameter
app_context process_data_entry(std::string filename)
{
    std::fstream input_file;
    input_file.open(filename, std::ios::in);

    if (!input_file.is_open())
    {
        std::cout << filename << " cannot be opend." << std::endl;
        //return false;
    }

//    geometry_data_store geometry_data_store = {};

// read header
    std::string irrelevant_data;
    std::string dimension;
    getline(input_file, irrelevant_data);
    getline(input_file, dimension);


    app_context ctx = {};

    if (dimension[0] == '3')
    {
        std::cout << "I am inside first iffff" <<  std::endl;

        geometry_3d_data_store geometry_3d_data_store = {};

        input_file >> geometry_3d_data_store.width_;
        input_file >> geometry_3d_data_store.length_;
        input_file >> geometry_3d_data_store.height_;

        geometry_3d_data_store.bitset3d_ = decltype(geometry_3d_data_store.bitset3d_)(geometry_3d_data_store.height_, bitset2d_t(geometry_3d_data_store.length_, boost::dynamic_bitset<>(geometry_3d_data_store.width_)));
       // geometry_data_store.bitset_variant_ = bitset3d;
       ctx.geometry_data_store_variant_ = geometry_3d_data_store;
    }
    else
    {
        std::cout << "I am inside second iffff" <<  std::endl;
        geometry_2d_data_store geometry_2d_data_store= {};

        input_file >> geometry_2d_data_store.width_;
        input_file >> geometry_2d_data_store.length_;

        geometry_2d_data_store.bitset2d_ = decltype(geometry_2d_data_store.bitset2d_)(geometry_2d_data_store.length_, boost::dynamic_bitset<>(geometry_2d_data_store.width_));
//        geometry_data_store.bitset_variant_ = bitset2d;
       ctx.geometry_data_store_variant_ = geometry_2d_data_store;
    }


    getline(input_file, irrelevant_data);
    getline(input_file, irrelevant_data);

    // just a helper - to remove
    boost::apply_visitor(output_printer{}, ctx.geometry_data_store_variant_);

// can return just geometry_data store not all app context


// read payload

//    app_context ctx{true, geometry_data_store};

//    std::cout << ctx.geometry_data_store_variant_.width_ << std::endl;
//    std::cout << ctx.geometry_data_store_variant_.length_ << std::endl;
//    std::cout << ctx.geometry_data_store_variant_.height_ << std::endl;

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
   return ctx;
}

