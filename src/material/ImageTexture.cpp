#include "../../include/utils/Math.h"
#include "../../include/material/Material.h"

#include <vector>
#include <utility>

#include "../../include/material/ImageTexture.h"

	ImageTexture::ImageTexture(int w, int h) {
		W = w;
		H = h;

		image = std::vector<std::vector<uint32_t>>(W, std::vector<uint32_t>(H, 0));

		TYPE = IMAGE;
	}

	ImageTexture::ImageTexture(std::vector<std::vector<uint32_t>> tex) {
		// std::cout << "IMG\n";
		W = tex.size();
		H = tex[0].size();
		for (auto i : tex) H = BASE::max(H, int(i.size()));

		image = std::vector<std::vector<uint32_t>>(W, std::vector<uint32_t>(H, 0));

		for (int i = 0; i < tex.size(); i++) {
			for (int j = 0; j < tex[i].size(); j++) image[i][j] = tex[i][j];
		}

		TYPE = IMAGE;
	}

	ImageTexture::ImageTexture(ImageTexture& other) {
		W = other.W;
		H = other.H;

		image = std::vector<std::vector<uint32_t>>(W, std::vector<uint32_t>(H, 0));

		for (int i = 0; i < other.image.size(); i++) {
			for (int j = 0; j < other.image[i].size(); j++) image[i][j] = other.image[i][j];
		}

		TYPE = IMAGE;
	}

	std::pair<int, int> ImageTexture::getTexel(Vector2 v) {
		int x = int(v.x * W);
		int y = int(v.y * H);
		if (x >= W) x = W - 1;
		if (y >= H) y = H - 1;

		return {x, y};
	}

	uint32_t ImageTexture::getColor(int x, int y) {
		if (x < 0) x = 0;
		if (y < 0) y = 0;
		if (x >= W) x = W - 1;
		if (y >= H) y = H - 1;
		return image[x][y];
	}

	uint32_t ImageTexture::getColor(Vector2 v) {
		auto p = getTexel(v);
		return getColor(p.first, p.second);
	}

	void ImageTexture::setTexel(int x, int y, uint32_t c) {
		if (x < 0) x = 0;
		if (y < 0) y = 0;
		if (x >= W) x = W - 1;
		if (y >= H) y = H - 1;

		image[x][y] = c;
	}

	void ImageTexture::setTexel(Vector2 v, uint32_t c) {
		auto p = getTexel(v);
		setTexel(p.first, p.second, c);
	}