/**
 * GLUS - OpenGL 3 Utilities. Copyright (C) 2010 Norbert Nopper
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "GL/glus.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

GLUSboolean GLUSAPIENTRY glusLoadTextFile(const GLUSchar* filename, GLUStextfile* textfile)
{
	FILE* f;

	if (!filename || !textfile)
	{
		return GLUS_FALSE;
	}

	textfile->text = 0;

	textfile->length = -1;

	f = fopen(filename, "r");

	if (!f)
	{
		return GLUS_FALSE;
	}

	fseek(f, 0, SEEK_END);

	textfile->length = ftell(f);

	textfile->text = (GLUSchar*) malloc(textfile->length + 1);

	if (!textfile->text)
	{
		textfile->length = -1;

		return GLUS_FALSE;
	}

	memset(textfile->text, 0, textfile->length + 1);

	rewind(f);

	fread(textfile->text, 1, textfile->length, f);

	fclose(f);

	return GLUS_TRUE;
}

GLUSvoid GLUSAPIENTRY glusDestroyTextFile(GLUStextfile* textfile)
{
	if (!textfile)
	{
		return;
	}

	if (textfile->text)
	{
		free(textfile->text);

		textfile->text = 0;
	}

	textfile->length = -1;
}

GLUSvoid glusSwapColorChannel(GLUSuint width, GLUSuint height, GLUSenum format, GLUSubyte* data)
{
	GLUSuint i;
	GLUSubyte temp;
	GLUSubyte bytesPerPixel = 3;

	if (format == GLUS_RGBA)
	{
		bytesPerPixel = 4;
	}

	// swap the R and B values to get RGB since the bitmap color format is in BGR
	for (i = 0; i < width * height * bytesPerPixel; i += bytesPerPixel)
	{
		temp = data[i];
		data[i] = data[i + 2];
		data[i + 2] = temp;
	}
}

GLUSboolean GLUSAPIENTRY glusLoadTgaImage(const GLUSchar* filename, GLUStgaimage* tgaimage)
{
	FILE* file;
	GLUSubyte type;
	GLUSubyte bitsPerPixel;

	// check, if we have a valid pointer
	if (!filename || !tgaimage)
	{
		return GLUS_FALSE;
	}

	// open filename in "read binary" mode
	file = fopen(filename, "rb");

	if (!file)
	{
		return GLUS_FALSE;
	}

	// seek through the tga header, up to the type:
	fseek(file, 2, SEEK_CUR);

	// read the type
	fread(&type, 1, 1, file);

	// check the type	
	if (type != 2 && type != 10 && type != 11)
	{
		fclose(file);
		return GLUS_FALSE;
	}

	// seek through the tga header, up to the width/height:
	fseek(file, 9, SEEK_CUR);

	// read the width
	fread(&tgaimage->width, 2, 1, file);

	// read the height 
	fread(&tgaimage->height, 2, 1, file);

	// read the bits per pixel
	fread(&bitsPerPixel, 1, 1, file);

	// check the pixel depth
	if (bitsPerPixel != 8 && bitsPerPixel != 24 && bitsPerPixel != 32)
	{
		fclose(file);
		return GLUS_FALSE;
	}
	else
	{
		tgaimage->format = GLUS_ALPHA;
		if (bitsPerPixel == 24)
		{
			tgaimage->format = GLUS_RGB;
		}
		else if (bitsPerPixel == 32)
		{
			tgaimage->format = GLUS_RGBA;
		}
	}

	// move file pointer to beginning of targa data
	fseek(file, 1, SEEK_CUR);

	// allocate enough memory for the targa  data
	tgaimage->data = (GLUSubyte*) malloc(tgaimage->width * tgaimage->height * bitsPerPixel / 8);

	// verify memory allocation
	if (!tgaimage->data)
	{
		fclose(file);
		return GLUS_FALSE;
	}

	if (type == 2)
	{
		// read in the raw data
		fread(tgaimage->data, 1, tgaimage->width * tgaimage->height * bitsPerPixel / 8, file);
	}
	else
	{
		// RLE encoded
		GLUSuint pixelsRead = 0;

		while (pixelsRead < (GLUSuint) tgaimage->width * tgaimage->height)
		{
			GLUSubyte amount;

			fread(&amount, 1, 1, file);

			if (amount & 0x80)
			{
				GLUSuint i;
				GLUSuint k;

				amount &= 0x7F;

				amount++;

				// read in the rle data
				fread(&tgaimage->data[pixelsRead * bitsPerPixel / 8], 1, bitsPerPixel / 8, file);

				for (i = 1; i < amount; i++)
				{
					for (k = 0; k < (GLUSuint) bitsPerPixel / 8; k++)
					{
						tgaimage->data[(pixelsRead + i) * bitsPerPixel / 8 + k] = tgaimage->data[pixelsRead * bitsPerPixel / 8 + k];
					}
				}
			}
			else
			{
				amount &= 0x7F;

				amount++;

				// read in the raw data
				fread(&tgaimage->data[pixelsRead * bitsPerPixel / 8], 1, (GLUSuint) amount * bitsPerPixel / 8, file);
			}

			pixelsRead += amount;
		}
	}

	// swap the color if necessary
	if (bitsPerPixel == 24 || bitsPerPixel == 32)
	{
		glusSwapColorChannel(tgaimage->width, tgaimage->height, tgaimage->format, tgaimage->data);
	}

	// close the file
	fclose(file);

	return GLUS_TRUE;
}

GLUSvoid GLUSAPIENTRY glusDestroyTgaImage(GLUStgaimage* tgaimage)
{
	if (!tgaimage)
	{
		return;
	}

	if (tgaimage->data)
	{
		free(tgaimage->data);

		tgaimage->data = 0;
	}

	tgaimage->width = 0;

	tgaimage->height = 0;

	tgaimage->format = 0;
}
