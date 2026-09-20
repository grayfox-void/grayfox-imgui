#ifndef __GF_IMGUI_UI_GRID_INCLUDED_H__
#define __GF_IMGUI_UI_GRID_INCLUDED_H__

#include "ui_base.h"
#include "ui_common.h"
#include "ui_constants.h"
#include "ui_input.h"
#include "ui_layout.h"
#include "ui_rect.h"
#include "ui_types.h"
#include <assert.h>
#include <stdint.h>

struct grid_size {
	float width;
	float height;
};

static inline struct grid_size _gf_grid(struct ui_context *ctx, int rows,
					int cols, float size)
{
	assert(ctx != NULL);

	struct ui_vertex *ui = &ctx->ui;
	struct ui_layout *layout = &ctx->layout;
	struct ui_theme *theme = &ctx->theme;
	struct ui_input *input = &ctx->input;

	struct ui_layout_context *parent = _gf_parent_ctx(layout);
	// struct ui_pos start = gf_rect_origin(layout);

	float w = (float)cols * size;
	float h = (float)rows * size;
	int w_mask = w <= parent->bounds_w;
	int h_mask = h <= parent->bounds_h;
	float width = w * w_mask + parent->bounds_w * !w_mask;
	float height = h * h_mask + parent->bounds_h * !h_mask;

	// TODO replace division
	int c = cols * w_mask + parent->bounds_w / size * !w_mask;
	int r = rows * h_mask + parent->bounds_h / size * !h_mask;

	int scroll_x_size =
		(parent->bounds_w / (w / parent->bounds_w)) * !w_mask;
	int scroll_y_size =
		(parent->bounds_h / (h / parent->bounds_h)) * !h_mask;

	struct ui_rect rect1 = { parent->bounds_x, parent->bounds_y, 1.0f,
				 height };

	for (int i = 0; i <= c; ++i, rect1.x += size)
		gf_push_rect_geometry_xy(ui, layout, rect1, 0x242424FF);

	struct ui_rect rect2 = { parent->bounds_x, parent->bounds_y, width,
				 1.0f };

	for (int i = 0; i <= r; ++i, rect2.y += size)
		gf_push_rect_geometry_xy(ui, layout, rect2, 0x242424FF);

	parent->over_x = !w_mask;
	parent->over_y = !h_mask;
	parent->scroll_x_size = scroll_x_size;
	parent->scroll_y_size = scroll_y_size;

	return (struct grid_size){ width, height };
}

static inline __attribute__((always_inline)) void
gf_grid(struct ui_context *ctx, int rows, int cols, float size)
{
	_gf_grid(ctx, rows, cols, size);
}

static inline struct ui_pos _gf_grid_hover(struct ui_pos mpos, uint32_t m_ev,
					   float x, float y, float size)
{
	float hover = GF_MOUSE_EV_HOVER(m_ev);

	float mask = size * hover;
	// TODO replace division for inverse
	float mx = x + (float)((int)((mpos.x - x) / size) * mask);
	float my = y + (float)((int)((mpos.y - y) / size) * mask);

	return (struct ui_pos){ mx, my };
}

static inline struct ui_pos _gf_grid_click(struct ui_pos mpos, uint32_t m_ev,
					   float x, float y, float size)
{
	float click = GF_MOUSE_EV_CLICK(m_ev);

	float mask = size * click;
	// TODO replace division for inverse
	float mx = x + (float)((int)((mpos.x - x) / size) * mask);
	float my = y + (float)((int)((mpos.y - y) / size) * mask);

	return (struct ui_pos){ mx, my };
}

static inline struct ui_pos _gf_grid_press(struct ui_pos mpos, uint32_t m_ev,
					   float x, float y, float size)
{
	float press = GF_MOUSE_EV_PRESS(m_ev);

	float mask = size * press;
	// TODO replace division for inverse
	float mx = x + (float)((int)((mpos.x - x) / size) * mask);
	float my = y + (float)((int)((mpos.y - y) / size) * mask);

	return (struct ui_pos){ mx, my };
}

static inline struct ui_pos gf_grid_interactive(struct ui_context *ctx,
						int rows, int cols, float size)
{
	struct ui_vertex *ui = &ctx->ui;
	struct ui_layout *layout = &ctx->layout;
	struct ui_input *input = &ctx->input;

	struct ui_layout_context *parent = _gf_parent_ctx(layout);
	struct ui_pos mpos = input->mpos;

	struct grid_size gs = _gf_grid(ctx, rows, cols, size);

	uint32_t m_ev = gf_mouse_event((struct ui_rect){ parent->bounds_x,
							 parent->bounds_y,
							 gs.width, gs.height },
				       input);

	struct ui_pos sel = _gf_grid_hover(mpos, m_ev, parent->bounds_x,
					   parent->bounds_y, size);

	struct ui_rect rect2 = { sel.x, sel.y, size, size };

	gf_draw_borders_color(ui, layout, rect2, 0x00CED1FF);

	return _gf_grid_click(mpos, m_ev, parent->bounds_x, parent->bounds_y,
			      size);
}

static inline void gf_grid_drag(struct ui_context *ctx, int rows, int cols,
				float size)
{
	gf_grid_interactive(ctx, rows, cols, size);
}

#endif // __GF_IMGUI_UI_GRID_INCLUDED_H__
