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
    app_ctx.tile_sizes_collection_ = {2, 3, 4, 5, 6, 7};

    process_data_entry(app_ctx, filename);

    if (!app_ctx.is_ready_)
    {
        return EXIT_FAILURE;
    }

    run_partitioning(app_ctx);

    std::cout << "VEC SIZE: " << app_ctx.all_tiling_parameters_store_.size() << std::endl;

    for (auto& v : app_ctx.all_tiling_parameters_store_){
    std::cout << "size: " << v.tile_size_ <<  std::endl;
    std::cout << "offset x, y: " << v.offset_.first << v.offset_.second << std::endl;

    std::cout << "total_hits: " << v.total_hits_ << std::endl;
    std::cout << "total_hits ratio: " << v.total_hit_ratio_ << std::endl;
    std::cout << "num of missed hits " << v.number_of_missed_hits_ << std::endl;

    std::cout << "total_tiles_with_hits: " << v.non_empty_tiles_.size()<< std::endl;
    std::cout << "total_tiles_without_hits: " << v.empty_tiles_.size()<< std::endl;
    std::cout << "total_reminders_with_hits: " << v.non_empty_remainders_.size()<< std::endl;
    std::cout << "total_reminders_without_hits: " << v.empty_remainders_.size()<< std::endl;
   //     for (auto& tile : v.non_empty_tiles_)
   //     {
   //         std::cout << tile.second.hit_ratio_ << "  ";
   //     }
    std::cout << std::endl;
    }

    return EXIT_SUCCESS;
}
