import os
import glob
import json
import subprocess
from zipfile import ZipFile


def create_archive(path, files):
    data_binary_path = os.environ.get("DATA_BIN_FOLDER")
    new_archive = ZipFile(path, 'w')
    for file in files:
        if '*' in file:
            all_files = glob.glob(os.path.join(data_binary_path, file))
            print("Glob: {}".format(all_files))
            for new_file in all_files:
                rel_path = os.path.relpath(new_file, data_binary_path)
                new_archive.write(new_file, rel_path)
        else:
            new_archive.write(os.path.join(data_binary_path, file), file)


def validate_pkg(data_src, data_out, meta_content):
    validator_path = os.getenv("VALIDATOR_FOLDER")
    validator_exe = os.getenv("VALIDATE_JSON_EXE")

    status = subprocess.run([validator_exe, data_src, validator_path+"pkg_validator.json"])
    if status.returncode != 0:
        exit(1)
    with open(data_src, 'r') as pkg_file:
        pkg_content = json.load(pkg_file)
        name = pkg_content["name"]
        files = pkg_content["files"]
        path = os.path.join(os.path.dirname(data_out), name+".pkg")
        create_archive(path, files)
