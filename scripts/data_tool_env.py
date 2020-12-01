import json
import os
from pathlib import Path

env_var = [
    "SRC_FOLDER",
    "BASISU_EXE",
    "IMAGE_FORMAT_EXE",
    "VALIDATOR_FOLDER",
    "VALIDATE_JSON_EXE"
]


def save_env_var():
    src_folder = os.getenv("SRC_FOLDER")
    env_json_path = Path(os.path.join(src_folder), "tmp/env.json")
    if not env_json_path.parent.is_dir():
        env_json_path.parent.mkdir(parents=True, exist_ok=True)
    env_var_content = {}
    for env in env_var:
        env_var_content[env] = os.getenv(env)
    with open(env_json_path, 'w') as env_json_file:
        json.dump(env_var_content, env_json_file)


if __name__ == "__main__":
    save_env_var()
