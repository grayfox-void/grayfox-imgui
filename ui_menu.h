#ifndef __GF_IMGUI_UI_MENU_INCLUDED_H__
#define __GF_IMGUI_UI_MENU_INCLUDED_H__

#include "ui_common.h"
#include "ui_constants.h"
#include "ui_dropdown.h"
#include "ui_input.h"
#include "ui_layout.h"
#include "ui_types.h"

static inline void gf_menu_begin(struct ui_context *ctx)
{
	struct ui_vertex *ui = &ctx->ui;
	struct ui_layout *layout = &ctx->layout;
	struct ui_theme *theme = &ctx->theme;

	struct ui_layout_context *parent = _gf_parent_ctx(layout);
	struct ui_rect rect = { .x = parent->cursor.x,
				.y = parent->cursor.y,
				.w = 0.0,
				.h = parent->bounds_h };

	// gf_rect_limits(theme, layout, UI_RECT_MENU, &rect);

	gf_layout_push(layout, rect, UI_DIRECTION_HORIZONTAL);

	printf("Creating menu in z index: %d\n", layout->z_index);
}

static inline void gf_menu_end(struct ui_context *ctx)
{
	struct ui_layout *layout = &ctx->layout;
	struct ui_layout_context self;

	gf_layout_pop_ctx(layout, &self);
	gf_layout_move_cursor(layout, self.bounds_w, self.bounds_h);
}

static inline int gf_menu_action(struct ui_context *ctx, const char *label)
{
	struct ui_vertex *ui = &ctx->ui;
	struct ui_layout *layout = &ctx->layout;
	struct ui_theme *theme = &ctx->theme;
	struct ui_atlas *atlas = &ctx->atlas;
	struct ui_input *input = &ctx->input;

	struct ui_layout_context *parent = _gf_parent_ctx(layout);

	struct ui_rect rect = { 0 };
	uint32_t color = 0;

	GF_RECT_ASSIGN(rect, gf_rect_limits, theme, layout, UI_RECT_MENU);
	color = gf_rect_color(theme, UI_RECT_MENU);

	printf("Creating menu %s\n", label);

	float text_w = gf_get_text_width(atlas, label);
	float item_w = text_w + PAD_MENU_H * 2.0f;

	rect.w = item_w;
	printf("text width: %f\n", text_w);

	printf("Rect: x: %f, y: %f, w: %f, h: %f\n", rect.x, rect.y, rect.w,
	       rect.h);

	int m_ev = gf_mouse_event(rect, input);
	uint32_t mask = -(GF_MOUSE_EV_CLICK(m_ev));
	uint32_t item_color = GF_COLOR_MASK(color, COLOR_BTN_CLICK, mask);

	gf_push_rect_geometry_xy(ui, layout, rect, item_color);

	float text_x = PAD_MENU_H;
	float text_y = parent->cursor.y + (rect.h * 0.5f);

	printf("2 - layout x: %f\n layout y: %f\n", text_x, text_y);
	ui_text(ctx, label, UI_FONT_STYLE_REGULAR, text_x, text_y, 0xFFFFFFFF);

	// Update width bounds
	parent->bounds_w = item_w;

	return GF_MOUSE_EV_CLICK(m_ev) & GF_MOUSE_EV_HOVER(m_ev);
}

static inline void gf_menudd_begin(struct ui_context *ctx)
{
	gf_dropdown_begin(ctx);
}

static inline int gf_menuitem_action(struct ui_context *ctx,
				     uint32_t control_id, uint32_t icon_id,
				     const char *label, char *shortcut)
{
	return gf_dd_item(ctx, control_id, icon_id, label, shortcut);
}

static inline void gf_menuitem_separator(struct ui_context *ctx)
{
	gf_dd_separator(ctx);
}

static inline void gf_menudd_end(struct ui_context *ctx)
{
	gf_dropdown_end(ctx);
}

#endif // __GF_IMGUI_UI_MENU_INCLUDED_H__
