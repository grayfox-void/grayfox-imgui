#ifndef __GF_IMGUI_UI_SELECTOR_INCLUDED_H__
#define __GF_IMGUI_UI_SELECTOR_INCLUDED_H__

#include "ui_common.h"
#include "ui_layout.h"
#include "ui_rect.h"
#include "ui_types.h"

static inline void gf_selector(struct ui_context *ctx, const char *options[],
			       uint32_t length)
{
	struct ui_vertex *ui = &ctx->ui;
	struct ui_layout *layout = &ctx->layout;
	struct ui_theme *theme = &ctx->theme;
	struct ui_atlas *atlas = &ctx->atlas;

	struct ui_layout_context *parent = _gf_parent_ctx(layout);
	uint32_t color;

	color = gf_rect_color(theme, UI_RECT_MENUBAR);

	struct ui_rect rect = { parent->cursor.x,
				parent->cursor.y + parent->bounds_h * 0.5f -
					24.0f * 0.5f,
				parent->bounds_w, 24.0f };

	gf_push_rect_geometry_xy(ui, layout, rect, 0x212121FF);

	gf_draw_borders(ui, layout, rect);

	/*gf_push_rect_geometry_xy(
		 *      ui, layout,
		 *      (struct ui_rect){rect.x + 1, rect.y + 1, rect.w - 1, 22.0f - 1},
		 *      0x2F2F2FFF);
		 *
		 *  gf_push_rect_geometry_xy(ui, layout,
		 *                           (struct ui_rect){rect.x, rect.y + 22, rect.w, 1.0f},
		 *                           COLOR_BORDER);
		 */
	float text_w = gf_get_text_width(atlas, "");
	ui_text(ctx, "", UI_FONT_STYLE_REGULAR,
		(parent->bounds_w - text_w) * 0.5, 22 * 0.5, 0xFFFFFFFF);

	printf("x: %f\ny: %f\n", rect.x, rect.y);

	printf("Creating window in z: %d\n", layout->z_index);
}

#endif // __GF_IMGUI_UI_SELECTOR_INCLUDED_H__
