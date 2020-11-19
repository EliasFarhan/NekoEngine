#!/usr/bin/env python3
import sys
import os
from os import environ
from pathlib import Path
import subprocess

src_path = environ.get("SRC_FOLDER")
basisu_exe = environ.get("BASISU_EXE")
if basisu_exe is None:
    exit(1)

ktxable_texture_ext = [
    ".jpeg",
    ".jpg",
    ".png",
    ".bmp",
    ".tga"
]

compression_types = [
    "uastc",
]


def convert_to_ktx(img, img_out, meta_data):
    # TODO select the format accroding to format DXT1 (TGB) DXT5 (RGBA)
    return 0
    arg = []

    if "linear" in meta_data:
        linear = meta_data["linear"]
    else:
        linear = True
        meta_data["linear"] = linear
    if linear:
        arg.append("-linear")


    if "genmipmap" in meta_data:
        genmipmap = meta_data["genmipmap"]
    else:
        genmipmap = True
        meta_data["genmipmap"] = genmipmap
    if genmipmap:
        arg.append("-mipmap")

    if "compression" in meta_data:
        compression = meta_data["compression"]
    else:
        compression = "none"
        meta_data["compression"] = compression

    if compression == "uastc":
        if "uastc_level" in meta_data:
            uastc_level = meta_data["uastc_level"]
        else:
            uastc_level = 2
            meta_data["uastc_level"] = uastc_level
        arg.append("-uastc")
        arg.append("-uastc_level")
        arg.append(str(uastc_level))

    if os.path.isfile(img_out):
        os.remove(img_out)
    
    tmp_folder = os.path.join(src_path, "tmp")
    if not os.path.isdir(tmp_folder):
        try:
            os.mkdir(tmp_folder)
        except OSError:
            sys.stderr.write("Error while creating folder {}".format(tmp_folder))

    img_name = Path(img).stem+".basis"
    basis_path =  os.path.join(tmp_folder, img_name)
    command = [basisu_exe, "-file", img, "-output_file", basis_path]
    command.extend(arg)
    print(command)
    status = subprocess.run(command)
    if status.returncode != 0:
        sys.stderr.write("[Error] Could not generate basis file\n")
        return status.returncode
    command = [basisu_exe, "-file", basis_path, "-unpack", "-output_path", tmp_folder+"/"]
    print(command)
    status = subprocess.run(command)
    if status.returncode != 0:
        sys.stderr.write("[Error] Could not generate ktx file\n")
        return status.returncode
    
    return 0


def validate_texture(img_src, img_out, meta_data):
    out_path = Path(img_out)
    extension = out_path.suffix.lower()
    if extension in ktxable_texture_ext:
        status = convert_to_ktx(img_out, img_out+".ktx", meta_data)
        meta_data["ktx"] = status == 0
