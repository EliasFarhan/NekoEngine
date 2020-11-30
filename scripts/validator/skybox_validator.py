import os
import subprocess
from enum import Enum
import json
import sys
from pathlib import Path



class CubemapTextureType(Enum):
    RIGHT = 0
    LEFT = 1
    TOP = 2
    BOTTOM = 3
    FRONT = 4
    BACK = 5
    LENGTH = 6


def validate_skybox(data_src, data_out, meta_content):
    src_path = os.environ.get("SRC_FOLDER")
    validator_path = os.getenv("VALIDATOR_FOLDER")
    validator_exe = os.getenv("VALIDATE_JSON_EXE")
    data_bin_path = os.getenv("DATA_BIN_FOLDER")

    status = subprocess.run([validator_exe, data_src, validator_path+"skybox_validator.json"])
    if status.returncode != 0:
        exit(1)
    basisu_exe = os.environ.get("BASISU_EXE")
    if basisu_exe is None:
        return 1
    with open(data_src, 'r') as skybox_file:
        skybox_content = json.load(skybox_file)
        textures = [
            skybox_content["right"],
            skybox_content["left"],
            skybox_content["top"],
            skybox_content["bottom"],
            skybox_content["front"],
            skybox_content["back"]
        ]
        command = [
            basisu_exe
        ]
        tmp_folder = os.path.join(src_path, "tmp")
        textures = map(lambda path : os.path.join(os.path.dirname(data_src), path), textures)
        command.extend(textures)
        basis_path = os.path.join(tmp_folder, skybox_content["name"]+".basis")
        img_out = os.path.join(os.path.dirname(data_out), skybox_content["name"]+".ktx")
        command.extend([
            "-tex_type", "cubemap",
            "-output_file",
             basis_path
        ])
        print(command)
        status = subprocess.run(command, stderr=subprocess.STDOUT, stdout=subprocess.PIPE)
        if status.returncode != 0:
            sys.stderr.write("[Error] Could not generate skybox basis\n")
            exit(1)
        output_format = ["-format"]
        transcoder = ""
        # default format is bc1
        if not transcoder:
            transcoder = "bc1"

        if transcoder == "bc1":
            output_format.append("BC1")
        elif transcoder == "bc7":
            output_format.append("BC7")
        elif transcoder == "etc1":
            output_format.append("ETC1")
        else:
            sys.stderr.write("Warning trying to add a non supported format for RGB")
            output_format.append("BC1")
            transcoder = "bc1"
        meta_content["transcoder"] = transcoder
        command = [
            basisu_exe,
            "-file",
            basis_path,
            "-unpack",
            "-no_png",
            "-output_file",
            img_out
        ]
        command.extend(output_format)
        print(command)
        status = subprocess.run(command, stderr=subprocess.STDOUT, stdout=subprocess.PIPE)
        if status.returncode != 0:
            sys.stderr.write("[Error] Could not generate skybox ktx\n")
            exit(1)
        meta_content["ktx_path"] = str(Path(img_out).relative_to(data_bin_path))


