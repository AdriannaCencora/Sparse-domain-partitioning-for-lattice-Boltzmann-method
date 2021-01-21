#pragma once

#include "app_context.h"

#include <fstream>
#include <string>

void process_data_entry(app_context& app_ctx, std::string filename);

geometry_2d_data_store read_2d_header(std::fstream& input_file);
geometry_3d_data_store read_3d_header(std::fstream& input_file);

void read_payload(app_context& app_ctx, std::fstream& input_file);
