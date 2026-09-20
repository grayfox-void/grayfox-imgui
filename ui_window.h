#ifndef __GF_IMGUI_UI_WINDOW_INCLUDED_H__
#define __GF_IMGUI_UI_WINDOW_INCLUDED_H__

#include "ui_common.h"
#include "ui_constants.h"
#include "ui_font.h"
#include "ui_glyph.h"
#include "ui_input.h"
#include "ui_rect.h"
#include "ui_types.h"

static inline void gf_window_begin(struct ui_context *ctx, const char *label,
				   float width, float height)
{
	struct ui_vertex *ui = &ctx->ui;
	struct ui_layout *layout = &ctx->layout;
	struct ui_theme *theme = &ctx->theme;
	struct ui_atlas *atlas = &ctx->atlas;
	struct ui_input *input = &ctx->input;

	struct ui_layout_context *root = &layout->ctx[0];
	uint32_t color = 0;

	color = gf_rect_color(theme, UI_RECT_MENUBAR);

	float x = (root->bounds_w - width) * 0.5f;
	float y = (root->bounds_h - height) * 0.5f;

	struct ui_rect rect = { x, y, width, height };

	/* window */
	gf_push_rect_geometry_xy(ui, layout, rect, color);

	struct ui_rect rect2 = { rect.x, rect.y, rect.w, 22.0f };

	/* top bar */
	gf_push_rect_geometry_xy(ui, layout, rect2, 0x2F2F2FFF);

	gf_push_rect_geometry_xy(ui, layout,
				 (struct ui_rect){ rect.x, rect.y + 22.0f,
						   rect.w, 1.0f },
				 COLOR_BORDER);

	/* end topbar */

	/* topbar move area */
	uint8_t m_ev_move = gf_mouse_event(rect2, input);

	/* top bar text */
	float text_w = gf_get_text_width(atlas, label);
	gf_text(ctx, label, UI_FONT_STYLE_REGULAR,
		(rect.x + width * 0.5f) - text_w * 0.5f,
		(rect.y + 22.0f * 0.5f), 0xFFFFFFFF);

	/* borders */
	gf_draw_borders(ui, layout, rect);

	// rect.x += 8.0f;
	rect.y += 22.0f; // + 8.0f;

	gf_layout_push(layout, rect, UI_DIRECTION_HORIZONTAL);

	gf_increase_z_idx(layout);

	printf("x: %f\ny: %f\n", rect.x, rect.y);

	printf("Creating window in z: %d\n", layout->z_index);
}

static inline void gf_window_end(struct ui_context *ctx)
{
	struct ui_layout *layout = &ctx->layout;

	gf_layout_pop(layout);
	gf_decrease_z_idx(layout);
}

