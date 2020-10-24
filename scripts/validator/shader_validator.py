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
    in_attributes = []
    out_attributes = []
    with open(path, 'r') as shader_file:
        lines = shader_file.readlines()
        for line in lines:
            line = line.replace('\n', '')
            line = line.replace(';', '')
            split_line = line.split(' ')
            if "uniform" in split_line:
                uniform_obj = {}
                uniform_obj["type"] = split_line[1]
                uniform_obj["name"] = split_line[2]
                uniforms.append(uniform_obj)
            if "in" in split_line:
                comment_index = len(split_line)
                if '//' in split_line:
                    comment_index = split_line.index('//')
                index = split_line.index('in')
                if index > comment_index:
                    continue
                in_variable = {}
                in_variable["type"] = split_line[index+1]
                in_variable["name"] = split_line[index+2]
                in_attributes.append(in_variable)
            if "out" in split_line:
                comment_index = len(split_line)
                if '//' in split_line:
                    comment_index = split_line.index('//')
                index = split_line.index('out')
                if index > comment_index:
                    continue
                index = split_line.index('out')
                out_variable = {}
                out_variable["type"] = split_line[index+1]
                out_variable["name"] = split_line[index+2]
                out_attributes.append(out_variable)
    meta_content["uniforms"] = uniforms
    meta_content["in_attributes"] = in_attributes
    meta_content["out_attributes"] = out_attributes





