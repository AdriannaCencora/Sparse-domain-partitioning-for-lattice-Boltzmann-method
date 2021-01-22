#include "app_context.h"
#include "process_data_entry.h"
#include "geometry_data_store.h"
#include "geometry_builder.h"

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

    std::string data;
    getline(input_file, data);
    getline(input_file, data);

    if (data[0] == '3')
    {
        app_ctx.geometry_data_store_variant_ = read_3d_header(input_file);
    }
    else if (data[0] == '2')
    {
        app_ctx.geometry_data_store_variant_ = read_2d_header(input_file);
    }
    else
    {
        std::cout << "Invalid data header: unsupported dimensions number.";
        app_ctx.is_ready_ = false;
        return;
    }

    // just a helper printer - may be removed
    boost::apply_visitor(output_printer{}, app_ctx.geometry_data_store_variant_);

    read_payload(app_ctx, input_file);

    input_file.close();
}

geometry_2d_data_store read_2d_header(std::fstream& input_file)
{
    geometry_2d_data_store data_store = {};

    input_file >> data_store.width_;
    input_file >> data_store.length_;

    std::string irrelevant_data;
    getline(input_file, irrelevant_data);
    getline(input_file, irrelevant_data);

    data_store.bitset2d_ = decltype(data_store.bitset2d_)
        (data_store.length_, boost::dynamic_bitset<>(data_store.width_));

    return data_store;
}

geometry_3d_data_store read_3d_header(std::fstream& input_file)
{
    geometry_3d_data_store data_store = {};

    input_file >> data_store.width_;
    input_file >> data_store.length_;
    input_file >> data_store.height_;

    std::string irrelevant_data;
    getline(input_file, irrelevant_data);
    getline(input_file, irrelevant_data);

    data_store.bitset3d_ = decltype(data_store.bitset3d_)(data_store.height_,
            bitset2d_t(data_store.length_, boost::dynamic_bitset<>(data_store.width_)));

    return data_store;
}

void read_payload(app_context& app_ctx, std::fstream& input_file)
{
    bitset_builder builder = bitset_builder(input_file);
    boost::apply_visitor(builder, app_ctx.geometry_data_store_variant_);

    app_ctx.is_ready_ = true;
}
