import json
import sys
import os
from pathlib import Path
from enum import Enum

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


class AssetType(Enum):
    UNKNOWN = -1
    MTL = 0
    OBJ = 1
    TEXTURE = 2
    SCENE = 3
    SHADER = 4

def define_asset_type(filename) -> AssetType:
    path = Path(filename)
    extension = path.suffix.lower

    if extension == ".jpg"  or extension == ".jpeg":
        return AssetType.TEXTURE
    if extension == '.mtl':
        return AssetType.MTL
    if extension == '.obj':
        return AssetType.OBJ
    return AssetType.UNKNOWN



if __name__ == "__main__":
    arguments = ", ".join(sys.argv)
    print("Data validator arugments: "+arguments)
    data_src = sys.argv[1]
    data_out = sys.argv[2]
    create_out_subdirectories(data_out)