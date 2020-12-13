from validator.asset_validator import validate_asset
import sys
import os
import shutil
import json
from pathlib import Path

img_extensions = [
    ".jpg",
    ".jpeg",
    ".png",
    ".bmp",
    ".tga",
    ".ktx"
]

txt_extensions = [
    ".json",
    ".txt"
]

model_extensions = [
    ".obj",
    ".fbx"
]

shader_extensions = [
    ".vert",
    ".frag",
    ".glsl",
    ".geom",
    ".comp",
    ".tesc",
    ".tese"
]

skybox_extensions = [
    ".skybox"
]

snd_extensions = [
    ".wav",
    ".ogg"
]

font_extensions = [
    ".ttf"
]

material_extensions = [
    ".mat",
    ".mtl"
]

package_extensions = [
    ".pkg_json"
]

def iterate_over_type(extensions):
    print("Iterate over type: {} in data_dir {}".format(extensions, data_dir))
    data_src_path = Path(data_dir)
    for extension in extensions:
        for file in data_src_path.rglob("*{}".format(extension)):
            rel_path = os.path.relpath(str(file), data_dir)
            out_file = Path(os.path.join(data_out_dir, rel_path))
            print("File: {} Rel path: {}".format(out_file, rel_path))
            if not out_file.parent.is_dir():
                out_file.parent.mkdir(parents=True, exist_ok=True)
            validate_asset(str(file), str(out_file))


def load_env_variables():
    os.environ["DATA_BIN_FOLDER"] = data_out_dir
    os.environ["DATA_SRC_FOLDER"] = data_dir
    with open(env_path, 'r') as env_file:
        env_content = json.load(env_file)
        for key in env_content:
            print("Adding env variable {} {}".format(key, env_content[key]))
            os.environ[key] = env_content[key]


if __name__ == "__main__":
    if len(sys.argv) >= 4:
        data_dir = sys.argv[1]
        data_out_dir = sys.argv[2]
        env_path = sys.argv[3]
    else:
        data_dir = "../data/"
        data_out_dir = "../data_out/"
        env_path = "../tmp/env.json"
    print("Data In: {}, Data Output: {}".format(data_dir, data_out_dir))
    load_env_variables()
    iterate_over_type(shader_extensions)
    iterate_over_type(skybox_extensions)
    iterate_over_type(img_extensions)
    iterate_over_type(txt_extensions)
    iterate_over_type(model_extensions)
    iterate_over_type(material_extensions)
    iterate_over_type(font_extensions)
    iterate_over_type(package_extensions)

