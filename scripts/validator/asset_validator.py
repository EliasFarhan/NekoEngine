import json
import sys
import os
from pathlib import Path


def create_out_subdirectories(data_out):
    parent_dirs = []
    data_path = Path(data_out).absolute()
    print("Absolute path: "+str(data_path))
    while not data_path.exists():
        data_path = data_path.parent.absolute()
        print("Parent path: "+str(data_path))
        parent_dirs.append(data_path)
    for p in parent_dirs[::-1]:
        print(str(p.absolute()))
        try:
            os.mkdir(str(p.absolute()))
        except FileExistsError:
            pass


def define_asset_type(filename):
    pass


if __name__ == "__main__":
    arguments = ", ".join(sys.argv)
    print("Data validaot rarugments: "+arguments)
    data_src = sys.argv[1]
    data_out = sys.argv[2]
    create_out_subdirectories(data_out)