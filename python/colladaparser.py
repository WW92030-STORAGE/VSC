import sys, os, math
import xml.etree.ElementTree as ET
from collections import deque

"""

!!!!!!!!!!!WARNING!!!!!!!!!!!

- This assumes your mesh adheres to a few rather limiting but simple assumptions. Everything else is undefined behavior!!!!

+ There is only one mesh in your DAE (This is achieved by CTRL+J on the meshes in Blender, or by exporting a subset of your scene containing your mesh and your armature.)
+ The mesh is only controlled by a single armature. There are no morphs or shape keys. WARNING - The mesh must be controlled by an armature -- everything else is undefined behavior.
+ All transforms have been applied, so the binding matrix is identity. (Select all, CTRL+A and then all transforms.)
+ All bones in your armature have distinct names.
+ The armature system is represented as NODES in the <library_visual_scenes> portion of the COLLADA file, and all bones have type JOINT.

- Be aware of any unexpected behavior. For best results export from Blender.

https://www.khronos.org/files/collada_spec_1_4.pdf

"""

# Constants

PREFIX = "../misc"
FILENAME = "BONES3"
OUTPUT = FILENAME + ".dat"

def printout(value, type):
    res = str(value)
    if "str" in type:
        res = "\"" + res + "\""
    return res

def get_info(element):
    tag = element.tag
    attrib = element.attrib
    text = element.text
    if text is not None:
        text = str(text).strip()
    return f"Tag: {tag}, Attributes: {attrib}, Text: {text}"

def print_set(input: list, type: str = "int", name: str = "v", unordered = True):
    retval = "std::" + ("unordered_" if unordered else "") + "set<" + type + "> " + name + "({"

    for i in range(len(input)):
        if i > 0:
            retval = retval + ", "
        
        retval = retval + printout(input[i], type)
    
    retval = retval + "});"
    return retval

def print_vector(input: list, type: str = "int", name: str = "v"):
    retval = "std::vector<" + type + "> " + name + "({"

    for i in range(len(input)):
        if i > 0:
            retval = retval + ", "
        
        retval = retval + printout(input[i], type)
    
    retval = retval + "});"
    return retval

def print_map(input: dict, key: str = "std::string", val: str = "std::string", name: str = "m", unordered = False):
    retval = "std::" + ("unordered_" if unordered else "") + "map<" + key + ", " + val + "> " + name + "({"
    xx = False
    for i in input:
        if xx:
            retval = retval + ", "
        xx = True
        
        retval = retval + "{" + printout(i, key) + ", " + printout(input[i], val) + "}"
    
    retval = retval + "});"
    return retval

def print_vector_2d(input: list[list], type: str = "int", name: str = "v"):
    retval = "std::vector<std::vector<" + type + ">> " + name + "({"

    for i in range(len(input)):
        if i > 0:
            retval = retval + ", "
        
        retval = retval + "{"
        for j in range(len(input[i])):
            if j > 0:
                retval = retval + ", "
            retval = retval + printout(input[i][j], type)
        retval = retval + "}"
    
    retval = retval + "});"
    return retval

def print_vector_vector(input: list[list], type: str = "Vector3", name: str = "v"):
    retval = "std::vector<" + type + "> " + name + "({"

    for i in range(len(input)):
        if i > 0:
            retval = retval + ", "
        
        retval = retval + str(type) + "("
        for j in range(len(input[i])):
            if j > 0:
                retval = retval + ", "
            retval = retval + str(input[i][j])
        retval = retval + ")"
    
    retval = retval + "});"
    return retval


def load_array(thing) -> list:
    text = thing.text.split()
    if "float_array" in thing.tag:
        array = [float(x) for x in text]
    elif "int_array" in thing.tag:
        array = [int(x) for x in text]
    elif "bool_array" in thing.tag:
        array = [bool(x) for x in text]
    else:
        array = text
    
    return array


