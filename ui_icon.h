#ifndef __GF_IMGUI_UI_ICON_INCLUDED_H__
#define __GF_IMGUI_UI_ICON_INCLUDED_H__

#include "ui_glyph.h"
#include "ui_types.h"

static inline void ui_icon(struct ui_context *ctx, uint32_t icon, float x,
			   float y, uint32_t color)
{
	struct ui_vertex *ui = &ctx->ui;
	struct ui_layout *layout = &ctx->layout;
	struct ui_atlas *atlas = &ctx->atlas;
	struct ui_glyph *glyph = NULL;

	glyph = gf_get_icon_glyph(atlas, icon, UI_FONT_STYLE_REGULAR);
	gf_put_icon_glyph(ui, layout, glyph, (const struct ui_pos){ x, y },
			  color);
}

int gf_load_icon(struct ui_context *ctx, const char *bmp_path,
		 uint32_t *global_rgba_bitmap, int atlas_w, int atlas_h);

#endif // __GF_IMGUI_UI_ICON_INCLUDED_H__
