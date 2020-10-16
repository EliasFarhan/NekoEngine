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
    if toktx_exe is None:
        return 1
    arg = ""

    if "linear" in meta_data:
        linear = meta_data["linear"]
    else:
        linear = True
        meta_data["linear"] = linear
    if linear:
        arg += " --linear "
    else:
        arg += " --srgb "

    if "ktx2" in meta_data:
        ktx2 = meta_data["ktx2"]
    else:
        ktx2 = True
        meta_data["ktx2"] = ktx2
    if ktx2:
        arg += " --t2 "

    if "genmipmap" in meta_data:
        genmipmap = meta_data["genmipmap"]
    else:
        genmipmap = True
        meta_data["genmipmap"] = genmipmap
    if genmipmap:
        arg += " --genmipmap "

    if "compression" in meta_data:
        compression = meta_data["compression"]
    else:
        compression = "zcmp"
        meta_data["compression"] = compression
    if compression == "bcmp":
        arg += " --bcmp "
    elif compression == "zcmp":
        if "zcmp_level" in meta_data:
            zcmp_level = meta_data["zcmp_level"]
        else:
            zcmp_level = 3
            meta_data["zcmp_level"] = zcmp_level
        arg += " --zcmp {} ".format(zcmp_level)

    if os.path.isfile(img_out):
        os.remove(img_out)
    command = "{} {} {} {}".format(toktx_exe, arg, img_out, img)
    print(command)
    return os.system(command)


def validate_texture(img_src, img_out, meta_data):
    out_path = Path(img_out)
    extension = out_path.suffix.lower()
    if extension in ktxable_texture_ext:
        status = convert_to_ktx(img_out, img_out+".ktx", meta_data)
        meta_data["ktx"] = status == 0
