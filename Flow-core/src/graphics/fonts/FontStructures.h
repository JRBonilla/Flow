#pragma warning( disable: 4244 )
#pragma once

#include <iostream>
#include <algorithm>
#include <ft2build.h>
#include FT_FREETYPE_H
#include "../Texture.h"
#include "../../utils/Log.h"

namespace fl { namespace graphics {

#define MAX_ATLAS_WIDTH 512

	// Stores the glyph metrics for each character
	struct FontCharacter
	{
		float advance_x;
		float advance_y;

		float width;
		float height;
		
		float left;
		float top;

		float offset_x;
		float offset_y;
	};

	// Creates a large texture of characters containing all of the latin alphabet
	struct FontTextureAtlas
	{
		TextureParameters parameters = { TextureFormat::RED, TextureFilter::LINEAR, TextureWrap::CLAMP_TO_EDGE };
		Texture* texture;
	
		int width, height;
		FontCharacter characters[128];

		FontTextureAtlas(FT_Face face, int size)
		{
			FT_Set_Pixel_Sizes(face, 0, size);
			FT_GlyphSlot glyph = face->glyph;
			
			int row_width = 0;
			int row_height = 0;
			width = 0;
			height = 0;

			memset(characters, 0, sizeof(FontCharacter));

			// Set the width and height of the atlas based on the width & height of each character
			for (int i = 32; i < 128; i++)
			{
				if (FT_Load_Char(face, i, FT_LOAD_RENDER))
				{
					LOG("Loading character " << i << " failed!");
					continue;
				}

				if (row_width + glyph->bitmap.width + 1 >= MAX_ATLAS_WIDTH)
				{
					width = std::max<int>(width, row_width);
					height += row_height;
					row_width = 0;
					row_height = 0;
				}

				row_width += glyph->bitmap.width + 1;
				row_height = std::max<int>(row_height, glyph->bitmap.rows);
			}

			width = std::max<int>(width, row_width);
			height += row_height;

			texture = new Texture(width, height, true, parameters);
			texture->Bind(0);

			int xoffset = 0;
			int yoffset = 0;

			row_height = 0;

			// Create the texture atlas
			for (int i = 32; i < 128; i++)
			{
				if (FT_Load_Char(face, i, FT_LOAD_RENDER))
				{
					fprintf(stderr, "ERROR::FONT: Loading character %c failed!\n", i);
					continue;
				}

				if (xoffset + glyph->bitmap.width + 1 >= MAX_ATLAS_WIDTH)
				{
					yoffset += row_height;
					row_height = 0;
					xoffset = 0;
				}

				texture->SetData(glyph->bitmap.buffer, xoffset, yoffset, glyph->bitmap.width, glyph->bitmap.rows);

				characters[i].advance_x = glyph->advance.x >> 6;
				characters[i].advance_y = glyph->advance.y >> 6;

				characters[i].width = glyph->bitmap.width;
				characters[i].height = glyph->bitmap.rows;

				characters[i].left = glyph->bitmap_left;
				characters[i].top = glyph->bitmap_top;

				characters[i].offset_x = xoffset / static_cast<float>(width);
				characters[i].offset_y = yoffset / static_cast<float>(height);

				xoffset += glyph->bitmap.width + 1;
				row_height = std::max<int>(row_height, glyph->bitmap.rows);
			}
			LOG("Generated a " << width << " x " << height << " (" << width * height / 1024 << "kb) texture atlas.");
			FT_Done_Face(face);
		}

		~FontTextureAtlas()
		{
			delete texture;
		}
	};

}}
