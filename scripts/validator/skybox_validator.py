import os
import subprocess
from enum import Enum
import json
import sys


validator_path = os.getenv("VALIDATOR_FOLDER")
validator_exe = os.getenv("VALIDATE_JSON_EXE")
print("Validator exe: {} with validator folder: {}".format(validator_exe, validator_path))


class CubemapTextureType(Enum):
    RIGHT = 0
    LEFT = 1
    TOP = 2
    BOTTOM = 3
    FRONT = 4
    BACK = 5
    LENGTH = 6


def validate_skybox(data_src, data_out, meta_content):
    status = subprocess.run([validator_exe, data_src, validator_path+"skybox_validator.json"])
    if status.returncode != 0:
        exit(1)
    toktx_exe = os.environ.get("TOKTX_EXE")
    if toktx_exe is None:
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
            toktx_exe,
            "--cubemap",
            os.path.join(os.path.dirname(data_out), skybox_content["name"]+".ktx")
        ]

        textures = map(lambda path : os.path.join(os.path.dirname(data_src), path), textures)
        command.extend(textures)
        print(command)
        status = subprocess.run(command, stderr=subprocess.STDOUT, stdout=subprocess.PIPE)
        if status.returncode != 0:
            sys.stderr.write("[Error] Could not generate skybox ktx\n")
            exit(1)

