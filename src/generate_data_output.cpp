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

//TODO: variant is not necessary here. Refactor structures from tile.h
void file_writer::operator()(const tiling_2d_parameters_store& data_store)
{
    output_file_ << "GEOMETRY 2D - TILING PARAMETERS" << std::endl;

    output_file_ << "Tile size: " << data_store.tile_size_ << std::endl;
    output_file_ << "Offset: " << data_store.offset_.first << ", " <<
        data_store.offset_.second << std::endl;
    output_file_ << "Empty tiles: " << data_store.empty_tiles_.size() << std::endl;
    output_file_ << "Non empty tiles: " << data_store.non_empty_tiles_.size() << std::endl;

    output_file_ << "Total hits: " << data_store.total_hits_ << std::endl;
    output_file_ << "Total hit ratio: " << data_store.total_hit_ratio_ << std::endl;

    output_file_ << "Start coords : number of hits" << std::endl;
    for (const auto& el : data_store.non_empty_tiles_)
    {
        output_file_ << "(" <<  el.first.first << ", " << el.first.second << ") : " <<
             el.second.number_of_hits_ << ", ";
    }

     output_file_ << std::endl << std::endl << std::endl << std::endl;
}

void file_writer::operator()(const tiling_3d_parameters_store& data_store)
{
    output_file_ << "GEOMETRY 3D - TILING PARAMETERS" << std::endl;

    output_file_ << "Tile size: " << data_store.tile_size_ << std::endl;
    output_file_ << "Offset xyz: " << std::get<0>(data_store.offset_) << ", " <<
        std::get<1>(data_store.offset_)<< ", "<<
        std::get<2>(data_store.offset_)<<  std::endl;
    output_file_ << "Empty tiles: " << data_store.empty_tiles_.size() << std::endl;
    output_file_ << "Non empty tiles: " << data_store.non_empty_tiles_.size() << std::endl;
    output_file_ << "Empty reminders: " << data_store.empty_remainders_.size() << std::endl;
    output_file_ << "Non empty reminders: " << data_store.non_empty_remainders_.size() << std::endl;

    output_file_ << "Total hits: " << data_store.total_hits_ << std::endl;
    output_file_ << "Total hit ratio: " << data_store.total_hit_ratio_ << std::endl;
    output_file_ << "Hits in remainder (missed hits): " <<
         data_store.number_of_missed_hits_ << std::endl;

    output_file_ << "Start coords : number of hits" << std::endl;
    for (const auto& el : data_store.non_empty_tiles_)
    {
        output_file_ << "(" <<  std::get<0>(el.first) << ", " << std::get<1>(el.first) << ", " <<
           std::get<2>(el.first) << ") : " << el.second.number_of_hits_ << ", ";
    }

     output_file_ << std::endl << std::endl << std::endl << std::endl;

}
