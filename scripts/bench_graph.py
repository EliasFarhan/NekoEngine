# sphinx_gallery_thumbnail_number = 3
import matplotlib.pyplot as plt
import numpy as np
import json
import sys


def parse(filename):
    bench_values = {}
    with open(filename) as f:
        benchjson = json.load(f)

        benchmarks = benchjson["benchmarks"]
        for b in benchmarks:
            name_n = b["name"]
            name = name_n[0:name_n.find('/')]
            n = int(name_n[name_n.find('/')+1:])
            time = b["real_time"]
            if name not in bench_values.keys():
                bench_values[name] = []
            bench_values[name].append((n, time))

    return bench_values


def plot(bench_dict):
    for bench_name in bench_dict.keys():
        x = list(map(lambda xy: xy[0], bench_dict[bench_name]))
        y = list(map(lambda xy : xy[1], bench_dict[bench_name]))

        plt.plot(x, y, label=bench_name)
    plt.xlabel("Range")
    plt.xscale("log")
    plt.ylabel("Time (ns)")
    plt.yscale("log")
    plt.legend()
    plt.show()


# read and parse the data from json
# plot them to matplotlib
def main():
    try:
        filename = sys.argv[1]
    except Exception:
        sys.stderr.write("No argument given")
        exit(1)
    bench_dict = parse(filename)
    plot(bench_dict)


if __name__ == '__main__':
    main()
