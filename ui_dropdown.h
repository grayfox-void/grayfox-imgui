#ifndef __GF_IMGUI_UI_DROPDOWN_INCLUDED_H__
#define __GF_IMGUI_UI_DROPDOWN_INCLUDED_H__

#include "ui_base.h"
#include "ui_button.h"
#include "ui_common.h"
#include "ui_constants.h"
#include "ui_layout.h"
#include "ui_types.h"
#include <stdint.h>

static inline __attribute__((always_inline)) __attribute__((nonnull(1)))
__attribute__((const)) struct ui_dropdown_context *
_gf_dropdown_ctx(const struct ui_layout *layout)
{
	return (struct ui_dropdown_context *)&layout
		->dropdown_ctx[layout->dd_ctx_ptr];
}

static inline void gf_dropdown_push(struct ui_layout *restrict layout)
{
	assert(layout->dd_ctx_ptr < MAX_LAYOUT_CTX);

	int sp = ++layout->dd_ctx_ptr;

	// TODO check active element id before set values to 0
	// if the active id is the same DO NOT set values to 0

	layout->dropdown_ctx[sp].height = 0.0f;
	layout->dropdown_ctx[sp].label_max = 0.0f;
	layout->dropdown_ctx[sp].width = 0.0f;
	layout->dropdown_ctx[sp].first_render = 0;
}

static inline void gf_dropdown_pop(struct ui_layout *layout,
				   struct ui_dropdown_context *out)
{
	assert(layout->dd_ctx_ptr > UI_LAYOUT_SP_START);

	int sp = layout->dd_ctx_ptr--;

	out->height = layout->dropdown_ctx[sp].height;
	out->label_max = layout->dropdown_ctx[sp].label_max;
	out->width = layout->dropdown_ctx[sp].width;
}

static inline void gf_dropdown_reset(struct ui_layout *restrict layout)
{
	layout->dd_ctx_ptr = UI_LAYOUT_SP_START;

	struct ui_dropdown_context *dd = &layout->dropdown_ctx[0];

	dd->height = 0.0f;
	dd->label_max = 0.0f;
	dd->width = 0.0f;
	dd->first_render = 0;
}

static inline void gf_draw_dd_borders(struct ui_vertex *ui,
				      struct ui_layout *layout,
				      const struct ui_rect rect)
{
	float thickness = UI_BORDER_THICKNESS;

	// right border
	gf_push_rect_geometry_xy(ui, layout,
				 (struct ui_rect){ rect.x + rect.w, rect.y,
						   thickness, rect.h },
				 COLOR_BORDER);

	// bottom border
	gf_push_rect_geometry_xy(ui, layout,
				 (struct ui_rect){ rect.x, rect.y + rect.h,
						   rect.w, thickness },
				 COLOR_BORDER);

	// left border
	gf_push_rect_geometry_xy(ui, layout,
				 (struct ui_rect){ rect.x, rect.y, thickness,
						   rect.h },
				 COLOR_BORDER);
}

static inline void gf_dropdown_begin(struct ui_context *ctx)
{
	struct ui_layout *layout = &ctx->layout;
	struct ui_theme *theme = &ctx->theme;

	struct ui_layout_context *parent = _gf_parent_ctx(layout);
	struct ui_dropdown_context *dd = _gf_dropdown_ctx(layout);
	struct ui_rect rect = { 0 };

	rect.x = parent->cursor.x;
	rect.y = parent->cursor.y + parent->bounds_h + 1.0;
	// TODO add a no_wrap variable to the context
	rect.h = 10000.0f;

	gf_layout_push(layout, rect, UI_DIRECTION_VERTICAL);
	// gf_dropdown_push(layout);

	gf_increase_z_idx(layout);

	dd->items = 0;
}

