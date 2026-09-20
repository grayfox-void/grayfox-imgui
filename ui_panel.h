#ifndef __GF_IMGUI_UI_PANEL_INCLUDED_H__
#define __GF_IMGUI_UI_PANEL_INCLUDED_H__

#include "ui_base.h"
#include "ui_common.h"
#include "ui_constants.h"
#include "ui_input.h"
#include "ui_layout.h"
#include "ui_rect.h"
#include "ui_types.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>

#if defined(__x86_64__) || defined(_M_X64) || defined(__aarch64__)
#define GF_ARCH_64 1
#define GF_RECT_RETURN struct ui_rect
#define GF_RECT_OUT_PARAM
#else
#define GF_ARCH_64 0
#define GF_RECT_RETURN void
#define GF_RECT_OUT_PARAM , struct ui_rect *out
#endif

const float EV_WIDTH = 5.0f;
const float THICKNESS = UI_BORDER_THICKNESS;

static inline GF_RECT_RETURN _gf_panel(struct ui_context *ctx, float width,
				       float height GF_RECT_OUT_PARAM)
{
	struct ui_vertex *ui = &ctx->ui;
	struct ui_layout *layout = &ctx->layout;
	struct ui_theme *theme = &ctx->theme;
	struct ui_input *input = &ctx->input;

	struct ui_layout_context *parent = _gf_parent_ctx(layout);
	uint32_t color = gf_rect_color(theme, UI_RECT_MENUBAR);

	float w = 0;
	float h = 0;
	float layout_w = 0;
	float layout_h = 0;
	ui_direction direction;

	if (width) {
		w = width;
		h = parent->bounds_h;
		direction = UI_DIRECTION_HORIZONTAL;
		layout_w += w - EV_WIDTH;
		layout_h = h;
	} else {
		w = parent->bounds_w;
		h = height;
		direction = UI_DIRECTION_VERTICAL;
		layout_w = w;
		layout_h += h - EV_WIDTH;
	}

	struct ui_rect rect = {
		.x = parent->cursor.x, .y = parent->cursor.y, .w = w, .h = h
	};

	gf_push_rect_geometry_xy(ui, layout, rect, color);

	gf_layout_move_cursor(layout, rect.w, rect.h);

	gf_layout_push(layout,
		       (struct ui_rect){ rect.x + 0.3f, rect.y + 0.3f,
					 layout_w - 1.0f, layout_h - 1.0f },
		       direction);

#if GF_ARCH_64
	return rect;
#else
	*out = rect;
#endif
}

static inline void _gf_border_resize_h(struct ui_context *ctx,
				       struct ui_rect rect)
{
	struct ui_vertex *ui = &ctx->ui;
	struct ui_layout *layout = &ctx->layout;
	struct ui_theme *theme = &ctx->theme;
	struct ui_input *input = &ctx->input;

	uint32_t color = gf_rect_color(theme, UI_RECT_BORDER);

	gf_push_rect_geometry_xy(ui, layout,
				 (struct ui_rect){ rect.x + rect.w - EV_WIDTH,
						   rect.y, THICKNESS, rect.h },
				 color);

	int hover = gf_mouse_event((struct ui_rect){ rect.x + rect.w - EV_WIDTH,
						     rect.y, EV_WIDTH, rect.h },
				   input);

	gf_push_rect_geometry_xy(ui, layout,
				 (struct ui_rect){ rect.x + rect.w - THICKNESS,
						   rect.y, THICKNESS, rect.h },
				 color);
}

static inline void _gf_border_resize_v(struct ui_context *ctx,
				       struct ui_rect rect)
{
	struct ui_vertex *ui = &ctx->ui;
	struct ui_layout *layout = &ctx->layout;
	struct ui_theme *theme = &ctx->theme;
	struct ui_input *input = &ctx->input;

	uint32_t color = gf_rect_color(theme, UI_RECT_BORDER);

	gf_push_rect_geometry_xy(ui, layout,
				 (struct ui_rect){ rect.x,
						   rect.y + rect.h - EV_WIDTH,
						   rect.w, THICKNESS },
				 color);

	int hover = gf_mouse_event((struct ui_rect){ rect.x,
						     rect.y + rect.h - EV_WIDTH,
						     rect.w, EV_WIDTH },
				   input);

	gf_push_rect_geometry_xy(ui, layout,
				 (struct ui_rect){ rect.x,
						   rect.y + rect.h - THICKNESS,
						   rect.w, THICKNESS },
				 color);
}

static inline __attribute__((always_inline)) void
gf_panel_v_begin(struct ui_context *ctx, float width)
{
	struct ui_rect rect = { 0 };

	GF_ASSIGN(rect, _gf_panel, ctx, width, 0);
	_gf_border_resize_h(ctx, rect);
}

static inline void gf_panel_v_end(struct ui_context *ctx)
{
	struct ui_vertex *ui = &ctx->ui;
	struct ui_layout *layout = &ctx->layout;
	struct ui_input *input = &ctx->input;

	_gf_scroll(ui, layout, input);
	gf_layout_pop(layout);
}

static inline __attribute__((always_inline)) void
gf_panel_h_begin(struct ui_context *ctx, float height)
{
	struct ui_rect rect = { 0 };

	GF_ASSIGN(rect, _gf_panel, ctx, 0, height);
	_gf_border_resize_v(ctx, rect);
}

static inline void gf_panel_h_end(struct ui_context *ctx)
{
	struct ui_vertex *ui = &ctx->ui;
	struct ui_layout *layout = &ctx->layout;
	struct ui_input *input = &ctx->input;

	_gf_scroll(ui, layout, input);
	gf_layout_pop(layout);
}

#endif // __GF_IMGUI_UI_PANEL_INCLUDED_H__
