#!/usr/bin/env python3

import json
import sys
import os
from shader_validator import validate_shader
from texture_validator import validate_texture
from material_validator import validate_material
from pathlib import Path
from enum import Enum
import os.path
import uuid


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
    VERT_SHADER = 4
    FRAG_SHADER = 5
    FONT = 6


img_extension = [
    ".jpeg",
    ".jpg",
    ".png",
    ".bmp",
    ".hdr",
    ".ktx",
    ".dds",
    ".pam",
    ".ppm",
    ".pgm"
]


def define_asset_type(filename) -> AssetType:
    global img_extension
    path = Path(filename)
    extension = path.suffix.lower()
    if extension in img_extension:
        return AssetType.TEXTURE
    if extension == '.mtl' or extension == '.mat':
        return AssetType.MTL
    if extension == '.obj':
        return AssetType.OBJ
    if extension == '.vert':
        return AssetType.VERT_SHADER
    if extension == '.frag':
        return AssetType.FRAG_SHADER
    if extension == '.ttf':
        return AssetType.FONT
    return AssetType.UNKNOWN


def validate_asset(src="", out=""):
    global data_src, data_out
    if src != "" and out != "":
        data_src = src
        data_out = out
    meta_content = {}
    asset_type = define_asset_type(data_src)
    # load meta data
    if asset_type != AssetType.UNKNOWN:
        if os.path.isfile(data_out + ".meta"):
            with open(data_out + ".meta", "r") as meta_file:
                meta_content = json.loads(meta_file.read())

    if asset_type == AssetType.TEXTURE:
        validate_texture(data_src, data_out, meta_content)
    if asset_type == AssetType.VERT_SHADER or asset_type == AssetType.FRAG_SHADER:
        validate_shader(data_src, data_out, meta_content)
    if asset_type == AssetType.MTL:
        validate_material(data_src, data_out, meta_content)
    # write new meta content to meta file
    if asset_type != AssetType.UNKNOWN:
        if not os.path.isfile(data_out + ".meta"):
            # We generate the meta file
            meta_content["uuid"] = str(uuid.uuid1())
        with open(data_out + ".meta", "w") as meta_file:
            json.dump(meta_content, meta_file, indent=4)


if __name__ == "__main__":
    arguments = ", ".join(sys.argv)
    print("Data validator arugments: "+arguments)
    data_src = sys.argv[1]
    data_out = sys.argv[2]
    create_out_subdirectories(data_out)
    validate_asset()
