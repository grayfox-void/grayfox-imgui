#ifndef __GF_IMGUI_UI_MENUBAR_INCLUDED_H__
#define __GF_IMGUI_UI_MENUBAR_INCLUDED_H__

#include "ui_base.h"
#include "ui_common.h"
#include "ui_constants.h"
#include "ui_layout.h"
#include "ui_types.h"
#include <stdint.h>

static inline void _gf_menubar_begin(struct ui_context *ctx, float height,
				     uint32_t id)
{
	struct ui_vertex *ui = &ctx->ui;
	struct ui_layout *layout = &ctx->layout;
	struct ui_theme *theme = &ctx->theme;

	struct ui_rect rect1 = { 0 };
	struct ui_rect rect2 = { 0 };

	GF_ASSIGN(rect1, _gf_draw_rect_h, ui, layout, theme, UI_RECT_MENUBAR,
		  height);

	gf_layout_move_cursor(layout, rect1.w, rect1.h);

	GF_ASSIGN(rect2, _gf_draw_rect_h, ui, layout, theme, UI_RECT_BORDER,
		  UI_BORDER_THICKNESS);

	gf_layout_move_cursor(layout, rect2.w, rect2.h);

	gf_layout_push(layout, rect1, UI_DIRECTION_HORIZONTAL);
}

static inline __attribute__((always_inline)) void
gf_menubar_begin(struct ui_context *ctx, float height)
{
	_gf_menubar_begin(ctx, height, 0);
}

static inline __attribute__((always_inline)) void
gf_menubar_id_begin(struct ui_context *ctx, float height, uint32_t id)
{
	_gf_menubar_begin(ctx, height, id);
}

static inline void gf_menubar_end(struct ui_context *ctx)
{
	struct ui_layout *layout = &ctx->layout;

	gf_layout_pop(layout);
}

#endif // __GF_IMGUI_UI_MENUBAR_INCLUDED_H__
