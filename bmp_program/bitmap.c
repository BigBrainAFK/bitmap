/*
This is an exercise proposed by Teufelchen#2781 to help me (BigBrainAFK#0001) become the next CLang god

99% of this code exists only cause JFR told me to do it this way.

So objectives:
- Understand the file format -- DONE
- Write software that creates empty bmps -- DONE
- Write functions to draw line/rects/circles -- DONE
- Draw funny pictures -- maybe done?
*/

#include "bitmap.h"

void drawLineHigh(const bitmap* bmp_file, const color* line_color, long int x0, long int y0, long int x1, long int y1);
void drawLineLow(const bitmap* bmp_file, const color* line_color, long int x0, long int y0, long int x1, long int y1);

void symmetryFilled(const bitmap* bmp_file, const color* line_color, long int x, long int y, long int xc, long int yc);
void symmetry(const bitmap* bmp_file, const color* line_color, long int x, long int y, long int xc, long int yc);

size_t alignUp(size_t size, size_t alignment);

bitmap bitmap_newBitmap(int32_t width, int32_t height) {
	bitmap_header header = {
		0x4D42, // set type to "BM" which is the most common in any mdoern system/software
		0, // size of the whole bitmap in bytes
		0, // reserved and only used by certain software
		54 // offset where the image data starts which here is fixed due to not much functionality being used
	};

	bitmap_dib_header dib_header = {
		40, //size of header
		width, //width
		height, //height
		1, //planes should always be 1
		24, //bit depth usually best left at 24
		0, //compression
		0, //size of image in bytes
		2835, //ppm in x axis in this case its equal to 72ppi
		2835, //ppm in y axis in this case its equal to 72ppi
		0, // color table used
		0 // important colors used
	};

	bitmap bitmap_file = {
		header,
		dib_header
	};

	//calculate size of image data
	bitmap_file.dib_header.size_image = (uint32_t)(bitmap_linePitch(&bitmap_file) * bitmap_getHeight(&bitmap_file));
	bitmap_file.header.file_size = sizeof(header) + sizeof(dib_header) + dib_header.size_image;

	//allocate memory for image data
	bitmap_file.image_data_ptr = (uint8_t*)malloc(bitmap_file.dib_header.size_image);

	struct color white = {
		0xFF,
		0xFF,
		0xFF
	};

	// set entire image to white
	bitmap_fillImage(&bitmap_file, &white);

	return bitmap_file;
}

void bitmap_drawCircle(const bitmap* bitmap_file_ptr, const color* line_color_ptr, long int x_center, long int y_center, long int radius, bool filled) {
	int x = 0;
	int y = radius;
	int p = 1 - radius;

	for (x = 0; y > x; x++) {
		if (p < 0)
			p += 2 * x + 3;
		else {
			p += 2 * (x - y) + 5;
			y--;
			if (filled == true && y != x) {
				symmetryFilled(bitmap_file_ptr, line_color_ptr, y, x, x_center, y_center);
			}
		}

		if (filled == true) {
			symmetryFilled(bitmap_file_ptr, line_color_ptr, x, y, x_center, y_center);
		}
		else {
			symmetry(bitmap_file_ptr, line_color_ptr, x, y, x_center, y_center);
		}
	}
}

void bitmap_drawRect(const bitmap* bitmap_file_ptr, const color* line_color_ptr, long int x0, long int y0, long int x1, long int y1, bool filled) {
	//calculate pitch of each line
	size_t line_length = bitmap_linePitch(bitmap_file_ptr);

	if (y1 > abs(bitmap_file_ptr->dib_header.height)) {
		y1 = abs(bitmap_file_ptr->dib_header.height) - 1;
	}

	if (x1 > bitmap_file_ptr->dib_header.width) {
		x1 = bitmap_file_ptr->dib_header.width - 1;
	}

	// loop through all lines and the entire width in pixels and set each pixel to the color given
	long int line, row;
	for (line = y0; line <= y1; line++) { //renegate height since we store a negative value for top-to-bottom pixel rows
		for (row = x0; row <= x1; row++) {
			if (line > y0 && line < y1 && row > x0 && row < x1 && filled == false) continue;
			bitmap_drawPixel(bitmap_file_ptr, line_color_ptr, row, line);
		}
	}
}

void drawLineHigh(const bitmap* bitmap_file_ptr, const color* line_color_ptr, long int x0, long int y0, long int x1, long int y1) {
	//calculate pitch of each line
	size_t line_length = bitmap_linePitch(bitmap_file_ptr);

	// calculate delta of x and y to know the direction of the line
	long int deltax = x1 - x0;
	long int deltay = y1 - y0;
	long int D = 2 * deltax - deltay;
	long int xi = 1;

	if (deltax < 0) {
		xi = -1;
		deltax = -deltax;
	}

	// loop through the entire images "height" from y0 to y1
	long int x = x0;;
	long int y;
	for (y = y0; y < y1; y++) {
		bitmap_drawPixel(bitmap_file_ptr, line_color_ptr, x, y);

		if (D > 0) {
			x = x + xi;
			D = D - 2 * deltay;
		}

		D = D + 2 * deltax;
	}
}

