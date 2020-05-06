// bmp_program.cpp : Defines the entry point for the application.
//

#include "main.h"

void main()
{
	bitmap bitmap_file = bitmap_newBitmap(1231, -1234);
	bitmap* bitmap_file_ptr = &bitmap_file;

	struct color background_color = {
		0xCD,
		0xCD,
		0xCD
	}; // this is grey

	struct color line_color = {
		0x00,
		0x00,
		0x00
	}; // black for the line

	struct color rectangle_blue = {
		0x00,
		0x00,
		0xFF
	}; //pure blue

	struct color circle_red = {
		0xFF,
		0x00,
		0x00
	}; // pure red

	bitmap_fillImage(bitmap_file_ptr, &background_color); // make the entire background grey
	bitmap_drawLine(bitmap_file_ptr, &line_color, 0, 200, 300, 450); // weird line to satisfy JFR
	bitmap_drawLine(bitmap_file_ptr, &line_color, 300, 200, 0, 450); // weird line other direction to satisfy JFR
	bitmap_drawLine(bitmap_file_ptr, &line_color, 30, 20, 300, 100); // JFRs Line
	bitmap_drawLine(bitmap_file_ptr, &line_color, 100, 0, 100, 1000); // vertica line
	bitmap_drawLine(bitmap_file_ptr, &line_color, 0, 100, 1000, 100); // horizontal line
	bitmap_drawLine(bitmap_file_ptr, &line_color, 0, 0, bitmap_width(bitmap_file_ptr), bitmap_height(bitmap_file_ptr)); // diagonal top left to bottom right
	bitmap_drawLine(bitmap_file_ptr, &line_color, bitmap_width(bitmap_file_ptr), 0, 0, bitmap_height(bitmap_file_ptr)); // diagonal top right to bottom left
	bitmap_drawRect(bitmap_file_ptr, &rectangle_blue, 100, 100, 400, 400, false); // rectangle top left no fill
	bitmap_drawRect(bitmap_file_ptr, &rectangle_blue, 400, 400, 700, 700, true); // rectangle directly bottom right of the previous one with fill
	bitmap_drawCircle(bitmap_file_ptr, &circle_red, 700, 700, 100, false); // empty circle at 800,800 with radius 100
	bitmap_drawCircle(bitmap_file_ptr, &circle_red, 400, 700, 100, true); // filled circle at 500,800 with radius 100

	//printf("ImageData %d\n", imageData);

	printf("size of header: %zu\n", sizeof(bitmap_file.header));
	printf("size of dib_header: %zu\n", sizeof(bitmap_file.dib_header));
	//printf("Size of imageData: %zu\n", sizeof(&imageData));

	// open file write stream
	FILE* f = fopen("file.bmp", "wb");

	// check pointer real quick
	if (f != NULL) {
		// write garbage data to disk
		fwrite(&bitmap_file.header, sizeof(bitmap_file.header), 1, f);
		fwrite(&bitmap_file.dib_header, sizeof(bitmap_file.dib_header), 1, f);
		fwrite(bitmap_file.image_data_ptr, bitmap_file.dib_header.size_image, 1, f);

		// nicely close file otherwise JFR gets mad
		fclose(f);
	}

	// free data so JFR doesnt get memleaks in the think ways.
	// and since jfr keeps compalining i might aswell rm -rf / --no-preserve-root right now just to save myself from his nitpics
	free(bitmap_file.image_data_ptr);

	return 0;
}
