# A program to render the output of Scene::buffer_data() to an image

from PIL import Image
import numpy as np
import cv2


def render(INPUT):
	file = open(INPUT, 'r')
	bufdat = file.read()
	
	ket = bufdat.find("]")
	dims = [int(i) for i in bufdat[1:ket].split(",")]

	W = dims[0]
	H = dims[1]
	
	im = Image.new(mode = "RGB", size = (W, H))

	restofdat = "".join(bufdat[ket+1:].splitlines())
	restofdat = [int(i) for i in restofdat.split(",")[:-1]]

	index = 0
	for y in range(H):
		for x in range(W):
			c = restofdat[index]
			r = (c>>24)
			g = (c>>16) % (1<<8)
			b = (c>>8) % (1<<8)
			a = c % (1<<8)
			im.putpixel((x, y), (r, g, b, a))
			index += 1

	file.close()

	return im

def make_gif(INPUT_DIR, OUTPUT, LEN):
	frames = [render(INPUT_DIR + "/frame" + str(i)) for i in range(LEN)]

	output = frames[0]

	output.save(OUTPUT, format="GIF", append_images = frames[1:], save_all = True, duration = 0.1, loop = 0)

if __name__ == "__main__":
	
	INPUT_DIR = "video"
	OUTPUT = "VIDEO.gif"
	lenfile = open(INPUT_DIR + "/LEN", 'r')

	LEN = int(lenfile.read())
	make_gif(INPUT_DIR, OUTPUT, LEN)