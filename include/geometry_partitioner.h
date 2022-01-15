#pragma once

#include "geometry_data_store.h"
#include "tile.h"
#include "helpers.h"

struct geometry_partitioner : public boost::static_visitor<>
{
    geometry_partitioner(const std::size_t tile_size)
    : tile_size_(tile_size)
    {}

    void operator()(const geometry_2d_data_store& geometry);
    void operator()(const geometry_3d_data_store& geometry);

private:
    const std::size_t tile_size_;
    tiling_parameters_store_variant_t tiling_store_variant_;
};


tile<coords_2d>& get_tile(tiling_parameters_store<coords_2d>& store,
                          const coords_2d current_coord);

tile<coords_3d>& get_tile(tiling_parameters_store<coords_3d>& store,
                          const coords_3d current_coord);

void prepare_tiles(const geometry_2d_data_store& geometry,
                   tiling_parameters_store<coords_2d>& store);

void prepare_tiles(const geometry_3d_data_store& geometry,
                   tiling_parameters_store<coords_3d>& store);

template <typename CoordsType>
void process_tiles(tiling_parameters_store<CoordsType>& store,
                   const std::size_t tile_area)
{
    //TODO: filtering empty tiles and calculate some stats, extract to separate function
    auto tile_it = store.non_empty_tiles_.begin();
    while (tile_it != store.non_empty_tiles_.end())
    {
        if (tile_it->second.number_of_hits_ == 0)
        {
            store.empty_tiles_[tile_it->first] = tile_it->second;
            tile_it = store.non_empty_tiles_.erase(tile_it);
        }
        else
        {
            tile_it->second.fill_ratio_ =
                static_cast<float>(tile_it->second.number_of_hits_) / tile_area;
            store.total_hits_ += tile_it->second.number_of_hits_;
            ++tile_it;
        }
    }

    collect_tiling_parameters(store);
}

template <typename GeometryDataStore, typename CoordsType>
tiling_parameters_store<CoordsType> apply_tiling(GeometryDataStore& geometry,
                                                 CoordsType& offset,
                                                 const std::size_t tile_size)
{
    tiling_parameters_store<CoordsType> store = {};
    store.offset_ = offset;
    store.tile_size_ = tile_size;

    prepare_tiles(geometry, store);

    std::size_t tile_area = calculate_tile_area(tile_size, geometry.dimension_);

    process_tiles(store, tile_area);

    return store;
}
