#!/usr/bin/env python3

import sys
import platform
import subprocess
import os
from enum import Enum
from pathlib import Path

if platform.system() == 'Windows':
    vulkan_path = os.getenv("VULKAN_SDK")
    glsl_validator_program = '{}\\Bin\\glslangValidator.exe'.format(vulkan_path)
else:
    glsl_validator_program = 'glslangValidator'


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


def validate_glsl_shader(data_src, data_out, meta_content):

    path = Path(data_out)
    extension = path.suffix.lower()
    if "shader_type" in meta_content:
        shader_type = meta_content['shader_type']
    else:
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
        else:
            shader_type = ShaderType.MISC

        meta_content["shader_type"] = shader_type.value

    uniforms = []
    in_attributes = []
    out_attributes = []
    with open(path, 'r') as shader_file:
        lines = shader_file.readlines()
        if len(lines) == 0:
            sys.stderr.write("[Warning] No lines in shader file: {}\n".format(data_src))
            sys.exit(0)
        first_line = lines[0]
        first_line.replace('\n', '')
        split_line = first_line.split(' ')
        meta_content["glsl_version"] = split_line[1]
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

    render_api = "opengl"
    if "render_api" in meta_content:
        render_api = meta_content["render_api"]
    else:
        glsl_version = meta_content["glsl_version"]
        glsl_version_int = int(glsl_version)
        if glsl_version_int < 450:
            render_api = "opengl"

    if render_api == "opengl":
        status = subprocess.run([glsl_validator_program, data_src])
        if status.returncode != 0:
            exit(1)
    elif render_api == "vulkan":
        args = [glsl_validator_program, "-V", "-o", "{}.spv".format(data_out), data_src]
        status = subprocess.run(args)
        if status.returncode != 0:
            exit(1)





