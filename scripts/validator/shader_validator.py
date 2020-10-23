#!/usr/bin/env python3

import platform
import subprocess
import os
from enum import Enum
from pathlib import Path

if platform.system() == 'Windows':
    vulkan_path = os.getenv("VULKAN_SDK")
    program = '{}\\Bin\\glslangValidator.exe'.format(vulkan_path)
else:
    program = 'glslangValidator'


class ShaderType(Enum):
    MISC = 0
    VERT = 1
    FRAG = 2
    TESC = 3
    TESE = 4
    GEOM = 5
    COMP = 6


class UniformObject:
    def __init__(self, name, type):
        self.name = name
        self.type = type


def validate_shader(data_src, data_out, meta_content):
    status = subprocess.run([program, data_src])
    if status.returncode != 0:
        exit(1)
    path = Path(data_out)
    extension = path.suffix.lower()
    if extension == '.vert':
        shader_type = ShaderType.VERT
    elif extension == '.frag':
        shader_type = ShaderType.FRAG
    elif extension == '.tesc':
        shader_type = ShaderType.TESC
    elif extension == '.tese':
        shader_type = ShaderType.TESE
    elif extension == '.geom':
        shader_type = ShaderType.GEOM
    elif extension == '.comp':
        shader_type = ShaderType.COMP
    elif extension == '.glsl':
        shader_type = ShaderType.MISC

    meta_content["shader_type"] = shader_type.value

    uniforms = []
    with open(path, 'r') as shader_file:
        lines = shader_file.readlines()
        for line in lines:
            line = line.replace('\n', '')
            line = line.replace(';', '')
            if "uniform" in line:
                uniform = line.split(' ')
                uniform_obj = {}
                uniform_obj["type"] = uniform[1]
                uniform_obj["name"] = uniform[2]
                uniforms.append(uniform_obj)
    meta_content["uniforms"] = uniforms




