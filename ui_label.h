#ifndef __GF_IMGUI_UI_LABEL_INCLUDED_H__
#define __GF_IMGUI_UI_LABEL_INCLUDED_H__

#include "ui_common.h"
#include "ui_layout.h"
#include "ui_types.h"

static inline void gf_label(struct ui_context *ctx, const char *text)
{
	struct ui_vertex *ui = &ctx->ui;
	struct ui_layout *layout = &ctx->layout;
	struct ui_theme *theme = &ctx->theme;
	struct ui_atlas *atlas = &ctx->atlas;

	struct ui_layout_context *parent = _gf_parent_ctx(layout);
	uint32_t color;

	float text_w = gf_get_text_width(atlas, text);
	ui_text(ctx, text, UI_FONT_STYLE_REGULAR, 0, parent->bounds_h * 0.5f,
		0xFFFFFFFF);

	// printf("x: %f\ny: %f\n", rect.x, rect.y);

	parent->cursor.x += text_w;

	// printf("Creating label in z: %d\n", layout->z_index);
}

#endif // __GF_IMGUI_UI_LABEL_INCLUDED_H__
