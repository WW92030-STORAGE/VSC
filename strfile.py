import sys
import argparse


'''
input is the input file
name is the name of the string we save to
out is the output file
'''

# DEFAULT ARG VALUES
input = "misc/mcrproto.obj"
name = "protostring"
out = "misc/mcrproto.dat"

def output():
	FILE = open(input, 'r')

	string = "std::string " + name + " = \"\";\n"

	for line in FILE:
		string = string + name + " = " + name + "+ \"" + line.strip() + "\\n\";\n"
	
	with open(out, "w") as OUTPUTUT:
		OUTPUTUT.write(string)

'''

-input = specify the input file path
-name = specify the string name
-out = specify the output file path

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