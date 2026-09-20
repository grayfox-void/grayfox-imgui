#include "ui_icon.h"

#include <SDL3/SDL_surface.h>
/*
int gf_load_icon(struct ui_context *ctx, const char *bmp_path,
		 uint32_t *global_rgba_bitmap, int atlas_w, int atlas_h)
{
	struct ui_atlas *atlas = &ctx->atlas;
	SDL_Surface *surface = NULL;
	SDL_Surface *optimized = NULL;
	uint32_t glyph_id;
	struct ui_glyph *g = NULL;

	if (atlas->total_icons_loaded >= MAX_UI_ICONS) {
		return 1;
	}

	surface = SDL_LoadBMP(bmp_path);
	if (!surface)
		return 1;

	optimized = SDL_ConvertSurface(surface, SDL_PIXELFORMAT_RGBA32);
	SDL_DestroySurface(surface);
	if (!optimized)
		return 1;

	if (atlas->next_icon_y == 0) {
		atlas->next_icon_x = 0;
		atlas->next_icon_y = ICON_ROW_START_Y;
		atlas->current_row_max_h = 0;
	}

	if (atlas->next_icon_x + optimized->w > atlas_w) {
		atlas->next_icon_x = 0;
		atlas->next_icon_y += atlas->current_row_max_h + 4;
		atlas->current_row_max_h = 0;
	}

	if (atlas->next_icon_y + optimized->h > atlas_h) {
		SDL_DestroySurface(optimized);
		return 1;
	}

	uint32_t *src_pixels = (uint32_t *)optimized->pixels;
	for (int y = 0; y < optimized->h; y++) {
		for (int x = 0; x < optimized->w; x++) {
			int dest_x = atlas->next_icon_x + x;
			int dest_y = atlas->next_icon_y + y;

			global_rgba_bitmap[dest_y * atlas_w + dest_x] =
				src_pixels[y * optimized->w + x];
		}
	}

	glyph_id = (GLYPH_BANK_SIZE * UI_FONT_STYLE_COUNT) +
		   atlas->total_icons_loaded;
	g = &atlas->glyphs[glyph_id];

	g->w = (float)optimized->w * 2.0f;
	g->h = (float)optimized->h * 2.0f;
	g->x_offset = 0.0f;
	g->y_offset = 0.0f;
	g->advance_x = (float)optimized->w;
	g->advance_y = 0.0f;
	g->_unused = 0.0f;

	g->u = (uint16_t)atlas->next_icon_x;
	g->v = (uint16_t)atlas->next_icon_y;

	atlas->next_icon_x += optimized->w + 4;
	if (optimized->h > atlas->current_row_max_h) {
		atlas->current_row_max_h = optimized->h;
	}

	atlas->total_icons_loaded++;
	SDL_DestroySurface(optimized);

	return glyph_id;
}
*/
int gf_load_icon(struct ui_context *ctx, const char *bmp_path,
		 uint32_t *global_rgba_bitmap, int atlas_w, int atlas_h)
{
	struct ui_atlas *atlas = &ctx->atlas;
	SDL_Surface *surface = NULL;
	SDL_Surface *optimized = NULL;
	uint32_t glyph_id;
	struct ui_glyph *g = NULL;

	if (atlas->total_icons_loaded >= MAX_UI_ICONS) {
		return 1;
	}

	surface = SDL_LoadBMP(bmp_path);
	if (!surface)
		return 1;

	optimized = SDL_ConvertSurface(surface, SDL_PIXELFORMAT_RGBA32);
	SDL_DestroySurface(surface);
	if (!optimized)
		return 1;

	// Añadimos un margen de seguridad (borde) en el Atlas para evitar sangrado de textura (Bleeding)
	const int ATLAS_PADDING = 8;

	if (atlas->next_icon_y == 0) {
		atlas->next_icon_x = ATLAS_PADDING;
		atlas->next_icon_y = ICON_ROW_START_Y;
		atlas->current_row_max_h = 0;
	}

	if (atlas->next_icon_x + optimized->w + ATLAS_PADDING > atlas_w) {
		atlas->next_icon_x = ATLAS_PADDING;
		atlas->next_icon_y += atlas->current_row_max_h + ATLAS_PADDING;
		atlas->current_row_max_h = 0;
	}

	if (atlas->next_icon_y + optimized->h + ATLAS_PADDING > atlas_h) {
		SDL_DestroySurface(optimized);
		return 1;
	}

	uint32_t *src_pixels = (uint32_t *)optimized->pixels;
	for (int y = 0; y < optimized->h; y++) {
		for (int x = 0; x < optimized->w; x++) {
			int dest_x = atlas->next_icon_x + x;
			int dest_y = atlas->next_icon_y + y;

			global_rgba_bitmap[dest_y * atlas_w + dest_x] =
				src_pixels[y * optimized->w + x];
		}
	}

	glyph_id = (GLYPH_BANK_SIZE * UI_FONT_STYLE_COUNT) +
		   atlas->total_icons_loaded;
	g = &atlas->glyphs[glyph_id];

	// MATEMÁTICA DEFINITIVA PARA ACOPLAR CON OVERSAMPLING 2X2:
	// Para que (glyph->w * 0.5f) en pantalla dé el tamaño real del icono (ej. 16),
	// guardamos el doble en g->w.
	g->w = (float)optimized->w * 2.0f;
	g->h = (float)optimized->h * 2.0f;

	g->x_offset = 0.0f;
	g->y_offset = 0.0f;

	// El avance horizontal que va a retornar gf_put_glyph debe ser el ancho real en pantalla
	g->advance_x = (float)optimized->w;
	g->advance_y = 0.0f;
	g->_unused = 0.0f;

	// Coordenadas UV iniciales en el Atlas
	g->u = (uint16_t)atlas->next_icon_x;
	g->v = (uint16_t)atlas->next_icon_y;

	// Avanzamos el puntero del atlas sumando el tamaño del icono mas el padding de seguridad
	atlas->next_icon_x += optimized->w + ATLAS_PADDING;
	if (optimized->h > atlas->current_row_max_h) {
		atlas->current_row_max_h = optimized->h;
	}

	atlas->total_icons_loaded++;
	SDL_DestroySurface(optimized);

	return glyph_id;
}