static inline void gf_window_resizable_begin(struct ui_context *ctx,
					     const char *label, float width,
					     float height)
{
	struct ui_vertex *ui = &ctx->ui;
	struct ui_layout *layout = &ctx->layout;
	struct ui_theme *theme = &ctx->theme;
	struct ui_atlas *atlas = &ctx->atlas;
	struct ui_input *input = &ctx->input;

	struct ui_layout_context *root = &layout->ctx[0];
	uint32_t color;

	color = gf_rect_color(theme, UI_RECT_MENUBAR);

	float x = (root->bounds_w - width) * 0.5f;
	float y = (root->bounds_h - height) * 0.5f;

	struct ui_rect rect = { x, y, width, height };

	/* window */
	gf_push_rect_geometry_xy(ui, layout, rect, color);

	struct ui_rect rect2 = { rect.x, rect.y, rect.w, 22.0f };

	/* top bar */
	gf_push_rect_geometry_xy(ui, layout, rect2, 0x2F2F2FFF);

	gf_push_rect_geometry_xy(ui, layout,
				 (struct ui_rect){ rect.x, rect.y + 22.0f,
						   rect.w, 1.0f },
				 COLOR_BORDER);

	/* end topbar */

	/* topbar move area */
	uint8_t m_ev_move = gf_mouse_event(rect2, input);

	/* resize border events */
	uint8_t m_ev_rs_t = gf_mouse_event(
		(struct ui_rect){ rect.x, rect.y - UI_EVENT_BORDER_SIZE, rect.w,
				  UI_EVENT_BORDER_SIZE },
		input);
	uint8_t m_ev_rs_r =
		gf_mouse_event((struct ui_rect){ rect.x + rect.w, rect.y,
						 UI_EVENT_BORDER_SIZE, rect.h },
			       input);
	uint8_t m_ev_rs_b =
		gf_mouse_event((struct ui_rect){ rect.x, rect.y + rect.h,
						 rect.w, UI_EVENT_BORDER_SIZE },
			       input);
	uint8_t m_ev_rs_l = gf_mouse_event(
		(struct ui_rect){ rect.x - UI_EVENT_BORDER_SIZE, rect.y,
				  UI_EVENT_BORDER_SIZE, rect.h },
		input);

	/* resize vertex events */
	uint8_t m_ev_rs_tl =
		gf_mouse_event((struct ui_rect){ rect.x - UI_EVENT_BORDER_SIZE,
						 rect.y - UI_EVENT_BORDER_SIZE,
						 UI_EVENT_BORDER_SIZE + 10.0f,
						 UI_EVENT_BORDER_SIZE + 10.0f },
			       input);
	uint8_t m_ev_rs_tr = gf_mouse_event(
		(struct ui_rect){ rect.x + rect.w - UI_EVENT_BORDER_SIZE,
				  rect.y - UI_EVENT_BORDER_SIZE,
				  UI_EVENT_BORDER_SIZE + 10.0f,
				  UI_EVENT_BORDER_SIZE + 10.0f },
		input);
	uint8_t m_ev_rs_bl = gf_mouse_event(
		(struct ui_rect){ rect.x + rect.w - UI_EVENT_BORDER_SIZE,
				  rect.y + rect.h - UI_EVENT_BORDER_SIZE,
				  UI_EVENT_BORDER_SIZE + 10.0f,
				  UI_EVENT_BORDER_SIZE + 10.0f },
		input);
	uint8_t m_ev_rs_br = gf_mouse_event(
		(struct ui_rect){ rect.x - UI_EVENT_BORDER_SIZE,
				  rect.y + rect.h - UI_EVENT_BORDER_SIZE,
				  UI_EVENT_BORDER_SIZE + 10.0f,
				  UI_EVENT_BORDER_SIZE + 10.0f },
		input);

	/* top bar text */
	float text_w = gf_get_text_width(atlas, label);
	gf_text(ctx, label, UI_FONT_STYLE_REGULAR,
		(rect.x + width * 0.5f) - text_w * 0.5f,
		(rect.y + 22.0f * 0.5f), 0xFFFFFFFF);

	/* borders */
	gf_draw_borders(ui, layout, rect);

	rect.x += 8.0f;
	rect.y += 22.0f + 8.0f;

	gf_layout_push(layout, rect, UI_DIRECTION_HORIZONTAL);

	gf_increase_z_idx(layout);

	printf("x: %f\ny: %f\n", rect.x, rect.y);

	printf("Creating window in z: %d\n", layout->z_index);
}

static inline void gf_window_resizable_end(struct ui_context *ctx)
{
	struct ui_layout *layout = &ctx->layout;

	gf_layout_pop(layout);
	gf_decrease_z_idx(layout);
}

static inline void gf_window_footer_begin(struct ui_context *ctx)
{
	struct ui_vertex *ui = &ctx->ui;
	struct ui_layout *layout = &ctx->layout;
	struct ui_theme *theme = &ctx->theme;
	struct ui_atlas *atlas = &ctx->atlas;
	struct ui_input *input = &ctx->input;

	struct ui_layout_context *parent = _gf_parent_ctx(layout);
	uint32_t color;

	color = gf_rect_color(theme, UI_RECT_BORDER);

	struct ui_rect rect = { parent->bounds_x,
				parent->bounds_y + parent->bounds_h - 70.0f,
				parent->bounds_w, 1.0f };

	gf_layout_push(layout, rect, UI_DIRECTION_HORIZONTAL);

	/* bottom buttons */
	gf_push_rect_geometry_xy(ui, layout, rect, color);
}

static inline void gf_window_footer_end(struct ui_context *ctx)
{
	struct ui_layout *layout = &ctx->layout;

	gf_layout_pop(layout);
	// gf_decrease_z_idx(layout);
}

static inline void gf_window_header(struct ui_context *ctx, const char *title,
				    const char *subtitle)
{
	struct ui_vertex *ui = &ctx->ui;
	struct ui_layout *layout = &ctx->layout;
	struct ui_theme *theme = &ctx->theme;
	struct ui_atlas *atlas = &ctx->atlas;

	struct ui_layout_context *parent = _gf_parent_ctx(layout);
	uint32_t color = 0xFFFFFFFF;

	float title_w = gf_get_text_width(atlas, title);
	struct ui_size glyph1_size = ui_glyph_size(atlas, 10);
	ui_text(ctx, title, UI_FONT_STYLE_REGULAR, 0,
		parent->bounds_h * 0.5 - glyph1_size.h * 0.75, color);

	parent->cursor.y += glyph1_size.h + 4.0f;

	float stitle_w = gf_get_text_width(atlas, subtitle);
	struct ui_size glyph2_size = ui_glyph_size(atlas, 10);
	ui_text(ctx, subtitle, UI_FONT_STYLE_REGULAR, 0,
		parent->bounds_h * 0.5 - glyph2_size.h * 0.75, color);

	printf("Creating label in z: %d\n", layout->z_index);
}

#endif // __GF_IMGUI_UI_WINDOW_INCLUDED_H__
