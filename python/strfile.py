import sys
import argparse

'''

This program, given a file, generates a C++ function to return the contents of the file as a string.
The function is intended to belong in its own header file and has an #include <string> at the beginning.

Although this works on any file it is most suited for .obj (wavefront) files and has special flags for those.
Use them at your own risk!

'''

'''
input is the input file
name is the name of the string we save to
out is the output file

ignore-t = ignore texture data
ignore-n = ignore normal data
ignore-useless = ignore comments and blank lines
'''

# DEFAULT ARG VALUES
input = "misc/mcrproto.obj"
name = "protostring"
out = "misc/mcrproto.dat"

ignore_t = False
ignore_n = False
ignore_useless = False

sn = "res"

def output():
	FILE = open(input, 'r')

	string = "#include <string>\nstd::string " + name + "() {\n"

	string = string + "\tstd::string " + sn + " = \"\";\n"

	for line in FILE:
		if ignore_t and (len(line) >= 2 and line[0:2] == "vt"):
			continue
		if ignore_n and (len(line) >= 2 and line[0:2] == "vn"):
			continue
		if ignore_useless and (len(line.strip()) <= 0 or line[0] == '#'):
			continue
		string = string + "\t" + sn + " = " + sn + "+ \"" + line.strip() + "\\n\";\n"
	
	string = string + "\treturn " + sn + ";\n}"
	
	with open(out, "w") as OUTPUTUT:
		OUTPUTUT.write(string)

'''

-input = specify the input file path
-name = specify the string name
-out = specify the output file path

-ignore-t = ignore textures
-ignore-n = ignore normals
-ignore-useless = ignore blank lines/comments

'''

if __name__ == "__main__":

	parser = argparse.ArgumentParser(
                    prog='strfile.py',
                    description='Convert file contents to C++ string',
                    epilog='...')

	parser.add_argument('-input', default = input)
	parser.add_argument('--name', default = name)
	parser.add_argument('-out', default = out)
	parser.add_argument("-ignore_t", action = "store_true")
	parser.add_argument("-ignore_n", action = "store_true")
	parser.add_argument("-ignore_useless", action = "store_true")

	args = parser.parse_args()

	input = args.input
	name = args.name
	out = args.out

	ignore_t = args.ignore_t
	ignore_n = args.ignore_n
	ignore_useless = args.ignore_useless
	output()