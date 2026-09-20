#ifndef __GF_IMGUI_UI_EVENT_INCLUDED_H__
#define __GF_IMGUI_UI_EVENT_INCLUDED_H__

#include "ui_types.h"
#include "SDL3/SDL_events.h"

static inline int gf_poll_event(struct ui_context *ctx)
{
	SDL_Event event;
	int ret;

	ret = SDL_PollEvent(&event);
	ctx->event.type = event.type;

	return ret;
}

#endif // __GF_IMGUI_UI_EVENT_INCLUDED_H__
