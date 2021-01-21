#include "process_data_entry.h"
#include "app_context.h"

#include <cstdlib>
#include <iostream>
#include <string>

void run_partitioning(app_context& app_ctx)
{
    int dummy_tile_size_ = 3;
    geometry_partitioner partitioner = geometry_partitioner(dummy_tile_size_);
    boost::apply_visitor(partitioner, app_ctx.geometry_data_store_variant_);
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

    process_data_entry(app_ctx, filename);

    if (!app_ctx.is_ready_)
    {
        return EXIT_FAILURE;
    }

    run_partitioning(app_ctx);

    return EXIT_SUCCESS;
}