static inline void gf_dd_separator(struct ui_context *ctx)
{
	struct ui_vertex *ui = &ctx->ui;
	struct ui_layout *layout = &ctx->layout;
	struct ui_theme *theme = &ctx->theme;

	struct ui_layout_context *parent = _gf_parent_ctx(layout);
	struct ui_dropdown_context *dd = _gf_dropdown_ctx(layout);
	struct ui_rect rect;
	uint32_t color;

	GF_RECT_ASSIGN(rect, gf_rect_limits, theme, layout, UI_RECT_SEPARATOR);
	color = gf_rect_color(theme, UI_RECT_SEPARATOR);

	// float dd_width = layout->dropdown_width;
	rect.w = dd->width;
	gf_push_rect_geometry_xy(ui, layout, rect, color);

	// layout->dropdown_height += rect.h;
	gf_layout_move_cursor(layout, rect.w, rect.h);
	dd->height += dd->first_render ? 0 : UI_BORDER_THICKNESS;
}

static inline void gf_dropdown_end(struct ui_context *ctx)
{
	struct ui_vertex *ui = &ctx->ui;
	struct ui_layout *layout = &ctx->layout;
	struct ui_layout_context self = { 0 };
	struct ui_dropdown_context dd_self = { 0 };
	struct ui_dropdown_context *dd = _gf_dropdown_ctx(layout);

	gf_layout_pop_ctx(layout, &self);
	// gf_dropdown_pop(layout, &dd_self);

	gf_draw_dd_borders(
		ui, layout,
		(struct ui_rect){ self.bounds_x, self.bounds_y,
				  // dd_self.width, dd_self.height });
				  dd->width, dd->height });
	dd->first_render = 1;
	gf_decrease_z_idx(layout);
}

static inline int gf_dd_item(struct ui_context *ctx, uint32_t control_id,
			     uint32_t icon_id, const char *label,
			     const char *shortcut)
{
	struct ui_vertex *ui = &ctx->ui;
	struct ui_layout *layout = &ctx->layout;
	struct ui_theme *theme = &ctx->theme;
	struct ui_atlas *atlas = &ctx->atlas;
	struct ui_input *input = &ctx->input;

	struct ui_rect rect = { 0 };
	struct ui_size icon = { 0 };
	struct ui_dropdown_context *dd = _gf_dropdown_ctx(layout);

	icon = ui_glyph_size(atlas, icon_id);
	float text_w = gf_get_text_width(atlas, label);
	float shct_w = gf_get_text_width(atlas, shortcut);
	float item_w = 24.0f + icon.w + text_w + shct_w + PAD_MENU_H * 2;

	dd->width = dd->width < item_w ? item_w : dd->width;
	dd->items += dd->first_render ? 0 : 1;

	rect = _gf_rect_limits_w(layout, dd->width);

	// TODO this will be removed once dynamic styling is implemented
	rect.h = UI_DDITEM_HEIGHT;

	int click = _gf_draw_rect_ev_hover_click(ui, layout, theme, input, rect,
						 UI_RECT_DROPDOWNITEM);

	float ctrl_x = PAD_MENU_H;
	float ctrl_y = (rect.h * 0.5f) - 2.0f;

	// ui_control(ctx, control_id, ctrl_x, ctrl_y, 0xDDDDDDFF);

	float icon_x = PAD_MENU_H + 24.0f;
	float icon_y = (icon.h * 0.5f) - 2.0f;

	ui_icon(ctx, icon_id, icon_x, icon_y, 0xDDDDDDFF);

	float text_x = PAD_MENU_H + 24.0f + icon.w;
	float text_y = (rect.h * 0.5f);

	ui_text(ctx, label, UI_FONT_STYLE_REGULAR, text_x, text_y, 0xFFFFFFFF);

	float shct_x = dd->width - shct_w - PAD_MENU_H;

	// TODO set up shortcut in keyboard

	ui_text(ctx, shortcut, UI_FONT_STYLE_REGULAR, shct_x, text_y,
		0xFFFFFFFF);

	dd->height += dd->first_render ? 0 : rect.h;

	gf_layout_move_cursor(layout, rect.w, rect.h);

	printf("dd width: %f\n", dd->width);

	return click;
}

#endif // __GF_IMGUI_UI_DROPDOWN_INCLUDED_H__
