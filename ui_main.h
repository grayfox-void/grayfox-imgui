#ifndef __GF_IMGUI_UI_MAIN_INCLUDED_H__
#define __GF_IMGUI_UI_MAIN_INCLUDED_H__

#include "ui_base.h"
#include "ui_constants.h"
#include "ui_layout.h"
#include "ui_types.h"

#include <string.h>

static void gf_init_imgui(struct ui_context *ctx, int viewport_w,
			  int viewport_h)
{
	memset(ctx, 0, sizeof(struct ui_context));

	ctx->layout.max_width = viewport_w;
	ctx->layout.max_height = viewport_h;
	ctx->frame.target_fps = TARGET_FPS;

	for (int i = 0, j = 0; i < MAX_VERTEX; i += 6, j += 4) {
		rect_indices[i] = j;
		rect_indices[i + 1] = j + 1;
		rect_indices[i + 2] = j + 2;
		rect_indices[i + 3] = j;
		rect_indices[i + 4] = j + 2;
		rect_indices[i + 5] = j + 3;
	}

	ctx->theme.style[UI_RECT_MENUBAR].h_rect = UI_MENUBAR_HEIGHT;
	ctx->theme.style[UI_RECT_MENUBAR].w_rect = viewport_w;
	ctx->theme.style[UI_RECT_MENUBAR].padding.x = 0.0f;
	ctx->theme.style[UI_RECT_MENUBAR].padding.y = 0.0f;
	ctx->theme.style[UI_RECT_MENUBAR].bg_color = COLOR_PANEL;

	ctx->theme.style[UI_RECT_MENU].h_rect = UI_MENUBAR_HEIGHT;
	ctx->theme.style[UI_RECT_MENU].w_rect = 0.0f;
	ctx->theme.style[UI_RECT_MENU].padding.x = PAD_MENU_H;
	ctx->theme.style[UI_RECT_MENU].padding.y = PAD_MENU_V;
	ctx->theme.style[UI_RECT_MENU].bg_color = COLOR_PANEL;

	ctx->theme.style[UI_RECT_MENUITEM].h_rect = UI_MENUBAR_HEIGHT;
	ctx->theme.style[UI_RECT_MENUITEM].w_rect = 0.0f;
	// ctx->theme.x_padding[UI_RECT_MENUITEM] = PAD_MENU_H;
	// ctx->theme.y_padding[UI_RECT_MENUITEM] = PAD_MENU_V;
	ctx->theme.style[UI_RECT_MENUITEM].bg_color = COLOR_PANEL;

	ctx->theme.style[UI_RECT_TOOLBAR].h_rect = UI_TOOLBAR_HEIGHT;
	ctx->theme.style[UI_RECT_TOOLBAR].w_rect = viewport_w;
	ctx->theme.style[UI_RECT_TOOLBAR].bg_color = COLOR_PANEL;

	ctx->theme.style[UI_RECT_DROPDOWNITEM].h_rect = UI_DDITEM_HEIGHT;
	ctx->theme.style[UI_RECT_DROPDOWNITEM].w_rect = 0.0f;
	ctx->theme.style[UI_RECT_DROPDOWNITEM].padding.x = 10.0f;
	ctx->theme.style[UI_RECT_DROPDOWNITEM].padding.y = 6.0f;
	ctx->theme.style[UI_RECT_DROPDOWNITEM].bg_color = COLOR_PANEL;

	ctx->theme.style[UI_RECT_BORDER].h_rect = UI_BORDER_THICKNESS;
	ctx->theme.style[UI_RECT_BORDER].w_rect = viewport_w;
	ctx->theme.style[UI_RECT_BORDER].bg_color = COLOR_BORDER;

	ctx->theme.style[UI_RECT_SEPARATOR].h_rect = UI_SEPARATOR_HEIGHT;
	ctx->theme.style[UI_RECT_SEPARATOR].bg_color = COLOR_SEPARATOR;

	ctx->theme.style[UI_RECT_WINDOW].padding.x = 8.0f;
	ctx->theme.style[UI_RECT_WINDOW].padding.y = 8.0f;
	ctx->theme.style[UI_RECT_WINDOW].bg_color = COLOR_PANEL;
}

static inline void gf_ui_begin(struct ui_context *ctx, float screen_w,
			       float screen_h)
{
	struct ui_layout *layout = &ctx->layout;
	gf_layout_reset(layout, screen_w, screen_h);
}

static inline void gf_ui_end(struct ui_context *ctx)
{
	ctx->input.mouse_click = 0;
}

#endif // __GF_IMGUI_UI_MAIN_INCLUDED_H__
