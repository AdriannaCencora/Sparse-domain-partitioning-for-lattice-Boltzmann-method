#include "generate_data_output.h"

#include <nlohmann/json.hpp>

#include <iomanip>

using json = nlohmann::ordered_json;

void generate_data_output(tiling_parameters_store_variant_t& data_store)
{
    std::string filename = "tiling_parameters.json";
    std::fstream output_file;

    output_file.open(filename, std::ios::out | std::ios::app);

    if (!output_file.is_open())
    {
        std::cout << filename << " cannot be opened for writing." << std::endl;
        return;
    }

    file_writer file_writer{output_file};
    boost::apply_visitor(file_writer, data_store);

    output_file.close();
}

void file_writer::operator()(const tiling_parameters_store<coords_2d>& data_store)
{

    json parameters;
    parameters["tile_size"] = data_store.tile_size_;
    parameters["offset"] = {data_store.offset_.x, data_store.offset_.y};
    parameters["tiles"] = data_store.tiles_.size();
    parameters["total_hits"] = data_store.total_hits_;
    parameters["fill_ratio"] = data_store.fill_ratio_;
    parameters["common_edges"] = data_store.number_of_common_edges_;
    parameters["common_edges_ratio"] = data_store.common_edges_ratio_;
    parameters["common_vertices"] = data_store.number_of_common_vertices_;
    parameters["common_vertices_ratio"] = data_store.common_vertices_ratio_;

    output_file_ << std::setw(4) << std::setprecision(5) << parameters << "," << std::endl;
}

void file_writer::operator()(const tiling_parameters_store<coords_3d>& data_store)
{
    json parameters;
    parameters["tile_size"] = data_store.tile_size_;
    parameters["offset"] = {data_store.offset_.x, data_store.offset_.y, data_store.offset_.z};
    parameters["tiles"] = data_store.tiles_.size();
    parameters["total_hits"] = data_store.total_hits_;
    parameters["fill_ratio"] = data_store.fill_ratio_;
    parameters["common_edges"] = data_store.number_of_common_edges_;
    parameters["common_edges_ratio"] = data_store.common_edges_ratio_;
    parameters["common_vertices"] = data_store.number_of_common_vertices_;
    parameters["common_vertices_ratio"] = data_store.common_vertices_ratio_;

    output_file_ << std::setw(4) << std::setprecision(5) << parameters << "," << std::endl;
}
