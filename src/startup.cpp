#include "app_context.h"
#include "process_data_entry.h"
#include "geometry_partitioner.h"

#include <cstdlib>
#include <iostream>
#include <string>

#include <boost/program_options.hpp>

void run_partitioning(const app_context& app_ctx)
{
    for (const auto& tile_size : app_ctx.tile_sizes_collection_)
    {
        std::cout << tile_size << " " << std::endl;
        geometry_partitioner partitioner{tile_size};
        boost::apply_visitor(partitioner, app_ctx.geometry_data_store_variant_);
    }
}

int main(int argc, char* argv[])
{
    std::string filename;
    app_context app_ctx = {};

    namespace po = boost::program_options;
    try
    {
        po::options_description options_desc("Options");
        options_desc.add_options()
            ("help, h", "Help")
            ("input-file", po::value<std::string>(&filename)->required(), "Path  to input file")
            ("tile-size", po::value<std::vector<std::size_t>>(
                &app_ctx.tile_sizes_collection_)->multitoken()->required(), "Tile sizes collection");

        po::variables_map options_map;
        po::store(po::parse_command_line(argc, argv, options_desc), options_map);
        po::notify(options_map);

        if (options_map.count("help"))
        {
            std::cout << options_desc << std::endl;
            return EXIT_SUCCESS;
        }
    }
    catch (const po::error &ex)
    {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }

    process_data_entry(app_ctx, filename);

    if (!app_ctx.is_ready_)
    {
        return EXIT_FAILURE;
    }

    run_partitioning(app_ctx);

    return EXIT_SUCCESS;
}
