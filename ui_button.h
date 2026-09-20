#ifndef __GF_IMGUI_UI_BUTTON_INCLUDED_H__
#define __GF_IMGUI_UI_BUTTON_INCLUDED_H__

#include "ui_base.h"
#include "ui_common.h"
#include "ui_constants.h"
#include "ui_font.h"
#include "ui_icon.h"
#include "ui_input.h"
#include "ui_layout.h"
#include "ui_rect.h"
#include "ui_types.h"
#include <stdint.h>

static inline __attribute__((always_inline))
__attribute__((nonnull(1, 2, 3, 4))) int
_gf_draw_rect_ev_click(struct ui_vertex *restrict ui,
		       struct ui_layout *restrict layout,
		       const struct ui_theme *restrict theme,
		       const struct ui_input *restrict input,
		       const struct ui_rect rect, int ui_rect_id)
{
	assert(ui != NULL);
	assert(layout != NULL);
	assert(theme != NULL);
	assert(input != NULL);

	uint32_t color = gf_rect_color(theme, ui_rect_id);

	int m_ev = gf_mouse_event(rect, input);
	uint32_t mask = -(GF_MOUSE_EV_CLICK(m_ev));

	gf_push_rect_geometry_xy(ui, layout, rect,
				 GF_COLOR_MASK(color, COLOR_BTN_CLICK, mask));

	return GF_MOUSE_EV_CLICK(m_ev);
}

static inline __attribute__((always_inline))
__attribute__((nonnull(1, 2, 3, 4))) int
_gf_draw_rect_ev_hover_click(struct ui_vertex *restrict ui,
			     struct ui_layout *restrict layout,
			     const struct ui_theme *restrict theme,
			     const struct ui_input *restrict input,
			     const struct ui_rect rect, int ui_rect_id)
{
	assert(ui != NULL);
	assert(layout != NULL);
	assert(theme != NULL);
	assert(input != NULL);

	uint32_t color = gf_rect_color(theme, ui_rect_id);

	int m_ev = gf_mouse_event(rect, input);
	uint32_t mask = -(GF_MOUSE_EV_HOVER(m_ev));

	gf_push_rect_geometry_xy(ui, layout, rect,
				 GF_COLOR_MASK(color, COLOR_BTN_CLICK, mask));

	return GF_MOUSE_EV_CLICK(m_ev);
}

static inline int gf_button_action(struct ui_context *ctx, const char *label)
{
	struct ui_vertex *ui = &ctx->ui;
	struct ui_layout *layout = &ctx->layout;
	struct ui_theme *theme = &ctx->theme;
	struct ui_atlas *atlas = &ctx->atlas;
	struct ui_input *input = &ctx->input;

	struct ui_rect rect = _gf_rect_limits_text(layout, atlas, label);
	int click = _gf_draw_rect_ev_hover_click(ui, layout, theme, input, rect,
						 UI_RECT_BUTTON);

	float text_x = PAD_MENU_H;
	float text_y = (rect.h * 0.5f);

	ui_text(ctx, label, UI_FONT_STYLE_REGULAR, text_x, text_y, 0xFFFFFFFF);

	gf_draw_borders(ui, layout,
			(struct ui_rect){ rect.x, rect.y + PAD_MENU_V, rect.w,
					  rect.h - PAD_MENU_V * 2 });

	gf_layout_move_cursor(layout, rect.w, rect.h);

	return click;
}

static inline void gf_draw_button(struct ui_context *ctx, uint32_t icon,
				  const char *label,
				  struct ui_input *out_minput)
{
	struct ui_vertex *ui = &ctx->ui;
	struct ui_layout *layout = &ctx->layout;
	struct ui_theme *theme = &ctx->theme;
	struct ui_atlas *atlas = &ctx->atlas;
	struct ui_input *input = &ctx->input;

	struct ui_rect rect = { 0 };

	rect = _gf_rect_limits_text(layout, atlas, label);
	int click = _gf_draw_rect_ev_hover_click(ui, layout, theme, input, rect,
						 UI_RECT_BUTTON);

	struct ui_size icon_size = ui_glyph_size(atlas, icon);

	float icon_x = PAD_MENU_H; // 24.0 action width
	// TODO temporary calculation - 2.0f because the icon don't have the real
	// height
	float icon_y = (icon_size.h * 0.5f) - 2.0f;

	// ui_icon(ctx, icon, icon_x, icon_y, 0xDDDDDDFF);

	float text_x = PAD_MENU_H + icon_size.w;
	float text_y = (rect.h * 0.5f);

	printf("2 - layout x: %f\n layout y: %f\n", text_x, text_y);
	ui_text(ctx, label, UI_FONT_STYLE_REGULAR, text_x, text_y, 0xFFFFFFFF);

	gf_draw_borders(ui, layout,
			(struct ui_rect){ rect.x, rect.y + PAD_MENU_V, rect.w,
					  rect.h - PAD_MENU_V * 2 });
}

