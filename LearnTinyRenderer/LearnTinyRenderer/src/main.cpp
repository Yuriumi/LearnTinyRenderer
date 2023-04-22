#include "tgaimage.h"
#include "model.h"

void line(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color);

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
Model* model = NULL;
const int width = 800;
const int height = 800;

int main()
{
	model = new Model("D:/TinyRenderer/Model/Lesson1/african_head.obj");

	TGAImage image(width, height, TGAImage::RGB);
	for (int i = 0; i < model->nfaces(); i++) {
		std::vector<int> face = model->face(i);
		for (int j = 0; j < 3; j++) {
			Vec3f v0 = model->vert(face[j]);
			Vec3f v1 = model->vert(face[(j + 1) % 3]);
			int x0 = (v0.x + 1.) * width / 2.;
			int y0 = (v0.y + 1.) * height / 2.;
			int x1 = (v1.x + 1.) * width / 2.;
			int y1 = (v1.y + 1.) * height / 2.;
			line(x0, y0, x1, y1, image, white);
		}
	}
	image.flip_vertically();
	image.write_tga_file("output.tga");

	delete model;
	return 0;
}

void line(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color)
{
#pragma region DDA
	/*float x = x0;
	float y = y0;

	float dx = x1 - x0;
	float dy = y1 - y0;

	float k{};

	if (std::abs(dy) >= std::abs(dx))
	{
		k = dx / dy;
		for (y; y < y1; y++)
		{
			image.set((int)x, y, color);
			x += k;
		}
	}
	else
	{
		k = dy / dx;
		for (x; x < x1; x++)
		{
			image.set(x, (int)y, color);
			y += k;
		}
	}*/
#pragma endregion

#pragma region Bresenham
	// 处理斜率绝对值大于1的直线
	bool steep = false;
	if (std::abs(x0 - x1) < std::abs(y0 - y1)) {
		std::swap(x0, y0);
		std::swap(x1, y1);
		steep = true;
	}
	// 交换起点终点的坐标
	if (x0 > x1) {
		std::swap(x0, x1);
		std::swap(y0, y1);
	}

	int y = y0;
	int dx = x1 - x0;
	int dy = y1 - y0;
	int eps{ 0 };
	int yi = 1;

	// 处理[-1,0]范围内的斜率
	if (dy < 0)
	{
		yi = -1;
		dy = -dy;
	}

	for (int x = x0; x <= x1; x++)
	{
		if (steep) {
			image.set(y, x, color);
		}
		else {
			image.set(x, y, color);
		}

		eps += dy;
		if (2 * eps >= dx)
		{
			y += yi;
			eps -= dx;
		}
	}
#pragma endregion
}