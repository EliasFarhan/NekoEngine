import json
import os.path

from pathlib import Path

binary_path = os.getenv("BINARY_FOLDER")
print("Binary path: {}".format(binary_path))


def get_texture_id(texture_path):
    # check if texture meta exists
    meta_path = str(texture_path)+'.meta'
    if not os.path.isfile(meta_path):
        return ''
    with open(meta_path, 'r') as texture_meta:
        meta_content = json.load(texture_meta)
        if 'uuid' in meta_content:
            return meta_content['uuid']
    return ''


def validate_mat_json_file(data_src, data_out, meta_content):
    with open(data_src, 'r') as mat_file:
        mat_content = json.load(mat_file)
        mat_keys = mat_content.keys()
        new_content = {}
        uniforms = []
        for key in mat_keys:
            # Putting texture uuid
            if "map_path" in key:
                key_id = key.replace('path', 'id')
                if key_id not in mat_keys:
                    id = get_texture_id(os.path.join(os.path.join(binary_path, 'data'), mat_content[key]))
                    new_content[key_id] = id
            # Loading shader content to material
            if "shader_path" in key:
                shader_path = os.path.join(os.path.join(binary_path, 'data'), mat_content[key])
                with open(shader_path, 'r') as shader_content:
                    new_key = key.replace('path', 'content')
                    new_content[new_key] = shader_content.read()
                shader_meta_path = shader_path+".meta"
                with open(shader_meta_path, 'r') as shader_meta_file:
                    shader_meta = json.load(shader_meta_file)
                    if 'uniforms' in shader_meta:
                        for uniform_obj in shader_meta['uniforms']:
                            insert = True
                            # Avoid duplicate uniform
                            for u in uniforms:
                                if u['name'] == uniform_obj['name']:
                                    insert = False
                                    break
                            if insert:
                                uniforms.append(uniform_obj)
        new_content['uniforms'] = uniforms
        mat_content.update(new_content)

    with open(data_out, 'w') as mat_file:
        json.dump(mat_content, mat_file, indent=4)


def insert_map_in_mat(map_type, dir_out, texture_name, current_material):
    texture_path = dir_out.joinpath(Path(texture_name))
    texture_id = get_texture_id(texture_path)
    current_material[map_type+"_path"] = str(texture_path.relative_to(os.path.join(binary_path, "data")))
    current_material[map_type+"_id"] = texture_id


def validate_mtl_file(data_src, data_out, meta_content):
    materials = []
    materials_name = []
    current_material = {}
    data_out_path = Path(data_out)
    dir_out = data_out_path.parent
    with open(data_src) as mtl_file:
        lines = mtl_file.readlines()
        for line in lines:
            line = line.replace('\n', '')
            split_line = line.split(' ')
            key = split_line[0].replace('\t', '')
            if key == 'newmtl':
                if len(current_material) != 0:
                    materials.append(current_material)
                current_material = {"name": split_line[1]}
                materials_name.append(split_line[1]+".mat")
            # ambient color
            if key == 'Ka':
                current_material['ambient'] = [float(split_line[1]), float(split_line[2]), float(split_line[3])]
            # diffuse color
            if key == 'Kd':
                current_material['diffuse'] = [float(split_line[1]), float(split_line[2]), float(split_line[3])]
            # specular color
            if key == 'Ks':
                current_material['specular'] = [float(split_line[1]), float(split_line[2]), float(split_line[3])]
            # specular exponent
            if key == 'Ns':
                current_material['specular_exponent'] = float(split_line[1])
            # transparency
            if key == 'd':
                current_material['alpha'] = float(split_line[1])

            if key == 'Tr':
                current_material['alpha'] = 1-float(split_line[1])

            # index of refraction
            if key == 'Ni':
                current_material['refraction'] = float(split_line[1])

            # ambient map
            if key == 'map_Ka':
                insert_map_in_mat('ambient_map', dir_out, split_line[1], current_material)

            # diffuse map
            if key == 'map_Kd':
                insert_map_in_mat('diffuse_map', dir_out, split_line[1], current_material)

            # specular map
            if key == 'map_Ks':
                insert_map_in_mat('specular_map', dir_out, split_line[1], current_material)

            # alpha map
            if key == 'map_d':
                insert_map_in_mat('alpha_map', dir_out, split_line[1], current_material)

            # normal map
            if key == 'bump' or key == 'map_bump':
                insert_map_in_mat('normal_map', dir_out, split_line[1], current_material)
    meta_content["material_files"] = materials_name
    for material in materials:
        mat_path = dir_out.joinpath(Path(material['name']+'.mat'))
        with open(mat_path, 'w') as mat_file:
            json.dump(material, mat_file, indent=4)


def validate_material(data_src, data_out, meta_content):
    path = Path(data_src)
    extension = path.suffix.lower()
    if extension == '.mtl':
        validate_mtl_file(data_src, data_out, meta_content)
    elif extension == '.mat':
        validate_mat_json_file(data_src, data_out, meta_content)
