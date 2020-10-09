#!/usr/bin/env python3

import platform
import subprocess
import os
if platform.system() == 'Windows':
    vulkan_path = os.getenv("VULKAN_SDK")
    program = '{}\\Bin\\glslangValidator.exe'.format(vulkan_path)
else:
    program = 'glslangValidator'


def validate_shader(data_src, meta_content):
    status = subprocess.run([program, data_src])
    if status.returncode != 0:
        exit(1)
