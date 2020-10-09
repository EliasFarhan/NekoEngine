#!/usr/bin/env python3
import sys
import os
from os import environ
from pathlib import Path

ktxable_texture_ext = [
    ".jpeg",
    ".jpg",
    ".png",
    ".pam",
    ".ppm",
    ".pgm"
]


def convert_to_ktx(img, img_out, meta_data):
    toktx_exe = environ.get("TOKTX_EXE")
    print("Convert {} to {} with {}".format(img, img_out, toktx_exe))
    arg = ""

    if "linear" in meta_data:
        linear = meta_data["linear"]
    else:
        linear = True
        meta_data["linear"] = True
    if linear:
        arg += " --linear "
    else:
        arg += " --srgb "

    if "ktx2" in meta_data:
        ktx2 = meta_data["ktx2"]
    else:
        ktx2 = True
        meta_data["ktx2"] = True
    if ktx2:
        arg += " --t2 "

    if "genmipmap" in meta_data:
        genmipmap = meta_data["genmipmap"]
    else:
        genmipmap = True
        meta_data["genmipmap"] = True
    if genmipmap:
        arg += " --genmipmap "

    if os.path.isfile(img_out):
        os.remove(img_out)
    return os.system("{} {} {} {}".format(toktx_exe, arg, img_out, img))


def validate_texture(img_src, img_out, meta_data):
    out_path = Path(img_out)
    extension = out_path.suffix.lower()
    if extension in ktxable_texture_ext:
        status = convert_to_ktx(img_out, img_out+".ktx", meta_data)
        meta_data["ktx"] = status == 0
