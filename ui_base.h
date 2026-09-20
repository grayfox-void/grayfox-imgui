#ifndef __GF_IMGUI_UI_BASE_INCLUDED_H__
#define __GF_IMGUI_UI_BASE_INCLUDED_H__

#include <stdint.h>

enum ui_z_layout {
	UI_Z_LAYOUT_BASE,
	UI_Z_LAYOUT_OVER,
	UI_Z_LAYOUT_DROPDOWN,
	UI_Z_LAYOUT_COUNT
};

typedef uint8_t ui_z_layout;

#define MAX_WIDGETS 1024
#define Z_MAX UI_Z_LAYOUT_COUNT
#define UI_CHANNEL_SIZE MAX_WIDGETS * 16
#define UI_CHANNEL_OFFSET UI_CHANNEL_SIZE
#define MAX_VERTEX (UI_CHANNEL_SIZE * 4)
#define FONT_FIRST_CHAR 32
#define FONT_CHAR_COUNT 96 // from space ' ' to '~'
#define MAX_LAYOUT_CTX 16
#define UI_LAYOUT_SP_START 0
#define GLYPH_BANK_SIZE 128
#define ICON_ROW_START_Y 1024
#define MAX_UI_ICONS 128

#ifndef STB_TRUETYPE_IMPLEMENTATION
#define STB_TRUETYPE_IMPLEMENTATION
#endif

static int rect_indices[MAX_VERTEX];

enum ui_layout_direction { UI_DIRECTION_HORIZONTAL = 1, UI_DIRECTION_VERTICAL };

typedef uint8_t ui_direction;

enum ui_layout_scroll { UI_SCROLL_HORIZONTAL = 1, UI_SCROLL_VERTICAL };

typedef uint8_t ui_scroll;

struct ui_pos {
	float x, y;
};

#define GF_COLOR_MASK(base, action, mask) (base ^ ((base ^ action) & mask))

#if defined(__x86_64__) || defined(_M_X64) || defined(__aarch64__)

#define GF_ASSIGN(out, func, ...) out = func(__VA_ARGS__)
#define GF_ASSIGN_PTR(out_ptr, func, ...) *out_ptr = func(__VA_ARGS__)

#else

// 32 bits (x86, ARM32) or old hardware
#define GF_ASSIGN(out, func, ...) func(__VA_ARGS__, &out)
#define GF_ASSIGN_PTR(out_ptr, func, ...) func(__VA_ARGS__, out_ptr)

#endif

#endif // __GF_IMGUI_UI_BASE_INCLUDED_H__
