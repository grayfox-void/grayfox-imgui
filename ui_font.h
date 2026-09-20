#ifndef __GF_IMGUI_UI_FONT_INCLUDED_H__
#define __GF_IMGUI_UI_FONT_INCLUDED_H__

#include "ui_glyph.h"
#include "ui_types.h"

#include <assert.h>
#include <stdio.h>

static inline float gf_get_text_width(struct ui_atlas *atlas, const char *text)
{
	assert(text != NULL);

	float total_width = 0.0f;

	while (*text) {
		uint8_t c = *text;

		int idx = (int)c - FONT_FIRST_CHAR;

		int mask = (idx >= 0 & idx < FONT_CHAR_COUNT);
		idx = idx * mask;
		struct ui_glyph *glyph = &atlas->glyphs[idx];
		total_width += glyph->advance_x;

		text++;
	}

	return total_width;
}

static inline void ui_text(struct ui_context *ctx, const char *text,
			   ui_font_style style, float x, float y,
			   uint32_t color)
{
	struct ui_vertex *ui = &ctx->ui;
	struct ui_layout *layout = &ctx->layout;
	struct ui_atlas *atlas = &ctx->atlas;
	struct ui_glyph *glyph = NULL;

	float current_x = x;

	while (*text) {
		glyph = gf_get_text_glyph(atlas, *text, style);
		current_x += gf_put_glyph(ui, layout, glyph,
					  (const struct ui_pos){ current_x, y },
					  color);
		text++;
	}
}

static inline void gf_text(struct ui_context *ctx, const char *text,
			   ui_font_style style, float x, float y,
			   uint32_t color)
{
	struct ui_vertex *ui = &ctx->ui;
	struct ui_layout *layout = &ctx->layout;
	struct ui_atlas *atlas = &ctx->atlas;
	struct ui_glyph *glyph = NULL;

	float current_x = x;

	while (*text) {
		glyph = gf_get_text_glyph(atlas, *text, style);
		current_x += gf_put_glyph_(
			ui, layout, glyph,
			(const struct ui_pos){ current_x, y }, color);
		text++;
	}
}

int gf_load_font(struct ui_context *ctx, const char *font_path, float font_size,
		 ui_font_style style, uint32_t *global_rgba_bitmap, int atlas_w,
		 int atlas_h);

#endif // __GF_IMGUI_UI_FONT_INCLUDED_H__
