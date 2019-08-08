import json
import sys
import os

data_source_dir = ""
data_binary_dir = ""


def validate_with_atlas(file: str):
    global data_source_dir, data_binary_dir
    filename = os.path.basename(file)
    filename_without_ext = filename.replace(".atlas", "").replace(".txt", "")
    skeleton_json_file = filename_without_ext + ".json"
    if not os.path.exists(data_source_dir + "/" + skeleton_json_file):
        sys.stderr.write("Skeleton does not exist: "+skeleton_json_file+"\n")
        return 1
    spine_json = {
        "atlas": filename,
        "skeleton": skeleton_json_file
    }

    spine_json_txt = json.dumps(spine_json, indent=4)
    with open(data_binary_dir + "/" + filename_without_ext + ".spine", "w") as spine_json_file:
        spine_json_file.write(spine_json_txt)

    print("Validating: "+filename_without_ext+".spine")
    return 0


def main():
    global data_source_dir, data_binary_dir
    argv = sys.argv
    if len(sys.argv) is not 4:
        return 1
    file = argv[1]
    data_source_dir = argv[2]
    data_binary_dir = argv[3]

    if '.atlas.txt' in file:
        validate_with_atlas(file)
        return 0
    return 0


if __name__ == "__main__":
    status = main()
    sys.exit(status)
