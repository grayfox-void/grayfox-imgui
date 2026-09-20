#ifndef __GF_IMGUI_UI_RECT_INCLUDED_H__
#define __GF_IMGUI_UI_RECT_INCLUDED_H__

#include "ui_base.h"
#include "ui_layout.h"
#include "ui_types.h"
#include <assert.h>
#include <stdint.h>

#if defined(__x86_64__) || defined(_M_X64) || defined(__aarch64__)

#define GF_RECT_ASSIGN GF_ASSIGN
#define GF_RECT_ASSIGN_PTR GF_ASSIGN_PTR

#else

// 32 bits (x86, ARM32) or old hardware
#define GF_RECT_ASSIGN(out, func, ...) func(__VA_ARGS__, &out)
#define GF_RECT_ASSIGN_PTR(out_ptr, func, ...) func(__VA_ARGS__, out_ptr)

#endif

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

static inline __attribute__((always_inline)) struct ui_rect
gf_rect_limits(const struct ui_theme *restrict theme,
	       const struct ui_layout *restrict layout, int ui_rect_id);

static inline __attribute__((always_inline)) struct ui_rect
_gf_rect_limits(const struct ui_layout *restrict layout);

#else

static inline __attribute__((always_inline)) void
gf_rect_limits(struct ui_theme *theme, struct ui_layout *layout, int ui_rect_id,
	       struct ui_rect *out);

static inline __attribute__((always_inline)) void
_gf_rect_limits(struct ui_layout *layout, struct ui_rect *out);

#endif

// ============================================================================
// [4] INLINE FUNCTIONS - COMMON
// ============================================================================

static inline __attribute__((always_inline)) __attribute__((nonnull(1, 2))) void
gf_push_rect_geometry_xy(struct ui_vertex *restrict ui,
			 struct ui_layout *restrict layout,
			 const struct ui_rect rect, uint32_t color)
{
	float x = rect.x;
	float y = rect.y;
	float x2 = x + rect.w;
	float y2 = y + rect.h;

	int z_index = layout->z_index;

	struct rect_vertex *vertices =
		&ui->vertices[UI_CHANNEL_OFFSET * z_index];

	// printf("Vertex count before: %d\n", ui->vertex_count[z_index]);

	vertices[ui->vertex_count[z_index]++] = (struct rect_vertex){
		.x = x, .y = y, .color = color, .u = 0, .v = 0
	};

	vertices[ui->vertex_count[z_index]++] = (struct rect_vertex){
		.x = x2, .y = y, .color = color, .u = 0, .v = 0
	};

	vertices[ui->vertex_count[z_index]++] = (struct rect_vertex){
		.x = x2, .y = y2, .color = color, .u = 0, .v = 0
	};

	vertices[ui->vertex_count[z_index]++] = (struct rect_vertex){
		.x = x, .y = y2, .color = color, .u = 0, .v = 0
	};

	// printf("Vertex count after: %d\n", ui->vertex_count[z_index]);
	// printf("Z INDEX: %d\n", z_index);
}

static inline __attribute__((always_inline)) __attribute__((nonnull(1, 2))) void
gf_push_rect_geometry_xyuv(struct ui_vertex *restrict ui,
			   struct ui_layout *restrict layout,
			   struct ui_rect *restrict xy_rect,
			   struct ui_rect *restrict uv_rect, uint32_t color)
{
	float x = xy_rect->x;
	float y = xy_rect->y;
	float x2 = x + xy_rect->w;
	float y2 = y + xy_rect->h;

	float u = uv_rect->x;
	float v = uv_rect->y;
	float u2 = u + uv_rect->w;
	float v2 = v + uv_rect->h;

	int z_index = layout->z_index;

	struct rect_vertex *vertices =
		&ui->vertices[UI_CHANNEL_OFFSET * z_index];

	// printf("Vertex count before: %d\n", ui->vertex_count[z_index]);

	vertices[ui->vertex_count[z_index]++] = (struct rect_vertex){
		.x = x, .y = y, .color = color, .u = u, .v = v
	};

	vertices[ui->vertex_count[z_index]++] = (struct rect_vertex){
		.x = x2, .y = y, .color = color, .u = u2, .v = v
	};

	vertices[ui->vertex_count[z_index]++] = (struct rect_vertex){
		.x = x2, .y = y2, .color = color, .u = u2, .v = v2
	};

	vertices[ui->vertex_count[z_index]++] = (struct rect_vertex){
		.x = x, .y = y2, .color = color, .u = u, .v = v2
	};

	// printf("Vertex count after: %d\n", ui->vertex_count[z_index]);
}

