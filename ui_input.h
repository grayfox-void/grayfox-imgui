#ifndef __GF_IMGUI_UI_INPUT_INCLUDED_H__
#define __GF_IMGUI_UI_INPUT_INCLUDED_H__

#include "ui_constants.h"
#include "ui_types.h"

static inline __attribute__((__always_inline__)) uint32_t
gf_mouse_event(const struct ui_rect rect, const struct ui_input *restrict input)
{
	const struct ui_pos *mpos = &input->mpos;

	float x1 = rect.x;
	float y1 = rect.y;
	float x2 = x1 + rect.w;
	float y2 = y1 + rect.h;

	int maction = ((mpos->x >= x1) & (mpos->y >= y1) & (mpos->x <= x2) &
		       (mpos->y <= y2));

	uint32_t mevent = 0;
	mevent |= ((UI_MOUSE_CLICK * input->mouse_click) * maction);
	mevent |= ((UI_MOUSE_HOVER * input->mouse_hover) * maction);
	mevent |= ((UI_MOUSE_PRESS * input->mouse_press) * maction);
	mevent |= ((UI_MOUSE_DRAG * input->mouse_drag) * maction);

	return mevent;
}

#endif // __GF_IMGUI_UI_INPUT_INCLUDED_H__
