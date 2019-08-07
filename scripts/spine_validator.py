import json
import sys

cmake_source_dir = ""
cmake_binary_dir = ""


def validate_with_atlas(file:str):
    print("Validate: "+file)


def main():
    global cmake_source_dir, cmake_binary_dir
    argv = sys.argv

    file = argv[1]
    if '.atlas.txt' in file:
        validate_with_atlas(file)


if __name__ == "__main__":
    main()
