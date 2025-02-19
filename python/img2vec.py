from PIL import Image
import sys

INPUT = "img"
NAME = ""
INFILE = "Earth.jpeg"
OUTPUT = "misc"
OUTSUF = "_texture.dat"

def rgba(tup):
	THEINT = 69420
	thing = 256

	if (type(tup) == type(THEINT)):
		return tup

	if len(tup) == 3:
		return 255 + thing * (tup[2] + thing * (tup[1] + thing * tup[0]))
	return tup[3] + thing * (tup[2] + thing * (tup[1] + thing * tup[0]))

def img2tex():
	global NAME
	if len(NAME) <= 0:
		NAME = INFILE[:INFILE.find(".")]
	img = Image.open(INPUT + "/" + INFILE).convert("RGB")
	width, height = img.size

	res = "std::vector<std::vector<uint32_t>> " + NAME + "({"

	for i in range(width):
		if (i > 0):
			res = res + ","
		res = res + "std::vector<uint32_t>({"

		for j in range(height):
			if (j > 0):
				res = res + ","
			res = res + str(rgba(img.getpixel((i, j))))
		res = res + "})"
	res = res + "});"

	with open(OUTPUT + "/" + NAME + OUTSUF, "w") as OOO:
		OOO.write(res)

if __name__ == "__main__":
	img2tex()