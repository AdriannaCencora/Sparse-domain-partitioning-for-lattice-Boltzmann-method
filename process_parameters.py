import json
import math


output_berea = []
output_c1 = []
common_edges_ratio = []
common_vertices_ratio = []

def process_fill_ratio(input_tiles_list, output):

    tile_dict = {"tile_size" : input_tiles_list[0]["tile_size"],
                 "start_ratio" : input_tiles_list[0]["fill_ratio"],
                 "end_ratio" : input_tiles_list[-1]["fill_ratio"],
                 "min_ratio" : [],
                 "min_offset" : [],
                 "max_ratio" : [],
                 "max_offset" : [],
                 "avg_ratio" : []}


    min_ratio_tile = min(input_tiles_list, key=lambda x:x["fill_ratio"])
    tile_dict["min_ratio"] = min_ratio_tile["fill_ratio"]
    tile_dict["min_offset"] = min_ratio_tile["offset"]

    max_ratio_tile = max(input_tiles_list, key=lambda x:x["fill_ratio"])
    tile_dict["max_ratio"] = max_ratio_tile["fill_ratio"]
    tile_dict["max_offset"] = max_ratio_tile["offset"]

    avg_ratio = sum(tile["fill_ratio"] for tile in input_tiles_list) / len(input_tiles_list)
    tile_dict["avg_ratio"] = avg_ratio

    output.append(tile_dict)



for tile_size in range(2, 16):

    with open('paramas_berea_2d/tiling_parameters_' + str(tile_size) + '.json', 'r') as params:
        data = params.read()

    input_tiles_list = json.loads(data)
    process_fill_ratio(input_tiles_list, output_berea)

    with open('params_c1_2d/tiling_parameters_' + str(tile_size) + '.json', 'r') as params_c1:
        data_c1 = params_c1.read()

    input_tiles_list_c1 = json.loads(data_c1)
    process_fill_ratio(input_tiles_list_c1, output_c1)

print("size  & offset & min & offset & max & avg & offset & min & offset & max & avg \\\\")

for berea, c1 in zip(output_berea, output_c1):
    print(berea["tile_size"], "&", berea["min_offset"], "&",
          format(berea["min_ratio"], '.4f'), "&", berea["max_offset"], "&",
          format(berea["max_ratio"], '.4f'), "&", format(berea["avg_ratio"], '.4f'), "&",
          c1["min_offset"], "&",
          format(c1["min_ratio"], '.4f'), "&",
          c1["max_offset"], "&",
          format(c1["max_ratio"], '.4f'), "&",
          format(c1["avg_ratio"], '.4f'), "\\\\")
    print("\\hline")

#import mplfinance as fplt