static inline __attribute__((always_inline)) __attribute__((nonnull(1)))
__attribute__((const)) struct ui_pos
gf_rect_cursor(const struct ui_layout *layout)
{
	const struct ui_layout_context *parent = _gf_parent_ctx(layout);
	return (struct ui_pos){ .x = parent->cursor.x, .y = parent->cursor.y };
}

static inline __attribute__((always_inline)) __attribute__((nonnull(1)))
__attribute__((const)) struct ui_pos
gf_rect_origin(const struct ui_layout *layout)
{
	const struct ui_layout_context *parent = _gf_parent_ctx(layout);
	return (struct ui_pos){ .x = parent->bounds_x, .y = parent->bounds_y };
}

static inline __attribute__((always_inline)) __attribute__((nonnull(1)))
__attribute__((const)) uint32_t
gf_rect_color(const struct ui_theme *restrict theme, int ui_rect_id)
{
	assert(theme != NULL);

	return theme->style[ui_rect_id].bg_color;
}

static inline __attribute__((always_inline)) __attribute__((nonnull(1)))
__attribute__((const)) struct ui_padding
gf_rect_padding(const struct ui_theme *restrict theme, int ui_rect_id)
{
	assert(theme != NULL);

	return (struct ui_padding){ .x = theme->style[ui_rect_id].padding.x,
				    .y = theme->style[ui_rect_id].padding.y };
}

// TODO get style from style context.
static inline __attribute__((always_inline)) __attribute__((nonnull(1, 3))) void
gf_rect_style(const struct ui_theme *restrict theme, int ui_rect_id,
	      struct ui_style *restrict out)
{
	assert(theme != NULL);
	assert(out != NULL);

	out->bg_color = theme->style[ui_rect_id].bg_color;
	out->text_color = 0;
	out->padding.x = theme->style[ui_rect_id].padding.x;
	out->padding.y = theme->style[ui_rect_id].padding.y;
}

// ============================================================================
// [5] INLINE FUNCTIOS - ARCH DEPENDENT (64bits - 32bits)
// ============================================================================

#if defined(__x86_64__) || defined(_M_X64) || defined(__aarch64__)

/*
 * get the rectangle limits (x, y, w, h).
 * NOTE: if you need a portable function
 * use it with GF_RECT_ASSIGN macro
 */
static inline __attribute__((always_inline))
__attribute__((nonnull(1, 2))) struct ui_rect
gf_rect_limits(const struct ui_theme *restrict theme,
	       const struct ui_layout *restrict layout, int ui_rect_id)
{
	const struct ui_layout_context *parent = _gf_parent_ctx(layout);

	return (struct ui_rect){ .x = parent->cursor.x,
				 .y = parent->cursor.y,
				 .w = theme->style[ui_rect_id].w_rect,
				 .h = theme->style[ui_rect_id].h_rect };
}

/*
 * get the rectangle limits (x, y, w, h).
 * NOTE: if you need a portable function
 * use it with GF_RECT_ASSIGN macro
 */
static inline __attribute__((always_inline))
__attribute__((nonnull(1))) struct ui_rect
_gf_rect_limits(const struct ui_layout *restrict layout)
{
	const struct ui_layout_context *parent = _gf_parent_ctx(layout);

	return (struct ui_rect){ .x = parent->cursor.x,
				 .y = parent->cursor.y,
				 .w = parent->bounds_w - parent->cursor.x,
				 .h = parent->bounds_h - parent->cursor.y };
}

#else

/*
 * get the rectangle limits (x, y, w, h).
 * NOTE: if you need a portable function
 * use it with GF_RECT_ASSIGN macro
 */
static inline __attribute__((always_inline))
__attribute__((nonnull(1, 2, 4))) void
gf_rect_limits(struct ui_theme *theme, struct ui_layout *layout, int ui_rect_id,
	       struct ui_rect *out)
{
	struct ui_layout_context *parent = _gf_parent_ctx(layout);

	out->x = parent->cursor.x;
	out->y = parent->cursor.y;
	out->w = theme->style[ui_rect_id].w_rect;
	out->h = theme->style[ui_rect_id].h_rect;
}

/*
 * get the rectangle limits (x, y, w, h).
 * NOTE: if you need a portable function
 * use it with GF_RECT_ASSIGN macro
 */
static inline __attribute__((always_inline)) __attribute__((nonnull(1, 2))) void
_gf_rect_limits(struct ui_layout *layout, struct ui_rect *out)
{
	struct ui_layout_context *parent = _gf_parent_ctx(layout);

	out->x = parent->cursor.x;
	out->y = parent->cursor.y;
	out->w = parent->bounds_w - parent->cursor.x;
	out->h = parent->bounds_h - parent->cursor.y;
}

#endif

#endif // __GF_IMGUI_UI_RECT_INCLUDED_H__
