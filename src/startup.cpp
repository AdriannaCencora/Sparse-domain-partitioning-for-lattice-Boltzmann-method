#include "app_context.h"
#include "process_data_entry.h"
#include "geometry_partitioner.h"

#include <cstdlib>
#include <iostream>
#include <string>

void run_partitioning(app_context& app_ctx)
{
    for (const auto& tile_size : app_ctx.tile_sizes_collection_)
    {
        geometry_partitioner partitioner{tile_size, app_ctx.all_tiling_parameters_store_};
        boost::apply_visitor(partitioner, app_ctx.geometry_data_store_variant_);
    }
}

int main(int argc, char* argv[])
{
    if (argc == 1)
    {
        std::cout << "Missing path to data." << std::endl;
        return EXIT_FAILURE;
    }

    std::string filename = argv[1];

    app_context app_ctx = {};

    //TODO: Find prettier solution for preparing configuration:
    //read from file / move to function
//    app_ctx.tile_sizes_collection_ = {2, 3, 4, 5, 6, 7, 8, 9, 10};

    app_ctx.tile_sizes_collection_ = {3};
    process_data_entry(app_ctx, filename);

    if (!app_ctx.is_ready_)
    {
        return EXIT_FAILURE;
    }

    run_partitioning(app_ctx);

    std::cout << "VEC SIZE: " << app_ctx.all_tiling_parameters_store_.size() << std::endl;

    for (auto& v : app_ctx.all_tiling_parameters_store_){
    std::cout << "offset" << std::endl;
    std::cout << "x, y: " << v.offset_.first << v.offset_.second << std::endl;
    std::cout << "total_hits: " << v.total_hits_ << std::endl;
    std::cout << "total_tiles_with_hits: " << v.total_tiles_with_hits_<< std::endl;

    }

    return EXIT_SUCCESS;
}