void drawLineLow(const bitmap* bitmap_file_ptr, const color* line_color_ptr, long int x0, long int y0, long int x1, long int y1) {
	//calculate pitch of each line
	size_t line_length = bitmap_linePitch(bitmap_file_ptr);

	// calculate delta of x and y to know the direction of the line
	long int deltax = x1 - x0;
	long int deltay = y1 - y0;
	long int D = 2 * deltay - deltax;
	long int yi = 1;

	if (deltay < 0) {
yi = -1;
deltay = -deltay;
	}

	// loop through the entire lines "width" from x0 to x1
	long int y = y0;
	long int x;
	for (x = x0; x < x1; x++) {
		bitmap_drawPixel(bitmap_file_ptr, line_color_ptr, x, y);

		if (D > 0) {
			y = y + yi;
			D = D - 2 * deltax;
		}

		D = D + 2 * deltay;
	}
}

void bitmap_drawLine(const bitmap* bitmap_file_ptr, const color* line_color_ptr, long int x0, long int y0, long int x1, long int y1) {
	if (abs(y1 - y0) < abs(x1 - x0)) {
		if (x0 > x1) {
			drawLineLow(bitmap_file_ptr, line_color_ptr, x1, y1, x0, y0);
		}
		else {
			drawLineLow(bitmap_file_ptr, line_color_ptr, x0, y0, x1, y1);
		}
	}
	else {
		if (y0 > y1) {
			drawLineHigh(bitmap_file_ptr, line_color_ptr, x1, y1, x0, y0);
		}
		else {
			drawLineHigh(bitmap_file_ptr, line_color_ptr, x0, y0, x1, y1);
		}
	}
}

void bitmap_drawPixel(const bitmap* bitmap_file_ptr, const color* pixel_color_ptr, long int x, long int y) {
	//calculate pitch of each line
	size_t line_length = bitmap_linePitch(bitmap_file_ptr);
	uint8_t* pixel;

	if (bitmap_file_ptr->dib_header.height > 0) {
		pixel = &bitmap_file_ptr->image_data_ptr[(bitmap_file_ptr->dib_header.height - (y + 1)) * line_length + x * 3 * sizeof(uint8_t)];
	}
	else {
		pixel = &bitmap_file_ptr->image_data_ptr[y * line_length + x * 3 * sizeof(uint8_t)];
	}

	pixel[0] = pixel_color_ptr->blue;
	pixel[1] = pixel_color_ptr->green;
	pixel[2] = pixel_color_ptr->red;
}

void bitmap_fillImage(const bitmap* bitmap_file_ptr, const color* color_ptr) {
	//calculate pitch of each line
	size_t line_length = bitmap_linePitch(bitmap_file_ptr);

	// loop through all lines and the entire width in pixels and set each pixel to the color given
	long int line, row;
	for (line = 0; line < abs(bitmap_file_ptr->dib_header.height); line++) {
		for (row = 0; row < bitmap_file_ptr->dib_header.width; row++) {
			bitmap_drawPixel(bitmap_file_ptr, color_ptr, row, line);
		}
	}
}

size_t bitmap_linePitch(const bitmap* bitmap_file_ptr) {
	size_t pitch = bitmap_file_ptr->dib_header.width * 3 * sizeof(uint8_t);
	return alignUp(pitch, 4);
}

int32_t bitmap_getHeight(const bitmap* bitmap_file_ptr) {
	return abs(bitmap_file_ptr->dib_header.height);
}

int32_t bitmap_getWidth(const bitmap* bitmap_file_ptr) {
	return abs(bitmap_file_ptr->dib_header.width);
}

void symmetryFilled(const bitmap* bitmap_file_ptr, const color* line_color_ptr, long int x, long int y, long int xc, long int yc) {
	// instead of coloring in the individual pixels of each pair draw a line between them
	bitmap_drawLine(bitmap_file_ptr, line_color_ptr, xc + x, yc - y, xc + x, yc + y);

	if (x != 0) {
		bitmap_drawLine(bitmap_file_ptr, line_color_ptr, xc - x, yc - y, xc - x, yc + y);
	}
}

void symmetry(const bitmap* bitmap_file_ptr, const color* line_color_ptr, long int x, long int y, long int xc, long int yc) {
	// color in all symetrical pixel pairs
	bitmap_drawPixel(bitmap_file_ptr, line_color_ptr, xc + x, yc - y);
	bitmap_drawPixel(bitmap_file_ptr, line_color_ptr, xc - x, yc - y);

	bitmap_drawPixel(bitmap_file_ptr, line_color_ptr, xc + y, yc - x);
	bitmap_drawPixel(bitmap_file_ptr, line_color_ptr, xc - y, yc - x);

	bitmap_drawPixel(bitmap_file_ptr, line_color_ptr, xc + y, yc + x);
	bitmap_drawPixel(bitmap_file_ptr, line_color_ptr, xc - y, yc + x);

	bitmap_drawPixel(bitmap_file_ptr, line_color_ptr, xc + x, yc + y);
	bitmap_drawPixel(bitmap_file_ptr, line_color_ptr, xc - x, yc + y);
}

size_t alignUp(size_t size, size_t alignment) {
	// calculate remainder to align properly
	size_t remainder = size % alignment;
	size_t padding = remainder > 0 ? alignment - remainder : 0;
	// return inverse which is the missing bits
	return size + padding;
}