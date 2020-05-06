#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <stddef.h>

#pragma pack(push, 1)
typedef struct bitmap_header {
	uint16_t type;
	uint32_t file_size;
	uint32_t reserved;
	uint32_t pixel_offset;
} bitmap_header;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct bitmap_dib_header {
	uint32_t size;
	int32_t width;
	int32_t height;
	uint16_t planes;
	uint16_t bit_depth;
	uint32_t compression;
	uint32_t size_image;
	int32_t x_ppm;
	int32_t y_ppm;
	uint32_t color_used;
	uint32_t color_important;
} bitmap_dib_header;
#pragma pack(pop)

typedef struct bitmap {
	bitmap_header header;
	bitmap_dib_header dib_header;
	uint8_t* image_data_ptr;
} bitmap;

typedef struct color {
	uint8_t red;
	uint8_t green;
	uint8_t blue;
} color;

bitmap bitmap_newBitmap(int32_t width, int32_t height);

void bitmap_drawRect(const bitmap* bitmap_file_prt, const color* line_color_ptr, long int x0, long int y0, long int x1, long int y1, bool filled);
void bitmap_drawCircle(const bitmap* bitmap_file_prt, const color* line_color_ptr, long int xc, long int yc, long int radius, bool filled);
void bitmap_drawLine(const bitmap* bitmap_file_prt, const color* line_color_ptr, long int x0, long int y0, long int x1, long int y1);
void bitmap_drawPixel(const bitmap* bitmap_file_prt, const color* line_color_ptr, long int x, long int y);

void bitmap_fillImage(const bitmap* bitmap_file_prt, const color* color_ptr);

size_t bitmap_linePitch(const bitmap* bitmap_file_prt);
int32_t bitmap_height(const bitmap* bitmap_file_ptr);
int32_t bitmap_width(const bitmap* bitmap_file_ptr);
