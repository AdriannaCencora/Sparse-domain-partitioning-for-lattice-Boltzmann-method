#include "process_data_entry.h"
#include "geometry_data_store.h"
#include "app_context.h"

#include <iostream>
#include <fstream>

void process_data_entry(app_context& app_ctx, std::string filename)
{
    std::fstream input_file;
    input_file.open(filename, std::ios::in);

    if (!input_file.is_open())
    {
        std::cout << filename << " cannot be opend." << std::endl;
        app_ctx.is_ready_ = false;
        return;
    }

// read header
    std::string irrelevant_data;
    std::string dimension;
    getline(input_file, irrelevant_data);
    getline(input_file, dimension);

    if (dimension[0] == '3')
    {
        std::cout << "Debug: I am inside first iffff" <<  std::endl;

        geometry_3d_data_store geometry_3d_data_store = {};

        input_file >> geometry_3d_data_store.width_;
        input_file >> geometry_3d_data_store.length_;
        input_file >> geometry_3d_data_store.height_;

        geometry_3d_data_store.bitset3d_ = decltype(geometry_3d_data_store.bitset3d_)
            (geometry_3d_data_store.height_, bitset2d_t(geometry_3d_data_store.length_,
                                                        boost::dynamic_bitset<>(geometry_3d_data_store.width_)));
       app_ctx.geometry_data_store_variant_ = geometry_3d_data_store;
    }
    else
    {
        std::cout << "Debug: I am inside second iffff" <<  std::endl;

        geometry_2d_data_store geometry_2d_data_store= {};

        input_file >> geometry_2d_data_store.width_;
        input_file >> geometry_2d_data_store.length_;

        geometry_2d_data_store.bitset2d_ = decltype(geometry_2d_data_store.bitset2d_)
            (geometry_2d_data_store.length_, boost::dynamic_bitset<>(geometry_2d_data_store.width_));

        app_ctx.geometry_data_store_variant_ = geometry_2d_data_store;
    }

    getline(input_file, irrelevant_data);
    getline(input_file, irrelevant_data);


//    app_ctx.geometry_data_store_variant_  = process_header();

    // just a helper printer - to remove
    boost::apply_visitor(output_printer{}, app_ctx.geometry_data_store_variant_);

    //read payload
    bitset_builder builder = bitset_builder(input_file);
    boost::apply_visitor(builder, app_ctx.geometry_data_store_variant_);

    app_ctx.is_ready_ = true;

    input_file.close();
}
