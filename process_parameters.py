import json
import math
import pandas as pd
import plotly.graph_objects as go
import matplotlib.pyplot as plt
import numpy as np

output_berea_fill = []
output_c1_fill = []

output_berea_edges = []
output_c1_edges = []

output_berea_vertices = []
output_c1_vertices = []

output_berea_fill_3d = []
output_c1_fill_3d  = []

output_berea_edges_3d  = []
output_c1_edges_3d  = []

output_berea_vertices_3d  = []
output_c1_vertices_3d  = []


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


def process_edge_ratio(input_tiles_list, output):

    tile_dict = {"tile_size" : input_tiles_list[0]["tile_size"],
                 "min_ratio" : [],
                 "min_offset" : [],
                 "max_ratio" : [],
                 "max_offset" : [],
                 "avg_ratio" : []}


    min_ratio_tile = min(input_tiles_list, key=lambda x:x["common_edges_ratio"])
    tile_dict["min_ratio"] = min_ratio_tile["common_edges_ratio"]
    tile_dict["min_offset"] = min_ratio_tile["offset"]

    max_ratio_tile = max(input_tiles_list, key=lambda x:x["common_edges_ratio"])
    tile_dict["max_ratio"] = max_ratio_tile["common_edges_ratio"]
    tile_dict["max_offset"] = max_ratio_tile["offset"]

    avg_ratio = sum(tile["common_edges_ratio"] for tile in input_tiles_list) / len(input_tiles_list)
    tile_dict["avg_ratio"] = avg_ratio

    output.append(tile_dict)


def process_vertices_ratio(input_tiles_list, output):

    tile_dict = {"tile_size" : input_tiles_list[0]["tile_size"],
                 "min_ratio" : [],
                 "min_offset" : [],
                 "max_ratio" : [],
                 "max_offset" : [],
                 "avg_ratio" : []}


    min_ratio_tile = min(input_tiles_list, key=lambda x:x["common_vertices_ratio"])
    tile_dict["min_ratio"] = min_ratio_tile["common_vertices_ratio"]
    tile_dict["min_offset"] = min_ratio_tile["offset"]

    max_ratio_tile = max(input_tiles_list, key=lambda x:x["common_vertices_ratio"])
    tile_dict["max_ratio"] = max_ratio_tile["common_vertices_ratio"]
    tile_dict["max_offset"] = max_ratio_tile["offset"]

    avg_ratio = sum(tile["common_vertices_ratio"] for tile in input_tiles_list) / len(input_tiles_list)
    tile_dict["avg_ratio"] = avg_ratio

    output.append(tile_dict)


def print_json_output(output_berea, output_c1):
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

    print("#######################")

for tile_size in range(2, 16):

    with open('paramas_berea_2d/tiling_parameters_' + str(tile_size) + '.json', 'r') as params:
        data = params.read()

    input_tiles_list = json.loads(data)
    process_fill_ratio(input_tiles_list, output_berea_fill)
    process_edge_ratio(input_tiles_list, output_berea_edges)
    process_vertices_ratio(input_tiles_list, output_berea_vertices)


    with open('params_c1_2d/tiling_parameters_' + str(tile_size) + '.json', 'r') as params_c1:
        data_c1 = params_c1.read()

    input_tiles_list_c1 = json.loads(data_c1)
    process_fill_ratio(input_tiles_list_c1, output_c1_fill)
    process_edge_ratio(input_tiles_list_c1, output_c1_edges)
    process_vertices_ratio(input_tiles_list_c1, output_c1_vertices)


for tile_size in range(2, 10):
    with open("paramas_berea_3d/tiling_parameters_" + str(tile_size) + '.json', 'r') as params:
        data = params.read()

    input_tiles_list = json.loads(data)
    process_fill_ratio(input_tiles_list, output_berea_fill_3d)
    process_edge_ratio(input_tiles_list, output_berea_edges_3d)
    process_vertices_ratio(input_tiles_list, output_berea_vertices_3d)

    with open("params_c1_3d/tiling_parameters_" + str(tile_size) + '.json', 'r') as params_c1:
        data_c1 = params_c1.read()

    input_tiles_list_c1 = json.loads(data_c1)
    process_fill_ratio(input_tiles_list_c1, output_c1_fill_3d)
    process_edge_ratio(input_tiles_list_c1, output_c1_edges_3d)
    process_vertices_ratio(input_tiles_list_c1, output_c1_vertices_3d)




print("FILL RATIO")
print_json_output(output_berea_fill, output_c1_fill)

print("COMMON EDGES RATIO")
print_json_output(output_berea_edges, output_c1_edges)

print("COMMON VERTICES RATIO")
print_json_output(output_berea_vertices, output_c1_vertices)

print("FILL RATIO 3D")
print_json_output(output_berea_fill_3d, output_c1_fill_3d)

print("COMMON EDGES RATIO 3D")
print_json_output(output_berea_edges_3d, output_c1_edges_3d)

print("COMMON VERTICES RATIO 3D")
print_json_output(output_berea_vertices_3d, output_c1_vertices_3d)





df = pd.DataFrame(output_berea_fill)
df2 = pd.DataFrame(output_c1_fill)

df3 = pd.DataFrame(output_berea_fill_3d)

print(df)

#candlestick = go.Candlestick(x=df['tile_size'],
#                             open=df['start_ratio'],
#                             high=df['max_ratio'],
#                             low=df['min_ratio'],
#                             close=df['end_ratio'])
#
#candlestick2 = go.Candlestick(x=df2['tile_size'],
#                             open=df2['start_ratio'],
#                             high=df2['max_ratio'],
#                             low=df2['min_ratio'],
#                             close=df2['end_ratio'],
#                             increasing_line_color= 'blue',
#                             decreasing_line_color= 'orange')
#
#fig = go.Figure(data=[candlestick, candlestick2])
#fig.update_layout(title="Fill ratio",
#                  yaxis_title='Tile size',
#                  width=800, height=600)
#
#candlestick3 = go.Candlestick(x=df3['tile_size'],
#                             open=df3['start_ratio'],
#                             high=df3['max_ratio'],
#                             low=df3['min_ratio'],
#                             close=df3['end_ratio'])
#fig = go.Figure(data=[candlestick3])



ratio = pd.DataFrame(output_berea_fill, index=range(2,16), columns=['max_ratio', 'avg_ratio', 'min_ratio'])
#ratio = pd.DataFrame(data=output_berea_fill, index=[output_berea_fill['tile_size']], columns=['tile_size', 'min_ratio', 'avg_ratio', 'max_ratio'])
print(ratio)

#ax = ratio.plot.bar()
#plt.bar(ratio['tile_size'], ratio)
ax = ratio.plot(kind='bar', color={"min_ratio" : "mediumpurple", "avg_ratio" : "dodgerblue", "max_ratio" : "lightseagreen"}, width=0.9, figsize=(12,5), rot=0, edgecolor='black')
plt.yticks(ticks=np.arange(0.6, 1., .02, dtype=float))
plt.ylim(ymax=1, ymin = 0.7)
plt.title("Berea sandstone")
plt.xlabel("Rozmiar kafelka")
plt.ylabel("Średni współczynnik wypełnienia kafelka")

ax.axhline(y=0.7987,  color='r', label="Porowatość geometrii")
ax.legend()



plt.savefig("test6.png")

#import mplfinance as fplt
