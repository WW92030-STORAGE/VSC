import sys
import argparse

'''

This program, given a .obj file, creates VSC-compatible C++ vectors of vertices and triangle indices.
It returns a file containing C++ code initializing the vectors.

WARNING - This assumes your .obj has been properly triangulated and all faces are triangles.

'''

'''
input is the input file
name is the name of the string we save to
out is the output file
'''

# DEFAULT ARG VALUES
input = "misc/mcrproto.obj"
name = "proto"
out = "misc/mcrproto.dat"

def output():
	FILE = open(input, 'r')

	vertices = []

	verts = "std::vector<Vector3> " + name + "_verts({"
	tris = "std::vector<std::vector<int>> " + name + "_tris({"

	for line in FILE:
		sp = line.split()
		if (len(sp) <= 0):
			continue

		if (sp[0] == 'v'):
			verts = verts + "Vector3(" + sp[1] + "," + sp[2] + "," + sp[3] + "),"
			vertices.append(sp[1:])
		elif (sp[0] == 'f'):
			
			tris = tris + "{"
			
			for i in range(3):
				if (i > 0):
					tris = tris + ","
				ss = sp[i + 1]
				index = int(ss[:ss.find('/')]) - 1
				tris = tris + str(index)
			
			tris = tris + "},"

	verts = verts[:-1]
	tris = tris[:-1]

	verts = verts + "});"
	tris = tris + "});"

	with open(out, 'w') as OUT:
		OUT.write(verts + "\n" + tris)




if __name__ == "__main__":

	parser = argparse.ArgumentParser(
                    prog='strfile.py',
                    description='Convert file contents to C++ string',
                    epilog='...')

	parser.add_argument('-input', default = input)
	parser.add_argument('--name', default = name)
	parser.add_argument('-out', default = out)

	args = parser.parse_args()

	input = args.input
	name = args.name
	out = args.out
	output()