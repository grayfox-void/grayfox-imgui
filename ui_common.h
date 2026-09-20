#ifndef __GF_IMGUI_UI_COMMON_INCLUDED_H__
#define __GF_IMGUI_UI_COMMON_INCLUDED_H__

#include "ui_base.h"
#include "ui_constants.h"
#include "ui_input.h"
#include "ui_font.h"
#include "ui_rect.h"
#include "ui_types.h"

#include <assert.h>

#define GF_ID(str) (uint32_t)(uintptr_t)str

// ============================================================================
// [1] STRUCTS, ENUMS, CONSTANTS
// ============================================================================

// ============================================================================
// [2] FUNCTION PROTOTYPES
// ============================================================================

// ============================================================================
// [3] INLINE FUNCTION PROTOTYPES
// ============================================================================

#if defined(__x86_64__) || defined(_M_X64) || defined(__aarch64__)

static inline __attribute__((always_inline))
__attribute__((nonnull(1, 2, 3))) struct ui_rect
_gf_draw_rect_w(struct ui_vertex *ui, struct ui_layout *restrict layout,
		const struct ui_theme *restrict theme, int ui_rect_id,
		float width);

static inline __attribute__((always_inline))
__attribute__((nonnull(1, 2, 3))) struct ui_rect
_gf_draw_rect_h(struct ui_vertex *ui, struct ui_layout *restrict layout,
		const struct ui_theme *restrict theme, int ui_rect_id,
		float height);

#else

static inline __attribute__((always_inline))
__attribute__((nonnull(1, 2, 3, 6))) void
_gf_draw_rect_w(struct ui_vertex *ui, struct ui_layout *restrict layout,
		const struct ui_theme *restrict theme, int ui_rect_id,
		float width, struct ui_rect *out);

static inline __attribute__((always_inline))
__attribute__((nonnull(1, 2, 3, 6))) void
_gf_draw_rect_h(struct ui_vertex *ui, struct ui_layout *restrict layout,
		const struct ui_theme *restrict theme, int ui_rect_id,
		float height, struct ui_rect *out);
#endif

// ============================================================================
// [4] INLINE FUNCTIONS - COMMON
// ============================================================================

static inline __attribute__((always_inline)) void
gf_increase_z_idx(struct ui_layout *layout)
{
	layout->z_index++;
}

static inline __attribute__((always_inline)) void
gf_decrease_z_idx(struct ui_layout *layout)
{
	layout->z_index--;
}

