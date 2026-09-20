#ifndef __GF_IMGUI_UI_RENDER_INCLUDED_H__
#define __GF_IMGUI_UI_RENDER_INCLUDED_H__

#include "ui_constants.h"
#include "ui_types.h"

#include <stdio.h>
#include <SDL3/SDL_render.h>

struct sdl_vertex {
	float xy[MAX_VERTEX * 2];
	SDL_FColor color[MAX_VERTEX];
	float uv[MAX_VERTEX * 2];
};

static inline uint32_t gf_render_channel(struct ui_context *ctx,
					 SDL_Renderer *renderer,
					 SDL_Texture *texture,
					 struct sdl_vertex *vertices,
					 uint8_t z_index, uint32_t offset)
{
	struct ui_vertex *ui = &ctx->ui;
	struct ui_atlas *atlas = &ctx->atlas;

	uint32_t count = ui->vertex_count[z_index];
	// printf("Z: %d COUNT: %d\n", z_index, count);
	// printf("COUNT + COUNT >> 1: %d\n", count + (count >> 1));

	for (uint32_t i = offset, j = 0; i < offset + count; ++i, ++j) {
		struct rect_vertex *src =
			&ui->vertices[UI_CHANNEL_OFFSET * z_index + j];

		vertices->xy[i * 2 + 0] = src->x;
		vertices->xy[i * 2 + 1] = src->y;

		vertices->color[i].r = ((src->color >> 24) & 0xFF) * INV_255;
		vertices->color[i].g = ((src->color >> 16) & 0xFF) * INV_255;
		vertices->color[i].b = ((src->color >> 8) & 0xFF) * INV_255;
		vertices->color[i].a = (src->color & 0xFF) * INV_255;

		vertices->uv[i * 2 + 0] = (float)src->u * atlas->inv_w;
		vertices->uv[i * 2 + 1] = (float)src->v * atlas->inv_h;
	}
	/*
	if (z_index == UI_Z_LAYOUT_OVER) {
		// printf("SRC VERTEX\n");
		for (uint32_t i = offset; i < count; ++i) {
			struct rect_vertex *src =
				&ui->vertices[UI_CHANNEL_OFFSET * z_index + i];
			// printf("x: %f y: %f", src->x, src->y);
			if (i % 5 == 0)
				putchar('\n');
		}
		putchar('\n');

		printf("SDL VERTEX\n");
		for (uint32_t i = offset; i < count; ++i) {
			printf("x: %f y: %f", vertices->xy[i * 2 + 0],
			       vertices->xy[i * 2 + 1]);
			if (i % 5 == 0)
				putchar('\n');
		}
		putchar('\n');
	} */
	/*
	 *    SDL_RenderGeometryRaw(renderer, texture, vertices->xy, sizeof(float) * 2,
	 *                          vertices->color, sizeof(SDL_FColor), vertices->uv,
	 *                          sizeof(float) * 2, count, rect_indices,
	 *                          count + (count >> 1), sizeof(int));
	 */
	return count;
}

static inline void gf_render_sdl(struct ui_context *ctx, SDL_Renderer *renderer,
				 SDL_Texture *texture,
				 struct sdl_vertex *vertices)
{
	SDL_SetRenderDrawColor(renderer, 0x2C, 0x2C, 0x2C, 0xFF);
	SDL_RenderClear(renderer);

	uint32_t count = 0;
	struct ui_vertex *ui = &ctx->ui;

#pragma unroll Z_MAX
	for (int i = 0; i < Z_MAX; ++i) {
		count += gf_render_channel(ctx, renderer, texture, vertices, i,
					   count);
		ui->vertex_count[i] = 0;
	}

	// printf("Render count: %d\n", count);

	SDL_RenderGeometryRaw(renderer, texture, vertices->xy,
			      sizeof(float) * 2, vertices->color,
			      sizeof(SDL_FColor), vertices->uv,
			      sizeof(float) * 2, count, rect_indices,
			      count + (count >> 1), sizeof(int));

	SDL_RenderPresent(renderer);
}

#endif // __GF_IMGUI_UI_RENDER_INCLUDED_H__
