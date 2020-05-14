import platform
import subprocess
if platform.system() == 'Windows':
    program = '%VULKAN_SDK%/Bin/glslangValidator.exe'
else:
    program = 'glslangValidator'


def validate_shader(data_src):
    status = subprocess.run([program, data_src])
    if status.returncode != 0:
        exit(1)
