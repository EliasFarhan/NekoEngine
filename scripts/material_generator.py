from pathlib import Path
import json
import sys
import os

def generate_from_mtl(mtl_path: Path):
    materials = {}
    current_material = {}
    with open(str(mtl_path)) as mtl_file:
        for line in mtl_file.readlines():
            split_line = line.split(' ')
            key = split_line[0]
            if '#' == line[0]:
                continue
            if 'newmtl' in line:
                material_name = split_line[-1].replace('\n', '')
                materials[material_name] = {}
                current_material = materials[material_name]
                current_material["name"] = material_name

            if 'Ns' in key:
                current_material["specular_exponent"] = float(split_line[-1])
            if 'Ni' in key:
                current_material["optic_density"] = float(split_line[-1])
            if not 'map' in key and 'd' in key:
                current_material["transparency"] = float(split_line[-1])
            if 'Tr' in key:
                pass
            if 'Tf' in key:
                pass
            if not 'map' in key and 'Ka' in key:
                current_material["ambient_color"] = [float(split_line[1 + i]) for i in range(3)]
            if not 'map' in key and 'Kd' in key:
                current_material["diffuse_color"] = [float(split_line[1 + i]) for i in range(3)]
            if not 'map' in key and 'Ks' in key:
                current_material["specular_color"] = [float(split_line[1 + i]) for i in range(3)]
            if not 'map' in key and 'Ke' in key:
                current_material["emissive_color"] = [float(split_line[1 + i]) for i in range(3)]
            if 'map_Ka' in key:
                current_material["ambient_map"] = split_line[-1].replace('\n', '')
            if 'map_Kd' in key:
                current_material["diffuse_map"] = split_line[-1].replace('\n', '')
            if 'map_bump' in key:
                current_material["normal_map"] = split_line[-1].replace('\n', '')
            if 'illum' in key:
                pass
    return materials


if __name__ == '__main__':
    if len(sys.argv) != 3:
        sys.exit(1)
    mtl_filename = sys.argv[1]
    mat_out_folder = sys.argv[2]
    materials =  generate_from_mtl(Path(Path(mtl_filename)))
    for material_name in materials.keys():
        material = materials[material_name]
        material_json = json.dumps(material, indent=4)
        with open(os.path.join(mat_out_folder, material_name+".mat"), "w") as material_file:
            material_file.write(material_json)

