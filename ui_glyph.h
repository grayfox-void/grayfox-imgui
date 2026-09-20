#ifndef __GF_IMGUI_UI_GLYPH_INCLUDED_H__
#define __GF_IMGUI_UI_GLYPH_INCLUDED_H__

#include "ui_rect.h"
#include "ui_types.h"

#include <assert.h>
#include <stdio.h>

static inline struct ui_glyph *
gf_get_text_glyph(struct ui_atlas *atlas, uint32_t c, ui_font_style style)
{
	assert(atlas != NULL);

	if (c < FONT_FIRST_CHAR || c >= (FONT_FIRST_CHAR + FONT_CHAR_COUNT))
		return &atlas->glyphs[0];

	int target_index = (style * GLYPH_BANK_SIZE) + (c - FONT_FIRST_CHAR);
	return &atlas->glyphs[target_index];
}

static inline __attribute__((always_inline)) struct ui_glyph *
gf_get_icon_glyph(struct ui_atlas *atlas, uint32_t c, ui_font_style style)
{
	assert(atlas != NULL);

	int mask = !(c >= ATLAS_GLYPH_COUNT);
	return &atlas->glyphs[c * mask];
}

static inline struct ui_size ui_glyph_size(struct ui_atlas *atlas,
					   uint32_t glyph_id)
{
	assert(atlas != NULL);
	assert(glyph_id < ATLAS_GLYPH_COUNT);

	struct ui_glyph *g = &atlas->glyphs[glyph_id];

	return (struct ui_size){ .w = g->w * 0.5f, .h = g->h * 0.5f };
}
/*
static inline float gf_put_glyph(struct ui_vertex *restrict ui,
				 struct ui_layout *restrict layout,
				 const struct ui_glyph *restrict glyph,
				 const struct ui_pos pos, uint32_t color)
{
	int sp = layout->lay_ctx_ptr;

	float base_x = layout->ctx[sp].cursor.x + pos.x;
	float base_y = layout->ctx[sp].cursor.y + pos.y;

	float x1 = base_x + glyph->x_offset;
	float y1 = base_y + glyph->y_offset;

	float x2 = x1 + glyph->w * 0.5f;
	float y2 = y1 + glyph->h * 0.5f;

	uint16_t u1 = glyph->u;
	uint16_t v1 = glyph->v;
	uint16_t u2 = u1 + (uint16_t)glyph->w;
	uint16_t v2 = v1 + (uint16_t)glyph->h;

	gf_push_rect_geometry_xyuv(
		ui, layout,
		&(struct ui_rect){ x1, y1, glyph->w * 0.5f, glyph->h * 0.5f },
		&(struct ui_rect){ u1, v1, (uint16_t)glyph->w,
				   (uint16_t)glyph->h },
		color);

	return glyph->advance_x;
}*/

static inline float gf_put_glyph(struct ui_vertex *restrict ui,
				 struct ui_layout *restrict layout,
				 const struct ui_glyph *restrict glyph,
				 const struct ui_pos pos, uint32_t color)
{
	int sp = layout->lay_ctx_ptr;

	float base_x = layout->ctx[sp].cursor.x + pos.x;
	float base_y = layout->ctx[sp].cursor.y + pos.y;

	// Posición física exacta aplicando el desplazamiento limpio de la fuente
	float x1 = base_x + glyph->x_offset;
	float y1 = base_y + glyph->y_offset;

	uint16_t u1 = glyph->u;
	uint16_t v1 = glyph->v;

	// Dibujamos el quad en pantalla usando el tamaño real corregido (mitad de los pixeles del atlas)
	gf_push_rect_geometry_xyuv(
		ui, layout,
		&(struct ui_rect){ x1, y1, glyph->w * 0.5f, glyph->h * 0.5f },
		&(struct ui_rect){ u1, v1, (uint16_t)glyph->w,
				   (uint16_t)glyph->h },
		color);

	// Devolvemos el avance horizontal real sin alteraciones
	return glyph->advance_x;
}

static inline float gf_put_icon_glyph(struct ui_vertex *restrict ui,
				      struct ui_layout *restrict layout,
				      const struct ui_glyph *restrict glyph,
				      const struct ui_pos pos, uint32_t color)
{
	int sp = layout->lay_ctx_ptr;

	float base_x = layout->ctx[sp].cursor.x + pos.x;
	float base_y = layout->ctx[sp].cursor.y + pos.y;

	float x1 = base_x + glyph->x_offset;
	float y1 = base_y + glyph->y_offset;

	uint16_t u1 = glyph->u;
	uint16_t v1 = glyph->v;

	// Para los iconos cargados a mano, el tamaño de pantalla y el tamaño UV son 1:1 (sin el * 0.5f)
	gf_push_rect_geometry_xyuv(
		ui, layout,
		&(struct ui_rect){ x1, y1, glyph->w * 0.5f, glyph->h * 0.5f },
		&(struct ui_rect){ u1, v1, (uint16_t)glyph->w,
				   (uint16_t)glyph->h },
		color);

	return glyph->advance_x;
}

static inline float gf_put_glyph_(struct ui_vertex *restrict ui,
				  struct ui_layout *restrict layout,
				  const struct ui_glyph *restrict glyph,
				  const struct ui_pos pos, uint32_t color)
{
	float base_x = pos.x;
	float base_y = pos.y;

	float x1 = base_x + glyph->x_offset;
	float y1 = base_y + glyph->y_offset;

	float x2 = x1 + glyph->w * 0.5f;
	float y2 = y1 + glyph->h * 0.5f;

	uint16_t u1 = glyph->u;
	uint16_t v1 = glyph->v;
	uint16_t u2 = u1 + (uint16_t)glyph->w;
	uint16_t v2 = v1 + (uint16_t)glyph->h;

	gf_push_rect_geometry_xyuv(
		ui, layout,
		&(struct ui_rect){ x1, y1, glyph->w * 0.5f, glyph->h * 0.5f },
		&(struct ui_rect){ u1, v1, (uint16_t)glyph->w,
				   (uint16_t)glyph->h },
		color);

	return glyph->advance_x;
}

#endif // __GF_IMGUI_UI_GLYPH_INCLUDED_H__