static inline int gf_button_i_action(struct ui_context *ctx, uint32_t icon_id,
				     const char *label)
{
	struct ui_vertex *ui = &ctx->ui;
	struct ui_layout *layout = &ctx->layout;
	struct ui_theme *theme = &ctx->theme;
	struct ui_atlas *atlas = &ctx->atlas;
	struct ui_input *input = &ctx->input;

	struct ui_rect rect = { 0 };
	struct ui_size icon = { 0 };

	icon = ui_glyph_size(atlas, icon_id);
	float text_w = gf_get_text_width(atlas, label);

	rect = _gf_rect_limits_w(layout, icon.w + text_w);

	float icon_x = PAD_MENU_H;
	float icon_y = (icon.h * 0.5f) - 2.0f;

	float text_x = PAD_MENU_H + icon.w;
	float text_y = (rect.h * 0.5f);

	int click = _gf_draw_rect_ev_hover_click(ui, layout, theme, input, rect,
						 UI_RECT_BUTTON);

	ui_icon(ctx, icon_id, icon_x, icon_y, 0xDDDDDDFF);
	ui_text(ctx, label, UI_FONT_STYLE_REGULAR, text_x, text_y, 0xFFFFFFFF);

	gf_layout_move_cursor(layout, rect.w, rect.h);

	gf_draw_borders(ui, layout,
			(struct ui_rect){ rect.x, rect.y + PAD_MENU_V, rect.w,
					  rect.h - PAD_MENU_V * 2 });

	return click;
}

static inline int gf_button_ic_action(struct ui_context *ctx,
				      uint32_t control_id, uint32_t icon_id,
				      const char *label)
{
	struct ui_vertex *ui = &ctx->ui;
	struct ui_layout *layout = &ctx->layout;
	struct ui_theme *theme = &ctx->theme;
	struct ui_atlas *atlas = &ctx->atlas;
	struct ui_input *input = &ctx->input;

	struct ui_rect rect = { 0 };
	struct ui_size icon = { 0 };

	icon = ui_glyph_size(atlas, icon_id);
	float text_w = gf_get_text_width(atlas, label);

	rect = _gf_rect_limits_w(layout, 24.0f + icon.w + text_w);

	float ctrl_x = PAD_MENU_H;
	float ctrl_y = (rect.h * 0.5f) - 2.0f;

	float icon_x = PAD_MENU_H; // 24.0 action width
	float icon_y = (icon.h * 0.5f) - 2.0f;

	float text_x = PAD_MENU_H + icon.w;
	float text_y = (rect.h * 0.5f);

	int click = _gf_draw_rect_ev_hover_click(ui, layout, theme, input, rect,
						 UI_RECT_BUTTON);

	// TODO implement action (i.e checkbox)
	// ui_control(ctx, control_id, ctrl_x, ctrl_y);
	ui_icon(ctx, icon_id, icon_x, icon_y, 0xDDDDDDFF);
	ui_text(ctx, label, UI_FONT_STYLE_REGULAR, text_x, text_y, 0xFFFFFFFF);

	gf_draw_borders(ui, layout,
			(struct ui_rect){ rect.x, rect.y + PAD_MENU_V, rect.w,
					  rect.h - PAD_MENU_V * 2 });

	gf_layout_move_cursor(layout, rect.w, rect.h);

	return click;
}

static inline int gf_button_ics_action(struct ui_context *ctx,
				       uint32_t control_id, uint32_t icon_id,
				       const char *label, const char *shortcut)
{
	struct ui_vertex *ui = &ctx->ui;
	struct ui_layout *layout = &ctx->layout;
	struct ui_theme *theme = &ctx->theme;
	struct ui_atlas *atlas = &ctx->atlas;
	struct ui_input *input = &ctx->input;

	struct ui_rect rect = { 0 };
	struct ui_size icon = { 0 };

	icon = ui_glyph_size(atlas, icon_id);
	float text_w = gf_get_text_width(atlas, label);
	float shct_w = gf_get_text_width(atlas, shortcut);

	rect = _gf_rect_limits_w(layout, 24.0f + icon.w + text_w + shct_w);

	float ctrl_x = PAD_MENU_H;
	float ctrl_y = (rect.h * 0.5f) - 2.0f;

	float icon_x = PAD_MENU_H;
	float icon_y = (icon.h * 0.5f) - 2.0f;

	float text_x = PAD_MENU_H + icon.w;
	float text_y = (rect.h * 0.5f);

	int click = _gf_draw_rect_ev_hover_click(ui, layout, theme, input, rect,
						 UI_RECT_BUTTON);

	// TODO implement action (i.e checkbox)
	// ui_action(ctx, ctrl_id, ctrl_x, ctrl_y);
	ui_icon(ctx, icon_id, icon_x, icon_y, 0xDDDDDDFF);
	ui_text(ctx, label, UI_FONT_STYLE_REGULAR, text_x, text_y, 0xFFFFFFFF);

	float shct_x = rect.w - shct_w - PAD_MENU_H;

	// TODO set up shortcut in keyboard
	ui_text(ctx, shortcut, UI_FONT_STYLE_REGULAR, shct_x, text_y,
		0xFFFFFFFF);

	gf_draw_borders(ui, layout,
			(struct ui_rect){ rect.x, rect.y + PAD_MENU_V, rect.w,
					  rect.h - PAD_MENU_V * 2 });

	gf_layout_move_cursor(layout, rect.w, rect.h);

	return click;
}

#endif // __GF_IMGUI_UI_BUTTON_INCLUDED_H__
