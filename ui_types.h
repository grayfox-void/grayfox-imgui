#ifndef __GF_IMGUI_UI_TYPES_INCLUDED_H__
#define __GF_IMGUI_UI_TYPES_INCLUDED_H__

#include "ui_base.h"
#include <stdint.h>

enum {
	UI_RECT_MENUBAR,
	UI_RECT_MENU,
	UI_RECT_MENUITEM,
	UI_RECT_TOOLBAR,
	UI_RECT_TOOLITEM,
	UI_RECT_DROPDOWNITEM,
	UI_RECT_BUTTON,
	UI_RECT_STATBAR,
	UI_RECT_SIDEBAR,
	UI_RECT_WINDOW,
	UI_RECT_BORDER,
	UI_RECT_SEPARATOR,
	UI_RECT_LABEL,
	UI_RECT_COUNT
};

typedef uint8_t widget_type;

enum {
	UI_FONT_STYLE_REGULAR = 0,
	UI_FONT_STYLE_BOLD,
	UI_FONT_STYLE_ITALIC,
	UI_FONT_STYLE_BOLD_ITALIC,
	UI_FONT_STYLE_COUNT
};

typedef uint8_t ui_font_style;

#define ATLAS_GLYPH_COUNT \
	((GLYPH_BANK_SIZE * UI_FONT_STYLE_COUNT) + MAX_UI_ICONS)

struct rect_vertex {
	float x, y;
	uint32_t color;
	uint16_t u, v;
};

struct ui_rect {
	float x, y;
	float w, h;
};

struct ui_vertex {
	uint32_t rect_count;
	uint32_t vertex_count[Z_MAX];

	struct rect_vertex vertices[MAX_VERTEX];
};

struct ui_padding {
	float x, y;
};

struct ui_style_context {
	uint32_t bg_color;
	uint32_t text_color;

	struct ui_padding padding;
};

struct ui_style {
	uint32_t bg_color;
	uint32_t text_color;

	float w_rect;
	float h_rect;
	float border_size;
	struct ui_padding padding;
};

struct ui_theme {
	struct ui_style style[UI_RECT_COUNT];
};

struct ui_size {
	float w, h;
};

struct __attribute__((aligned(32))) ui_glyph {
	float x_offset;
	float y_offset;
	float advance_x;
	float w;
	float h;
	float advance_y;
	float _unused;

	uint16_t u;
	uint16_t v;
};

struct ui_atlas {
	int width;
	int height;
	float inv_w;
	float inv_h;
	float ascent;
	float descent;
	float line_gap;
	float font_size;

	int next_icon_x;
	int next_icon_y;
	int current_row_max_h;
	uint32_t total_icons_loaded;

	struct ui_glyph glyphs[FONT_CHAR_COUNT];
};

struct ui_event {
	int type;
};

struct ui_input {
	struct ui_pos mpos;
	int mouse_click;
	int mouse_hover;
	int mouse_press;
	int mouse_drag;
	uint8_t mouse_ev;
};

struct ui_dropdown_context {
	float width;
	float height;

	float label_max;

	int items;

	uint8_t first_render; // flag

	uint8_t _unused[3];
};

struct ui_layout_context {
	float max_x;
	float max_y;

	float bounds_x;
	float bounds_y;

	float bounds_w;
	float bounds_h;

	float scroll_x;
	float scroll_y;

	float scroll_x_size;
	float scroll_y_size;

	struct ui_pos cursor;

	int over_x;
	int over_y;

	ui_direction direction;

	uint8_t _unused[7];
};

struct ui_layout {
	int max_width;
	int max_height;

	int z_index;

	int dd_ctx_ptr;
	int lay_ctx_ptr;
	int sty_ctx_ptr;

	struct ui_dropdown_context dropdown_ctx[MAX_LAYOUT_CTX];
	struct ui_layout_context ctx[MAX_LAYOUT_CTX];
	struct ui_style_context style_ctx[MAX_LAYOUT_CTX];
};

struct ui_cache {
	uint32_t id;

	union {
		struct {
			float n1, n2;
		} f;

		struct {
			int n1, n2;
		} i;

		struct {
			float x, y;
		} scroll;

		struct {
			float w, h;
		} dropdown;
	};

	uint8_t type;
	uint8_t _unused[3];
};

struct ui_widget_status {
	int reg_cnt;
	struct ui_cache cache[MAX_WIDGETS];
};

#define MAX_UI_WINDOW 64

struct ui_window_geometry {
	float x, y, w, h;
};

struct ui_window_status {
	int window_cnt;
	struct ui_window_geometry windows[MAX_UI_WINDOW];
};

struct ui_frame {
	uint64_t frequency;
	uint64_t ticks_per_frame;
	uint64_t begin_frame_time;
	uint64_t prev_frame_time;
	uint64_t end_frame_time;
	float target_fps;
	float current_fps;
	float delta_time;
};

struct ui_context {
	struct ui_vertex ui;
	struct ui_layout layout;
	struct ui_theme theme;
	struct ui_input input;
	struct ui_atlas atlas;
	struct ui_frame frame;
	struct ui_event event;
	struct ui_widget_status widget_stat;
	struct ui_window_status window_stat;
};

#endif // __GF_IMGUI_UI_TYPES_INCLUDED_H__
