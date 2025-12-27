#ifndef IMGTEX_EXT
#define IMGTEX_EXT

#include "../utils/Math.h"
#include "Material.h"

#include <vector>
#include <utility>

// If you want to set the base material params DO IT SEPARATELY
class ImageTexture : public BaseMaterial {
	public:
	std::vector<std::vector<uint32_t>> image; // Image pixels go left to right and top to bottom

	int W, H;

	ImageTexture(int w, int h);

	ImageTexture(std::vector<std::vector<uint32_t>> tex);

	ImageTexture(ImageTexture& other);

	std::pair<int, int> getTexel(Vector2 v);

	virtual uint32_t getColor(int x, int y) override;

	virtual uint32_t getColor(Vector2 v) override;

	void setTexel(int x, int y, uint32_t c);

	void setTexel(Vector2 v, uint32_t c);
};

#endif