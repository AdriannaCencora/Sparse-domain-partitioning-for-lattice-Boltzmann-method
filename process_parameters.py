import json
import math


fill_ratio = []
common_edges_ratio = []
common_vertices_ratio = []

for tile_size in range(2, 16):

    with open('paramas_berea_2d/tiling_parameters_' + str(tile_size) + '.json', 'r') as params:
        data = params.read()

    tiles_list = json.loads(data)

    ratio_sum = 0
    common_edges_sum = 0
    common_vertices_sum = 0

    tile_dict = {"tile_size" : tile_size,
                 "start_ratio" : tiles_list[0]["fill_ratio"],
                 "end_ratio" : tiles_list[-1]["fill_ratio"],
                 "min_ratio" : [],
                 "min_offset" : [],
                 "max_ratio" : [],
                 "max_offset" : []}


    min_ratio_tile = min(tiles_list, key=lambda x:x["fill_ratio"])
    tile_dict["min_ratio"].append(min_ratio_tile["fill_ratio"])
    tile_dict["min_offset"].append(min_ratio_tile["offset"])

    max_ratio_tile = max(tiles_list, key=lambda x:x["fill_ratio"])
    tile_dict["max_ratio"].append(max_ratio_tile["fill_ratio"])
    tile_dict["max_offset"].append(max_ratio_tile["offset"])

    fill_ratio.append(tile_dict)

#    for tile in tiles_list:
#        ratio_sum += tile['fill_ratio']
#        common_edges_sum += tile['common_edges_ratio']
#        common_vertices_sum += tile['common_vertices_ratio']
#
#    fill_ratio.append(ratio_sum / len(tiles_list))
#    common_edges_ratio.append(common_edges_sum / len(tiles_list))
#    common_vertices_ratio.append(common_vertices_sum / len(tiles_list))
#
#print("Fill ratio: ")
#for ratio in fill_ratio:
#    print(format(ratio, '.5f'))
#print("Common edges ratio: ")
#for ratio in common_edges_ratio:
#    print(format(ratio, '5f'))
#print("Common vertices ratio: ")
#for ratio in common_vertices_ratio:
#    print(format(ratio, '.5f'))

for ratio in fill_ratio:
    for key, values in ratio.items():
        print(key, " : ", values)
