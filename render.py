# A program to render the output of Scene::buffer_data() to an image

from PIL import Image
# Important constants

INPUT = "BUFFER" # Where to read the buffer from



def render():
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

	im.show()

	file.close()


if __name__ == "__main__":
	render()