import json
import sys
import os
import shader_validator
import texture_validator
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


img_extension = [
    ".jpeg",
    ".jpg",
    ".png",
    ".bmp",
    ".hdr",
    ".ktx",
    ".dds"
]


def define_asset_type(filename) -> AssetType:
    global img_extension
    path = Path(filename)
    extension = path.suffix.lower()
    if extension in img_extension:
        return AssetType.TEXTURE
    if extension == '.mtl':
        return AssetType.MTL
    if extension == '.obj':
        return AssetType.OBJ
    if extension == '.vert':
        return AssetType.VERT_SHADER
    if extension == '.frag':
        return AssetType.FRAG_SHADER
    return AssetType.UNKNOWN


def validate_asset():
    global data_src
    meta_content = {}
    asset_type = define_asset_type(data_src)
    if asset_type != AssetType.UNKNOWN:
        if not os.path.isfile(data_out+".meta"):
            # We generate the meta file
            meta_content = {"uuid": str(uuid.uuid1())}
            meta_json = json.dumps(meta_content)
            with open(data_out+".meta", "w") as meta_file:
                meta_file.write(meta_json)

    if asset_type == AssetType.VERT_SHADER or asset_type == AssetType.FRAG_SHADER:
        shader_validator.validate_shader(data_src)


if __name__ == "__main__":
    arguments = ", ".join(sys.argv)
    print("Data validator arugments: "+arguments)
    data_src = sys.argv[1]
    data_out = sys.argv[2]
    create_out_subdirectories(data_out)
    validate_asset()
