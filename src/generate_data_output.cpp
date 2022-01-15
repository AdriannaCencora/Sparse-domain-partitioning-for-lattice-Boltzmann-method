#include "generate_data_output.h"

void generate_data_output(tiling_parameters_store_variant_t& data_store)
{
    std::string filename = "tiling_parameters";
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
    output_file_ << "GEOMETRY 2D - TILING PARAMETERS" << std::endl;

    output_file_ << "Tile size: " << data_store.tile_size_ << std::endl;
    output_file_ << "Offset: " << data_store.offset_.x << ", " <<
        data_store.offset_.y << std::endl;
    output_file_ << "Empty tiles: " << data_store.empty_tiles_.size() << std::endl;
    output_file_ << "Non empty tiles: " << data_store.non_empty_tiles_.size() << std::endl;

    output_file_ << "Total hits: " << data_store.total_hits_ << std::endl;
    output_file_ << "Fill ratio: " << data_store.fill_ratio_ << std::endl;
    output_file_ << "Number of common edges: " << data_store.number_of_common_edges_ << std::endl;
    output_file_ << "Common edges ratio: " << data_store.common_edges_ratio_ << std::endl;
    output_file_ << "Number of common vertices " << data_store.number_of_common_vertices_ << std::endl;
    output_file_ << "Common vertices ratio: " << data_store.common_vertices_ratio_ << std::endl;

     output_file_ << std::endl << std::endl;
}

void file_writer::operator()(const tiling_parameters_store<coords_3d>& data_store)
{
    output_file_ << "GEOMETRY 3D - TILING PARAMETERS" << std::endl;

    output_file_ << "Tile size: " << data_store.tile_size_ << std::endl;
    output_file_ << "Offset xyz: " << data_store.offset_.x << ", " <<
        data_store.offset_.y<< ", "<<
        data_store.offset_.z<<  std::endl;
    output_file_ << "Empty tiles: " << data_store.empty_tiles_.size() << std::endl;
    output_file_ << "Non empty tiles: " << data_store.non_empty_tiles_.size() << std::endl;

    output_file_ << "Total hits: " << data_store.total_hits_ << std::endl;
    output_file_ << "Total hit ratio: " << data_store.fill_ratio_ << std::endl;
    output_file_ << "Number of common edges: " << data_store.number_of_common_edges_ << std::endl;
    output_file_ << "Common edges ratio: " << data_store.common_edges_ratio_ << std::endl;
    output_file_ << "Number of common vertices " << data_store.number_of_common_vertices_ << std::endl;
    output_file_ << "Common vertices ratio: " << data_store.common_vertices_ratio_ << std::endl;

     output_file_ << std::endl << std::endl;
}
