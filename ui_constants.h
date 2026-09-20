#ifndef __GF_IMGUI_UI_CONSTANTS_INCLUDED_H__
#define __GF_IMGUI_UI_CONSTANTS_INCLUDED_H__

#define CLEAR_MASK 0x00

#define OFFSET_FONT_REGULAR 0
#define OFFSET_FONT_BOLD 256
#define OFFSET_FONT_ITALIC 512
#define OFFSET_BANK_ICONS 1024

#define EVAL_CONDITION(condition) (-(condition))

static inline __attribute__((always_inline)) float float_eval(int a, int b,
							      int mask)
{
	return (float)((a & mask) | (b & ~mask));
}

static inline __attribute__((always_inline)) int int_eval(int a, int b,
							  int mask)
{
	return (int)((a & mask) | (b & ~mask));
}

#define UI_DD_DEPTH_MAX 4
#define WIDGETS_MASK MAX_WIDGETS / 32
#define Z_WIDGET 3

#define TARGET_FPS 60.0f

#define UI_MENUBAR_HEIGHT 24.0f
#define UI_TOOLBAR_HEIGHT 40.0f
#define UI_STATUSBAR_HEIGHT 24.0f
#define UI_SIDEBAR_WIDTH 240.0f
#define UI_BORDER_THICKNESS 1.0f
#define UI_SEPARATOR_HEIGHT 1.0f
#define UI_DDITEM_HEIGHT 32.0f

#define UI_MENUBAR_Z 0
#define UI_MENU_Z 0
#define UI_TOOLBAR_Z 0
#define UI_TOOLITEM_Z 0
#define UI_STATBAR_Z 0
#define UI_SIDEBAR_Z 0
#define UI_MENUITEM_Z 1
#define UI_DROPDOWN_Z 1
#define UI_MODAL_Z 2

// ui_theme.h - Paleta de colores nativa para SDL
#define COLOR_ROOT 0x2C2C2CFF
#define COLOR_PANEL 0x383838FF
#define COLOR_BTN_NORMAL 0x3C3C3CFF
#define COLOR_BTN_CLICK 0x212121FF
#define COLOR_BTN_HOVER 0x212121FF
#define COLOR_BORDER 0x242424FF
#define COLOR_ACCENT 0x007ACCFF
#define COLOR_TEXT 0xF5F5F5FF
#define COLOR_MENU_PRESSED 0x1C1C1CFF
#define COLOR_SEPARATOR 0x2B2B2BFF
#define FONT_PATH "./VCR_MONO24_bitmap.bmp"

#define PAD_BTN_V 6.0f
#define PAD_BTN_H 12.0f
#define PAD_MENU_V 2.0f
#define PAD_MENU_H 6.0f
#define PAD_TOOLBAR_H 8.0f
#define PAD_TOOLBAR_V 4.0f
#define PAD_MENU_IT_V 6.0f
#define PAD_MENU_IT_H 12.0f
#define PAD_TFIELD_V 5.0f
#define PAD_TFIELF_H 10.0f
#define PAD_PANEL_V 8.0f
#define PAD_PANEL_H 8.0f

#define UI_ELEMENT_GAP 6.0f

#define WIDGET_HOVERABLE 0x02
#define WIDGET_CLICKABLE 0x01

#define WIDGET_NOT_HOVERABLE 0xFD
#define WIDGET_NOT_CLICKABLE 0xFE

#define WIDGET_ENABLE 0x02
#define WIDGET_VISIBLE 0x01

#define WIDGET_DISABLE 0xFD
#define WIDGET_INVISIBLE 0xFE

#define UI_MENU_DD_TOP_OFF 0
#define UI_MENU_DD_RIG_OFF UI_MENU_DD_TOP_OFF + 4
#define UI_MENU_DD_BOT_OFF UI_MENU_DD_RIG_OFF + 4
#define UI_MENU_DD_LEF_OFF UI_MENU_DD_BOT_OFF + 4

#define ROOT_VERTEX 4

#define UI_MOUSE_CLICK 0x01
#define UI_MOUSE_HOVER 0x02
#define UI_MOUSE_PRESS 0x04
#define UI_MOUSE_DRAG 0x08

#define GF_MOUSE_EV_CLICK(m_ev) (m_ev & UI_MOUSE_CLICK)
#define GF_MOUSE_EV_HOVER(m_ev) ((m_ev & UI_MOUSE_HOVER) >> 1)
#define GF_MOUSE_EV_PRESS(m_ev) ((m_ev & UI_MOUSE_PRESS) >> 2)

#define UI_EVENT_BORDER_SIZE 10.0f

const float INV_255 = 1.0f / 255.0f;

#endif // __GF_IMGUI_UI_CONSTANTS_INCLUDED_H__
