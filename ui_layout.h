#ifndef __GF_IMGUI_UI_LAYOUT_INCLUDED_H__
#define __GF_IMGUI_UI_LAYOUT_INCLUDED_H__

#include "ui_base.h"
#include "ui_types.h"

#include <assert.h>
#include <stdio.h>

static inline __attribute__((always_inline)) __attribute__((nonnull(1)))
__attribute__((const)) struct ui_layout_context *
_gf_parent_ctx(const struct ui_layout *layout)
{
	return (struct ui_layout_context *)&layout->ctx[layout->lay_ctx_ptr];
}

static inline void gf_layout_push(struct ui_layout *restrict layout,
				  const struct ui_rect rect, uint8_t direction)
{
	assert(layout->lay_ctx_ptr < MAX_LAYOUT_CTX);

	int sp = ++layout->lay_ctx_ptr;

	layout->ctx[sp].cursor.x = rect.x;
	layout->ctx[sp].cursor.y = rect.y;
	layout->ctx[sp].max_x = rect.x;
	layout->ctx[sp].max_y = rect.y;

	layout->ctx[sp].bounds_x = rect.x;
	layout->ctx[sp].bounds_y = rect.y;
	layout->ctx[sp].bounds_w = rect.w;
	layout->ctx[sp].bounds_h = rect.h;

	layout->ctx[sp].direction = direction;
}

static inline void gf_layout_pop(struct ui_layout *layout)
{
	assert(layout->lay_ctx_ptr > UI_LAYOUT_SP_START);
	layout->lay_ctx_ptr--;
}

static inline void gf_layout_pop_ctx(struct ui_layout *layout,
				     struct ui_layout_context *out)
{
	assert(layout->lay_ctx_ptr > UI_LAYOUT_SP_START);

	int sp = layout->lay_ctx_ptr--;

	out->cursor.x = layout->ctx[sp].cursor.x;
	out->cursor.y = layout->ctx[sp].cursor.y;
	out->max_x = layout->ctx[sp].max_x;
	out->max_y = layout->ctx[sp].max_y;

	out->bounds_x = layout->ctx[sp].bounds_x;
	out->bounds_y = layout->ctx[sp].bounds_y;
	out->bounds_w = layout->ctx[sp].bounds_w;
	out->bounds_h = layout->ctx[sp].bounds_h;
	out->over_x = layout->ctx[sp].over_x;
	out->over_y = layout->ctx[sp].over_y;
	out->scroll_x = layout->ctx[sp].scroll_x;
	out->scroll_y = layout->ctx[sp].scroll_y;
	out->scroll_x_size = layout->ctx[sp].scroll_x_size;
	out->scroll_y_size = layout->ctx[sp].scroll_y_size;

	out->direction = layout->ctx[sp].direction;
}

static inline void gf_layout_reset(struct ui_layout *restrict layout,
				   float screen_w, float screen_h)
{
	layout->lay_ctx_ptr = UI_LAYOUT_SP_START;

	struct ui_layout_context *screen = &layout->ctx[0];
	screen->cursor.x = 0.0f;
	screen->cursor.y = 0.0f;
	screen->max_x = 0.0f;
	screen->max_y = 0.0f;
	screen->bounds_x = 0.0f;
	screen->bounds_y = 0.0f;
	screen->bounds_w = screen_w;
	screen->bounds_h = screen_h;
	screen->over_x = 0.0f;
	screen->over_y = 0.0f;
	screen->scroll_x = 0.0f;
	screen->scroll_y = 0.0f;
	screen->scroll_x_size = 0.0f;
	screen->scroll_y_size = 0.0f;
	screen->direction = UI_DIRECTION_HORIZONTAL;
}

static inline __attribute__((nonnull(1))) void
gf_layout_move_cursor_(struct ui_layout *restrict layout, float width,
		       float height)
{
	struct ui_layout_context *parent = _gf_parent_ctx(layout);

	int ui_h = parent->direction == UI_DIRECTION_HORIZONTAL;
	int ui_v = parent->direction == UI_DIRECTION_VERTICAL;

	parent->cursor.x += width * (ui_h);
	parent->cursor.y += height * (ui_v);
}

static inline __attribute__((nonnull(1))) void
gf_layout_move_cursor(struct ui_layout *restrict layout, float width,
		      float height)
{
	assert(layout != NULL);

	struct ui_layout_context *parent = _gf_parent_ctx(layout);

	if (parent->direction == UI_DIRECTION_HORIZONTAL) {
		parent->cursor.x += width;

		if (/*!ctx->disable_wrap && */ parent->cursor.x >=
		    (parent->bounds_x + parent->bounds_w)) {
			parent->cursor.x = parent->bounds_x;
			parent->cursor.y += height;
		}
	} else if (parent->direction == UI_DIRECTION_VERTICAL) {
		parent->cursor.y += height;

		if (/*!ctx->disable_wrap && */ parent->cursor.y >=
		    (parent->bounds_y + parent->bounds_h)) {
			parent->cursor.y = parent->bounds_y;
			parent->cursor.x += width;
		}
	}
}

static inline __attribute__((nonnull(1))) void
gf_layout_move_cursor_global(struct ui_layout *restrict layout, float width,
			     float height)
{
	struct ui_layout_context *parent = _gf_parent_ctx(layout);

	int ui_h = parent->direction == UI_DIRECTION_HORIZONTAL;
	int ui_v = parent->direction == UI_DIRECTION_VERTICAL;

	parent->cursor.x += width;
	parent->cursor.y += parent->bounds_h - height;
}

// TODO replace those functions. Add the logic to gf_layout_move_cursor
static inline __attribute__((nonnull(1))) void
gf_layout_move_cursor_global_w(struct ui_layout *restrict layout, float width,
			       float height)
{
	struct ui_layout_context *parent = _gf_parent_ctx(layout);

	parent->cursor.x += width;
	parent->cursor.y += parent->bounds_h - height;
}

static inline __attribute__((nonnull(1))) void
gf_layout_move_cursor_global_h(struct ui_layout *restrict layout, float width,
			       float height)
{
	struct ui_layout_context *parent = _gf_parent_ctx(layout);

	parent->cursor.x += parent->bounds_w - width;
	parent->cursor.y += height;
}

#endif // __GF_IMGUI_UI_LAYOUT_INCLUDED_H__
