#ifndef __GF_IMGUI_UI_FRAME_INCLUDED_H__
#define __GF_IMGUI_UI_FRAME_INCLUDED_H__

#include "ui_types.h"

#include <SDL3/SDL_timer.h>

static inline void gf_frame_begin(struct ui_context *ctx)
{
	struct ui_frame *frame = &ctx->frame;

	frame->frequency = SDL_GetPerformanceFrequency();
	frame->ticks_per_frame =
		(uint64_t)((float)frame->frequency / frame->target_fps);

	frame->begin_frame_time = SDL_GetPerformanceCounter();
	frame->delta_time =
		(float)(frame->begin_frame_time - frame->prev_frame_time) /
		(float)frame->frequency;
	frame->prev_frame_time = frame->begin_frame_time;
}

static inline void gf_frame_end(struct ui_context *ctx)
{
	struct ui_frame *frame = &ctx->frame;

	frame->end_frame_time = SDL_GetPerformanceCounter();
	uint64_t curr_ticks = frame->end_frame_time - frame->begin_frame_time;

	if (curr_ticks < frame->ticks_per_frame) {
		uint64_t ticks_left = frame->ticks_per_frame - curr_ticks;
		uint64_t delay = (ticks_left * 1000) / frame->frequency;

		// if (delay > 0)
		// SDL_Delay((uint32_t)delay);
	}
}

#endif // __GF_IMGUI_UI_FRAME_INCLUDED_H__
