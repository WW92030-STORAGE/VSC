import sys
import argparse

'''

This program, given a file, generates a C++ function to return the contents of the file as a string.
The function is intended to belong in its own header file and has an #include <string> at the beginning.

'''

'''
input is the input file
name is the name of the string we save to
out is the output file
'''

# DEFAULT ARG VALUES
input = "misc/mcrproto.obj"
name = "protostring"
out = "misc/mcrproto.dat"

sn = "res"

def output():
	FILE = open(input, 'r')

	string = "#include <string>\nstd::string " + name + "() {\n"

	string = string + "\tstd::string " + sn + " = \"\";\n"

	for line in FILE:
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

'''

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