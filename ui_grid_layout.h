#ifndef __GF_IMGUI_UI_GRID_LAYOUT_INCLUDED_H__
#define __GF_IMGUI_UI_GRID_LAYOUT_INCLUDED_H__

#include "ui_layout.h"
#include "ui_rect.h"
#include "ui_types.h"

static inline void gf_row_height(struct ui_context *ctx, float height)
{
}

static inline void gf_columns_set(struct ui_context *ctx, int cols)
{
	struct ui_layout *layout = &ctx->layout;
	struct ui_layout_context *parent = _gf_parent_ctx(layout);
}

static inline void gf_row_begin(struct ui_context *ctx, float height)
{
	struct ui_layout *layout = &ctx->layout;
	struct ui_layout_context *parent = _gf_parent_ctx(layout);

	struct ui_rect rect = { parent->cursor.x, parent->cursor.y,
				parent->bounds_w, height };

	gf_layout_push(layout, rect, UI_DIRECTION_HORIZONTAL);
}

static inline void gf_row_end(struct ui_context *ctx)
{
	struct ui_layout *layout = &ctx->layout;
	struct ui_layout_context self = { 0 };

	gf_layout_pop_ctx(layout, &self);

	struct ui_layout_context *parent = _gf_parent_ctx(layout);

	parent->cursor.y += self.bounds_h;
}

static inline void gf_col_begin(struct ui_context *ctx, float width)
{
	struct ui_layout *layout = &ctx->layout;
	struct ui_layout_context *parent = _gf_parent_ctx(layout);

	struct ui_pos start = gf_rect_cursor(layout);
	struct ui_rect rect = { parent->cursor.x, parent->cursor.y, width,
				parent->bounds_h };

	gf_layout_push(layout, rect, UI_DIRECTION_HORIZONTAL);
}

static inline void gf_col_end(struct ui_context *ctx)
{
	struct ui_layout *layout = &ctx->layout;
	struct ui_layout_context self = { 0 };

	gf_layout_pop_ctx(layout, &self);

	struct ui_layout_context *parent = _gf_parent_ctx(layout);

	parent->cursor.x += self.bounds_w;
}

#endif // __GF_IMGUI_UI_GRID_LAYOUT_INCLUDED_H__