static inline void gf_draw_borders(struct ui_vertex *ui,
				   struct ui_layout *layout,
				   const struct ui_rect rect)
{
	float thickness = UI_BORDER_THICKNESS;

	// upper border
	gf_push_rect_geometry_xy(ui, layout,
				 (struct ui_rect){ rect.x, rect.y, rect.w,
						   thickness },
				 COLOR_BORDER);
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

static inline void gf_draw_borders_color(struct ui_vertex *ui,
					 struct ui_layout *layout,
					 const struct ui_rect rect,
					 uint32_t color)
{
	float thickness = UI_BORDER_THICKNESS;

	// upper border
	gf_push_rect_geometry_xy(
		ui, layout,
		(struct ui_rect){ rect.x, rect.y, rect.w, thickness }, color);
	// right border
	gf_push_rect_geometry_xy(ui, layout,
				 (struct ui_rect){ rect.x + rect.w, rect.y,
						   thickness, rect.h },
				 color);

	// bottom border
	gf_push_rect_geometry_xy(ui, layout,
				 (struct ui_rect){ rect.x, rect.y + rect.h,
						   rect.w, thickness },
				 color);

	// left border
	gf_push_rect_geometry_xy(
		ui, layout,
		(struct ui_rect){ rect.x, rect.y, thickness, rect.h }, color);
}

static inline void _gf_scroll(struct ui_vertex *ui, struct ui_layout *layout,
			      struct ui_input *input)
{
	struct ui_layout_context *parent = _gf_parent_ctx(layout);

	struct ui_rect container = { parent->bounds_x, parent->bounds_y,
				     parent->bounds_w, parent->bounds_h };

	if (parent->over_x) {
		struct ui_rect scroll_x = { parent->bounds_x,
					    parent->bounds_y +
						    parent->bounds_h - 10.0f,
					    parent->bounds_w, 10.0f };

		struct ui_rect scroll_x_bar = { parent->bounds_x,
						parent->bounds_y +
							parent->bounds_h - 8.0f,
						parent->scroll_x_size, 6.0f };

		uint32_t m_ev_in = gf_mouse_event(scroll_x_bar, input);
		uint32_t m_ev_clk = gf_mouse_event(scroll_x, input);

		uint8_t hover = GF_MOUSE_EV_HOVER(m_ev_in);
		uint8_t press = GF_MOUSE_EV_PRESS(m_ev_clk);

		scroll_x_bar.x += ((input->mpos.x - parent->bounds_x) * press);

		if (scroll_x_bar.x < parent->bounds_x)
			scroll_x_bar.x = parent->bounds_x;

		if (scroll_x_bar.w >= parent->bounds_w)
			scroll_x_bar.x = parent->bounds_w - scroll_x_bar.w;

		gf_push_rect_geometry_xy(ui, layout, scroll_x, 0x2F2F2FFF);
		gf_push_rect_geometry_xy(ui, layout, scroll_x_bar, 0x383838FF);
	}

	if (parent->over_y) {
		struct ui_rect scroll_y = { parent->bounds_x +
						    parent->bounds_w - 10.0f,
					    parent->bounds_y, 10.0f,
					    parent->bounds_h };

		struct ui_rect scroll_y_bar = { parent->bounds_x +
							parent->bounds_w - 8.0f,
						parent->bounds_y, 6.0f,
						parent->scroll_y_size };

		uint32_t m_ev_in = gf_mouse_event(scroll_y_bar, input);
		uint32_t m_ev_clk = gf_mouse_event(scroll_y, input);

		uint8_t hover = GF_MOUSE_EV_HOVER(m_ev_in);
		uint8_t press = GF_MOUSE_EV_PRESS(m_ev_clk);

		scroll_y_bar.y += ((input->mpos.y - parent->bounds_y) * press);

		if (scroll_y_bar.y < parent->bounds_y)
			scroll_y_bar.y = parent->bounds_y;

		if (scroll_y_bar.h >= parent->bounds_h)
			scroll_y_bar.y = parent->bounds_h - scroll_y_bar.h;

		gf_push_rect_geometry_xy(ui, layout, scroll_y, 0x2F2F2FFF);
		gf_push_rect_geometry_xy(ui, layout, scroll_y_bar, 0x383838FF);
	}
}

// ============================================================================
// [5] INLINE FUNCTIOS - ARCH DEPENDENT (64bits - 32bits)
// ============================================================================

#if defined(__x86_64__) || defined(_M_X64) || defined(__aarch64__)

static inline __attribute__((always_inline))
__attribute__((nonnull(1))) struct ui_rect
_gf_rect_limits_w(const struct ui_layout *restrict layout, float width)
{
	assert(layout != NULL);

	struct ui_layout_context *parent = _gf_parent_ctx(layout);
	struct ui_pos start = gf_rect_cursor(layout);

	return (struct ui_rect){
		.x = start.x, .y = start.y, .w = width, .h = parent->bounds_h
	};
}

static inline __attribute__((always_inline))
__attribute__((nonnull(1))) struct ui_rect
_gf_rect_limits_h(const struct ui_layout *restrict layout, float height)
{
	assert(layout != NULL);

	struct ui_layout_context *parent = _gf_parent_ctx(layout);
	struct ui_pos start = gf_rect_cursor(layout);

	return (struct ui_rect){
		.x = start.x, .y = start.y, .w = parent->bounds_w, .h = height
	};
}

static inline __attribute__((always_inline)) struct ui_rect
_gf_rect_limits_text(struct ui_layout *restrict layout,
		     struct ui_atlas *restrict atlas,
		     const char *restrict label)
{
	assert(layout != NULL);

	float text_w = gf_get_text_width(atlas, label);
	float item_w = text_w + PAD_MENU_H * 2.0f;

	return _gf_rect_limits_w(layout, item_w);
}

static inline __attribute__((always_inline)) struct ui_rect
_gf_rect_limits_icon_text(struct ui_layout *restrict layout,
			  struct ui_atlas *restrict atlas, int icon,
			  const char *restrict label)
{
	assert(layout != NULL);

	float text_w = gf_get_text_width(atlas, label);
	struct ui_size glyph_size = ui_glyph_size(atlas, icon);

	float item_w = glyph_size.w + text_w + PAD_MENU_H * 2.0f;

	return _gf_rect_limits_w(layout, item_w);
}

static inline __attribute__((always_inline))
__attribute__((nonnull(1, 2, 3))) struct ui_rect
_gf_draw_rect_w(struct ui_vertex *restrict ui,
		struct ui_layout *restrict layout,
		const struct ui_theme *restrict theme, int ui_rect_id,
		float width)
{
	assert(ui != NULL);
	assert(layout != NULL);
	assert(theme != NULL);

	struct ui_rect rect = _gf_rect_limits_w(layout, width);
	uint32_t color = gf_rect_color(theme, ui_rect_id);

	gf_push_rect_geometry_xy(ui, layout, rect, color);

	// parent->cursor.x += rect.w;
	// parent->cursor.y += parent->bounds_h - rect.h;

	return rect;
}

static inline __attribute__((always_inline))
__attribute__((nonnull(1, 2, 3))) struct ui_rect
_gf_draw_rect_h(struct ui_vertex *restrict ui,
		struct ui_layout *restrict layout,
		const struct ui_theme *restrict theme, int ui_rect_id,
		float height)
{
	assert(ui != NULL);
	assert(layout != NULL);
	assert(theme != NULL);

	struct ui_rect rect = _gf_rect_limits_h(layout, height);
	uint32_t color = gf_rect_color(theme, ui_rect_id);

	gf_push_rect_geometry_xy(ui, layout, rect, color);

	// parent->cursor.x += parent->bounds_w - rect.w;
	// parent->cursor.y += rect.h;

	return rect;
}

#else

static inline __attribute__((always_inline)) __attribute__((nonnull(1))) void
_gf_rect_limits_w(const struct ui_layout *restrict layout, float width,
		  struct ui_rect *out)
{
	assert(layout != NULL);

	struct ui_layout_context *parent = _gf_parent_ctx(layout);
	struct ui_pos start = gf_rect_cursor(layout);

	out->x = start.x;
	out->y = start.y;
	out->w = width;
	out->h = parent->bounds_h;
}

static inline __attribute__((always_inline))
__attribute__((nonnull(1))) struct ui_rect
_gf_rect_limits_h(const struct ui_layout *restrict layout, float height,
		  struct ui_rect *out)
{
	assert(layout != NULL);

	struct ui_layout_context *parent = _gf_parent_ctx(layout);
	struct ui_pos start = gf_rect_cursor(layout);

	out->x = start.x;
	out->y = start.y;
	out->w = parent->bounds_w;
	out->h = height;
}

static inline __attribute__((always_inline))
__attribute__((nonnull(1, 2, 3, 6))) void
_gf_draw_rect_w(struct ui_vertex *ui, struct ui_layout *restrict layout,
		const struct ui_theme *restrict theme, int ui_rect_id,
		float width, struct ui_rect *out)
{
	assert(ui != NULL);
	assert(layout != NULL);
	assert(theme != NULL);

	struct ui_rect rect = { 0 };

	_gf_rect_limits_w(layout, &rect);
	uint32_t color = gf_rect_color(theme, ui_rect_id);

	gf_push_rect_geometry_xy(ui, layout, rect, color);

	// parent->cursor.x += rect.w;
	// parent->cursor.y += parent->bounds_h - rect.h;

	out->x = rect.x;
	out->y = rect.y;
	out->w = rect.w;
	out->h = rect.h;
}

static inline __attribute__((always_inline))
__attribute__((nonnull(1, 2, 3, 6))) void
_gf_draw_rect_h(struct ui_vertex *ui, struct ui_layout *restrict layout,
		const struct ui_theme *restrict theme, int ui_rect_id,
		float height, struct ui_rect *out)
{
	assert(ui != NULL);
	assert(layout != NULL);
	assert(theme != NULL);

	struct ui_rect rect = { 0 };

	_gf_rect_limits_h(layout, &rect);
	uint32_t color = gf_rect_color(theme, ui_rect_id);

	gf_push_rect_geometry_xy(ui, layout, rect, color);

	// parent->cursor.x += parent->bounds_w - rect.w;
	// parent->cursor.y += rect.h;

	out->x = rect.x;
	out->y = rect.y;
	out->w = rect.w;
	out->h = rect.h;
}

#endif

#endif // __GF_IMGUI_UI_COMMON_INCLUDED_H__
