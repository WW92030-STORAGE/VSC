import sys, os, math
import xml.etree.ElementTree as ET

"""

!!!!!!!!!!!WARNING!!!!!!!!!!!

- This assumes your mesh adheres to a few rather limiting but simple assumptions. Everything else is undefined behavior!!!!

+ There is only one mesh in your DAE (This is achieved by CTRL+J on the meshes in Blender)
+ The mesh is only controlled by a single armature. There are no morphs or shape keys.

- Be aware of any unexpected behavior.

"""

# Constants

PREFIX = "../misc"
FILENAME = "mcrproto"
OUTPUT = FILENAME + ".dat"

def get_info(element):
    tag = element.tag
    attrib = element.attrib
    text = element.text
    if text is not None:
        text = str(text).strip()
    return f"Tag: {tag}, Attributes: {attrib}, Text: {text}"

def print_vector_2d(input: list[list], type: str = "int", name: str = "v"):
    retval = "std::vector<std::vector<" + type + ">> " + name + "({"

    for i in range(len(input)):
        if i > 0:
            retval = retval + ", "
        
        retval = retval + "{"
        for j in range(len(input[i])):
            if j > 0:
                retval = retval + ", "
            retval = retval + str(input[i][j])
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


def parse(FILE_PATH = PREFIX + "/" + FILENAME + ".dae", NAME = FILENAME):
    tree = ET.parse(FILE_PATH)
    root = tree.getroot()
    print("Tree: " + str(tree))
    print("ROOT: " + str(root))

    # Storage structures for the resultants

    vertex_positions = []
    tex_coords = []


    # Fuck around and find out!

    for library in root:
        if library is None:
            continue
        # print(get_info(child))

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
                                    count = int(thing.attrib.get("count", 0))
                                    text = thing.text.split()
                                    if "float_array" in thing.tag:
                                        array = [float(x) for x in text]
                                    elif "int_array" in thing.tag:
                                        array = [int(x) for x in text]
                                    elif "bool_array" in thing.tag:
                                        array = [bool(x) for x in text]
                                    else:
                                        array = text
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

                    print("v =", vertex_positions)
                    print("tex =", tex_coords)
                    print("t =", tri_indices)
                    print("texco =", texco)

    
    return [
        print_vector_vector(vertex_positions, "Vector3", NAME + "_v"), 
        print_vector_2d(tri_indices, "int", NAME + "_t"),
        print_vector_vector(tex_coords, "Vector2", NAME + "_tex"),
        print_vector_2d(texco, "int", NAME + "_texco")
    ]




if __name__ == "__main__":
    retval = parse(PREFIX + "/" + FILENAME + ".dae")
    final_output = ""
    for thing in retval:
        final_output = final_output + str(thing) + "\n"
    with open(OUTPUT, "w") as FII:

        FII.write(final_output)