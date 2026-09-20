#include "ui_font.h"
#include "stb_truetype.h"

#include <SDL3/SDL_surface.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
/*
int gf_load_font(struct ui_context *ctx, const char *font_path, float font_size,
		 ui_font_style style, uint32_t *global_rgba_bitmap, int atlas_w,
		 int atlas_h)
{
	size_t size;

	void *font_data = SDL_LoadFile(font_path, &size);
	if (!font_data)
		return 1;

	uint8_t *temp_bitmap = malloc(atlas_w * atlas_h);
	if (!temp_bitmap) {
		SDL_free(font_data);
		return 1;
	}
	memset(temp_bitmap, 0, atlas_w * atlas_h);

	stbtt_pack_context pc;
	if (!stbtt_PackBegin(&pc, temp_bitmap, atlas_w, atlas_h, atlas_w, 1,
			     NULL)) {
		free(temp_bitmap);
		SDL_free(font_data);
		return 1;
	}

	stbtt_PackSetOversampling(&pc, 2, 2);

	int vertical_offset_pixels = style * 256;
	pc.pixels = temp_bitmap + (vertical_offset_pixels * atlas_w);
	pc.height = 256;

	stbtt_packedchar packed_chars[FONT_CHAR_COUNT];
	int result = stbtt_PackFontRange(&pc, font_data, 0, -font_size,
					 FONT_FIRST_CHAR, FONT_CHAR_COUNT,
					 packed_chars);
	stbtt_PackEnd(&pc);

	if (!result) {
		free(temp_bitmap);
		SDL_free(font_data);
		return 1;
	}

	for (int i = 0; i < atlas_w * atlas_h; i++) {
		uint8_t alpha = temp_bitmap[i];
		if (alpha > 0) {
			global_rgba_bitmap[i] = ((uint32_t)alpha << 24) |
						(255 << 16) | (255 << 8) | 255;
		}
	}

	struct ui_atlas *atlas = &ctx->atlas;
	atlas->width = atlas_w;
	atlas->height = atlas_h;
	atlas->inv_w = 1.0f / (float)atlas_w;
	atlas->inv_h = 1.0f / (float)atlas_h;

	stbtt_fontinfo font_info;
	if (stbtt_InitFont(&font_info, font_data, 0)) {
		int raw_ascent, raw_descent, raw_line_gap;
		stbtt_GetFontVMetrics(&font_info, &raw_ascent, &raw_descent,
				      &raw_line_gap);

		float scale = stbtt_ScaleForPixelHeight(&font_info, font_size);

		if (style == UI_FONT_STYLE_REGULAR) {
			atlas->ascent = (float)raw_ascent * scale * 0.5f;
			atlas->descent = (float)raw_descent * scale * 0.5f;
			atlas->line_gap = (float)raw_line_gap * scale * 0.5f;
			atlas->font_size = font_size;
		}
	}

	int glyph_bank_offset = style * GLYPH_BANK_SIZE;

	for (int i = 0; i < FONT_CHAR_COUNT; i++) {
		int target_index = glyph_bank_offset + i;

		atlas->glyphs[target_index].u = packed_chars[i].x0;
		atlas->glyphs[target_index].v =
			packed_chars[i].y0 + vertical_offset_pixels;
		atlas->glyphs[target_index].w =
			(float)(packed_chars[i].x1 - packed_chars[i].x0);
		atlas->glyphs[target_index].h =
			(float)(packed_chars[i].y1 - packed_chars[i].y0);

		atlas->glyphs[target_index].x_offset =
			floorf(packed_chars[i].xoff * 0.5f + 0.5f);
		atlas->glyphs[target_index].y_offset =
			floorf(packed_chars[i].yoff * 0.5f + 0.5f);

		float display_w =
			(float)(packed_chars[i].x1 - packed_chars[i].x0) * 0.5f;
		//atlas->glyphs[target_index].advance_x =
		//	display_w + (packed_chars[i].xoff * 0.5f);
		atlas->glyphs[target_index].advance_x =
			packed_chars[i].xadvance * 0.5f;
		atlas->glyphs[target_index].advance_y = 0.0f;
		atlas->glyphs[target_index]._unused = 0.0f;
	}

	free(temp_bitmap);
	SDL_free(font_data);

	return 0;
}
*/
int gf_load_font(struct ui_context *ctx, const char *font_path, float font_size,
		 ui_font_style style, uint32_t *global_rgba_bitmap, int atlas_w,
		 int atlas_h)
{
	size_t size;
	void *font_data = SDL_LoadFile(font_path, &size);
	if (!font_data)
		return 1;

	uint8_t *temp_bitmap = malloc(atlas_w * atlas_h);
	if (!temp_bitmap) {
		SDL_free(font_data);
		return 1;
	}
	memset(temp_bitmap, 0, atlas_w * atlas_h);

	stbtt_pack_context pc;
	if (!stbtt_PackBegin(&pc, temp_bitmap, atlas_w, atlas_h, atlas_w, 1,
			     NULL)) {
		free(temp_bitmap);
		SDL_free(font_data);
		return 1;
	}

	stbtt_PackSetOversampling(&pc, 2, 2);

	int vertical_offset_pixels = style * 256;
	pc.pixels = temp_bitmap + (vertical_offset_pixels * atlas_w);
	pc.height = 256;

	stbtt_packedchar packed_chars[FONT_CHAR_COUNT];
	int result = stbtt_PackFontRange(&pc, font_data, 0, -font_size,
					 FONT_FIRST_CHAR, FONT_CHAR_COUNT,
					 packed_chars);
	stbtt_PackEnd(&pc);

	if (!result) {
		free(temp_bitmap);
		SDL_free(font_data);
		return 1;
	}

	// Recuperamos tus paneles: Solo escribimos si el pixel contiene fuentes (alpha > 0)
	for (int i = 0; i < atlas_w * atlas_h; i++) {
		uint8_t alpha = temp_bitmap[i];
		if (alpha > 0) {
			global_rgba_bitmap[i] = ((uint32_t)alpha << 24) |
						(255 << 16) | (255 << 8) | 255;
		}
	}

	struct ui_atlas *atlas = &ctx->atlas;
	atlas->width = atlas_w;
	atlas->height = atlas_h;
	atlas->inv_w = 1.0f / (float)atlas_w;
	atlas->inv_h = 1.0f / (float)atlas_h;

	stbtt_fontinfo font_info;
	if (stbtt_InitFont(&font_info, font_data, 0)) {
		int raw_ascent, raw_descent, raw_line_gap;
		stbtt_GetFontVMetrics(&font_info, &raw_ascent, &raw_descent,
				      &raw_line_gap);

		float scale = stbtt_ScaleForPixelHeight(&font_info, font_size);

		if (style == UI_FONT_STYLE_REGULAR) {
			// Las métricas verticales globales van directas a escala de pantalla
			atlas->ascent = (float)raw_ascent * scale;
			atlas->descent = (float)raw_descent * scale;
			atlas->line_gap = (float)raw_line_gap * scale;
			atlas->font_size = font_size;
		}
	}

	int glyph_bank_offset = style * GLYPH_BANK_SIZE;

	for (int i = 0; i < FONT_CHAR_COUNT; i++) {
		int target_index = glyph_bank_offset + i;

		// Guardamos las coordenadas UV y el tamaño completo de la textura en el atlas
		atlas->glyphs[target_index].u = packed_chars[i].x0;
		atlas->glyphs[target_index].v =
			packed_chars[i].y0 + vertical_offset_pixels;
		atlas->glyphs[target_index].w =
			(float)(packed_chars[i].x1 - packed_chars[i].x0);
		atlas->glyphs[target_index].h =
			(float)(packed_chars[i].y1 - packed_chars[i].y0);

		// CORRECCIÓN CRÍTICA: Los offsets espaciales nativos ya están correctos en pixeles de pantalla.
		// No se dividen por 0.5f; se usa redondeo directo para evitar desalineaciones verticales de letras como la 'e'.
		atlas->glyphs[target_index].x_offset =
			floorf(packed_chars[i].xoff + 0.5f);
		atlas->glyphs[target_index].y_offset =
			floorf(packed_chars[i].yoff + 0.5f);

		// El avance horizontal de la tipografía también está listo para usarse directamente
		atlas->glyphs[target_index].advance_x =
			packed_chars[i].xadvance;
		atlas->glyphs[target_index].advance_y = 0.0f;
		atlas->glyphs[target_index]._unused = 0.0f;
	}

	free(temp_bitmap);
	SDL_free(font_data);

	return 0;
}