def parse(FILE_PATH = PREFIX + "/" + FILENAME + ".dae", NAME = FILENAME, OUTPUT_NAME = "BONE"):
    tree = ET.parse(FILE_PATH)
    root = tree.getroot()
    print("Tree: " + str(tree))
    print("ROOT: " + str(root))

    # Storage structures for the resultants

    vertex_positions = []
    tex_coords = []

    # Storage structures for skinning information
    vertex_weights = []  # should have same length as vertex_positions. each one has a dict {str, float} mapping joint names to their weights
    joint_names = []
    joint_inv = {}
    inverse_bind_matrices = []
    bone_tips = {} # Tips for bones if they have them
    absolute_tips = []


    # Fuck around and find out!

    for library in root:
        if library is None:
            continue

        tag = library.tag  # ID
        if "library_geometries" in tag:  # library_geometries
            print("LIB_GEOM: " + get_info(library))
            for geometry in library:  # geometry
                for mesh in geometry:  # mesh
                    if "mesh" not in mesh.tag:
                        continue
                    # meshes only

                    print("MESH: " + get_info(mesh))
                    sources = {}
                    vertex_array_id = ""

                    vertices_id = ""

                    triangle_inputs = {}
                    triangle_array = []
                    

                    for element in mesh:
                        print("ELEMENT: " + get_info(element))
                        id = element.attrib.get("id", "null")
                        if "source" in element.tag:  # mesh.source
                            print("SOURCE: id=" + str(id))
                            array = []
                            stride = 1

                            for thing in element:
                                if "array" in thing.tag:  # array
                                    array = load_array(thing)
                                elif "technique" in thing.tag:  # technique
                                    for accessor in thing:  # accessor
                                        stride = int(accessor.attrib.get("stride", 1))
                            # print("ARRAY: " + str(array))
                            # print("STRIDE: " + str(stride))
                            final_array = []
                            for i in range(0, len(array), stride):
                                row = []
                                for j in range(stride):
                                    if i + j < len(array):
                                        row.append(array[i + j])
                                final_array.append(row)
                            
                            sources[id] = final_array
                        elif "vertices" in element.tag:  # mesh.vertices
                            vertices_id = element.attrib.get("id", "")
                            for child in element:
                                if "input" in child.tag and child.attrib.get("semantic", "") == "POSITION":  # only positions
                                    vertex_array_id = child.attrib.get("source", "")[1:]  # remove the pound
                                
                        
                        elif "triangles" in element.tag:  # mesh.triangles
                            for child in element:
                                print("TP:", get_info(child))
                                if child.tag[-1] == "p":
                                    triangle_array = [int(x) for x in child.text.split()]
                                elif "input" in child.tag:
                                    attrib = child.attrib
                                    triangle_inputs[attrib.get("semantic", "")] = [attrib.get("source", ""), int(attrib.get("offset", "0"))]
                            
                    
                    # endfor: element in mesh
                    
                    for thing in sources:
                        print("source:" + str(thing) + ": " + str(sources[thing]))
                    
                    # Get the vertex array. If you do things correctly 
                    if vertex_array_id in sources:
                        vertex_positions = sources[vertex_array_id]
                        sources[vertices_id] = vertex_positions  
                    print("VERTEX ARRAY ID: " + str(vertex_array_id))
                    print("VERTEX POSITIONS: " + str(vertex_positions))

                    print("TRI ARRAY: " + str(triangle_array) + " | " + str(len(triangle_array)))
                    print("TRI_INPUTS: " + str(triangle_inputs))

                    inputs_per_vertex = len(triangle_inputs)  # number of values that goes into a vertex, detailed by teh system

                    # Mesh constructor has v, t, tex, texco. v and texco are triangle_inputs["VERTEX"][0] and triangle_inputs["TEXCOORD"][0] respectively, in the sources:

                    # input (v)
                    vertex_positions = sources.get(triangle_inputs.get("VERTEX", ["#"])[0][1:], [])

                    # input (tex)
                    tex_coords = sources.get(triangle_inputs.get("TEXCOORD", ["#"])[0][1:], [])

                    # input (t)
                    vertex_offset = triangle_inputs.get("VERTEX", ["#", 0])[1]
                    print("VERTEX_OFFSET:", vertex_offset)

                    tri_indices = []
                    index = vertex_offset
                    while index < len(triangle_array):
                        row = []
                        for rot in range(3):
                            row.append(triangle_array[index])
                            index += inputs_per_vertex
                        tri_indices.append(row)
                        

                    # input (texco)
                    texcoord_offset = triangle_inputs.get("TEXCOORD", ["#", 0])[1]
                    print("TEXCOORD_OFFSET:", texcoord_offset)

                    texco = []
                    index = texcoord_offset
                    while index < len(triangle_array):
                        row = []
                        for rot in range(3):
                            row.append(triangle_array[index])
                            index += inputs_per_vertex
                        texco.append(row)

                    # print("v =", vertex_positions)
                    # print("tex =", tex_coords)
                    # print("t =", tri_indices)
                    # print("texco =", texco)
        
        elif "library_controllers" in tag:  # library_controllers
            for controller in library:
                if "controller" not in controller.tag:
                    continue
                # library_controllers.controller
                for skin in controller:
                    if "skin" not in skin.tag:
                        continue
                    # controller.skin

                    sources = {}
                    joint_inputs = {}
                    vertex_weight_inputs = {}

                    for element in skin:
                        if "bind_shape_matrix" in element.tag:  # you must apply all transforms before exporting
                            pass
                        elif "source" in element.tag:
                            attrib = element.attrib
                            id = attrib.get("id", "")
                            array = []
                            stride = 1

                            for thing in element:
                                if "array" in thing.tag:  # array
                                    array = load_array(thing)
                                elif "technique" in thing.tag:  # technique
                                    for accessor in thing:  # accessor
                                        stride = int(accessor.attrib.get("stride", 1))
                            # print("ARRAY: " + str(array))
                            # print("STRIDE: " + str(stride))
                            final_array = []
                            for i in range(0, len(array), stride):
                                row = []
                                for j in range(stride):
                                    if i + j < len(array):
                                        row.append(array[i + j])
                                final_array.append(row)
                            
                            print("FINAL_ARRAY FOR " + id + " =", final_array)
                            sources[id] = final_array
                        if "joints" in element.tag:
                            for thing in element:
                                if "input" in thing.tag:
                                    attrib = thing.attrib
                                    source = attrib.get("source", "#")[1:]
                                    offset = int(attrib.get("offset", "0"))
                                    joint_inputs[attrib.get("semantic", "")] = (source, offset)
                        if "vertex_weights" in element.tag:
                            vcounts = []
                            v_array = []
                            for thing in element:
                                if "input" in thing.tag:
                                    attrib = thing.attrib
                                    source = attrib.get("source", "#")[1:]
                                    offset = int(attrib.get("offset", "0"))
                                    vertex_weight_inputs[attrib.get("semantic", "")] = (source, offset)
                                elif "vcount" in thing.tag:
                                    vcounts = [int(x) for x in thing.text.split()]
                                elif "v" in thing.tag and "v" == thing.tag[-1]:
                                    v_array = [int(x) for x in thing.text.split()]

                    # endfor element in skin

                    print("SRC", sources)
                    print("JI", str(joint_inputs))
                    print("VWI", str(vertex_weight_inputs))
                    print("VC", vcounts)
                    print("VA", v_array)

                    
                    # Joint information

                    joint_names = [x[0] for x in sources.get(joint_inputs.get("JOINT", "")[0], [])]
                    for i in range(len(joint_names)):
                        joint_inv[joint_names[i]] = i
                    print("JOINT NAMES:", joint_names)
                    print("JOINT_INV", joint_inv)

                    inverse_bind_matrices = []
                    for matrix in sources.get(joint_inputs.get("INV_BIND_MATRIX", "")[0], []):
                        mat = [[], [], [], []]
                        for i in range(4):
                            for j in range(4):
                                mat[i].append(matrix[i * 4 + j])
                        inverse_bind_matrices.append(mat)
                    print("INVM:", inverse_bind_matrices)

                    # Vertex weight information
                    num_offsets = len(vertex_weight_inputs)
                    print("NO:", num_offsets)

                    index = 0

                    for vc in vcounts:
                        thing = []
                        for i in range(vc): # influence calculations
                            rara = {}
                            for inp in vertex_weight_inputs:  # id
                                offset = vertex_weight_inputs[inp][1]
                                source = vertex_weight_inputs[inp][0]

                                true_index = index + offset
                                # print("TI", v_array[true_index], len(sources.get(source, [])))
                                rara[inp] = sources.get(source, [])[v_array[true_index]]
                            index += num_offsets
                            thing.append(rara)
                        vertex_weights.append(thing)
                    
                    print("VERTEX INFLUENCES:", str(vertex_weights))


                
                # endfor skin in controller
            # endfor controller in library
        elif "library_visual_scenes" in library.tag:
            # breadth first search for the highest armature node
            root = None
            qq = deque()
            qq.append(library)
            while len(qq):
                cur = qq.popleft()
                if cur.attrib.get("type", "") == "JOINT":
                    root = cur
                    break
                for child in cur:
                    qq.append(child)
            
            print("ROOT", get_info(root))

            parent = {}  # sid --> parent_sid
            transforms = {}  # sid --> transform
            
            qq.clear()
            qq.append(root)

            while len(qq):
                cur = qq.popleft()
                cur_sid = cur.attrib.get("sid", "")

                for child in cur:
                    if "node" == child.tag[-4:]:
                        child_sid = child.attrib.get("sid", "")
                        parent[child_sid] = (cur_sid)
                    elif "matrix" == child.tag[-6:]:
                        mat_array = [float(x) for x in child.text.split()]
                        mat = [[], [], [], []]
                        for i in range(4):
                            for j in range(4):
                                mat[i].append(mat_array[i * 4 + j])
                        transforms[cur_sid] = mat
                    elif "extra" == child.tag[-5:]:
                        for technique in child:
                            if "technique" in technique.tag:
                                tip = [None, None, None]
                                for xyz in technique:
                                    if "tip_x" == xyz.tag[-5:]:
                                        tip[0] = float(xyz.text)
                                    elif "tip_y" == xyz.tag[-5:]:
                                        tip[1] = float(xyz.text)
                                    elif "tip_z" == xyz.tag[-5:]:
                                        tip[2] = float(xyz.text)
                                if None not in tip:
                                    bone_tips[cur_sid] = tip
                                    absolute_tips.append(cur_sid)

                    
                    qq.append(child)

                

            
            print("PARENT:", parent)
            print("TRANS:", transforms)
            print("TIPS:", bone_tips)


    # Bone transform postprocessin

    bone_transform_printout = "std::unordered_map<std::string, Matrix4> " + OUTPUT_NAME + "_bonetrans({"
    
    hasElements = False
    for key in transforms:
        if hasElements:
            bone_transform_printout = bone_transform_printout + ", "
        hasElements = True  # Matrixes in collada are row major so use the row-wise constructor
        bone_transform_printout = bone_transform_printout + "{" + printout(key, "std::string") + ", " + "Matrix4("
        HE = False
        for r in transforms[key]:
            for c in r:
                if HE:
                    bone_transform_printout = bone_transform_printout + ", "
                HE = True
                bone_transform_printout = bone_transform_printout + str(c)
        
        bone_transform_printout = bone_transform_printout + ")}"
    
    bone_transform_printout = bone_transform_printout + "});"

    # Get a representative childrens of each bone
    bone_children = {}
    for bone in parent:
        bone_children[parent[bone]] = bone
    print("CHILDRENS:", bone_children)
    # Bone tip calculations. Bone tips are shown in the bone's relative transform (TRANS). This is just a convenience feature.

    for name in joint_names:
        if name in bone_tips:
            continue
        elif name not in bone_children or bone_children[name] not in transforms:
            bone_tips[name] = [0, 0, 1]
            continue
        cht = transforms[bone_children[name]]
        bone_tips[name] = [cht[0][3], cht[1][3], cht[2][3]]

    
    print("REAL TIPS:", bone_tips)

    # Bone tip printout
    bone_tip_printout = "std::unordered_map<std::string, Vector3> " + OUTPUT_NAME + "_bonetips({"
    hasElements = False
    for key in bone_tips:
        vec = bone_tips[key]
        if hasElements:
            bone_tip_printout = bone_tip_printout + ", "
        hasElements = True
        bone_tip_printout = bone_tip_printout + "{" + printout(key, "std::string") + ", " + "Vector3("
        bone_tip_printout = bone_tip_printout + str(vec[0]) + ", " + str(vec[1]) + ", " + str(vec[2])

        
        bone_tip_printout = bone_tip_printout + ")}"
    
    bone_tip_printout = bone_tip_printout + "});"


    return [
        print_vector_vector(vertex_positions, "Vector3", OUTPUT_NAME + "_v"), 
        print_vector_2d(tri_indices, "int", OUTPUT_NAME + "_t"),
        print_vector_vector(tex_coords, "Vector2", OUTPUT_NAME + "_tex"),
        print_vector_2d(texco, "int", OUTPUT_NAME + "_texco"),
        print_vector(joint_names, "std::string", OUTPUT_NAME + "_bones"),
        print_map(parent, "std::string", "std::string", OUTPUT_NAME + "_parent", True),
        bone_transform_printout, 
        bone_tip_printout,
        print_set(absolute_tips, "std::string", OUTPUT_NAME + "_atips")  # unused
    ]




if __name__ == "__main__":
    retval = parse(PREFIX + "/" + FILENAME + ".dae")
    final_output = ""
    for thing in retval:
        final_output = final_output + str(thing) + "\n"
    with open(OUTPUT, "w") as FII:

        FII.write(final_output)