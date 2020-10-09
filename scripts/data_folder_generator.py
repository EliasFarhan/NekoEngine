#!/usr/bin/env python3

import validator.asset_validator
import sys
import os
import shutil


def iterate_over_folder(path):
    print("Iterating over: "+path)
    rel_path = os.path.relpath(path, data_dir)
    out_path = os.path.normpath(os.path.join(data_out_dir, rel_path))
    if not os.path.isdir(out_path):
        os.mkdir(out_path)
    directory = os.fsencode(path)
    for file in os.listdir(directory):
        filename = os.fsdecode(file)
        filepath = os.path.normpath(os.path.join(path, filename))
        if os.path.isdir(filepath):
            iterate_over_folder(filepath)
        elif os.path.isfile(filepath):
            out_file = os.path.join(out_path, filename)
            shutil.copy(filepath, out_file)
            validator.asset_validator.validate_asset(filepath, out_file)


if __name__ == "__main__":
    if len(sys.argv) >= 3:
        data_dir = sys.argv[1]
        data_out_dir = sys.argv[2]
    else:
        data_dir = "../data/"
        data_out_dir = "../data_out/"
    iterate_over_folder(data_dir)

