#include "ui_common.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_mouse.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_timer.h>
#include <stddef.h>
#include <stdint.h>

#include <stdio.h>
#include <stdlib.h>

#include "ui_button.h"
#include "ui_constants.h"
#include "ui_dropdown.h"
#include "ui_frame.h"
#include "ui_icon.h"
#include "ui_font.h"
#include "ui_grid.h"
#include "ui_event.h"
#include "ui_grid_layout.h"
#include "ui_main.h"
#include "ui_menu.h"
#include "ui_label.h"
#include "ui_panel.h"
#include "ui_render.h"
#include "ui_selector.h"
#include "ui_types.h"
#include "ui_toolbar.h"
#include "ui_menubar.h"
#include "ui_window.h"

static inline void create_new_document(struct ui_context *ctx)
{
	const float WINDOW_WIDTH = 400;
	const float WINDOW_HEIGHT = 300;
	const float ROW_HEIGHT = 30;
	const float COL_LABEL_W = 100;
	const float COL_SELEC_W = 250;

	gf_window_begin(ctx, "New Map", WINDOW_WIDTH, WINDOW_HEIGHT);
	{
		gf_row_begin(ctx, 40.0f);
		{
			gf_window_header(
				ctx, "Create a new map",
				"Set the type, width and height of the grid");
		}
		gf_row_end(ctx);

		// MAP TYPE
		gf_row_begin(ctx, ROW_HEIGHT);
		{
			gf_col_begin(ctx, COL_LABEL_W);
			{
				gf_label(ctx, "Map type: ");
			}
			gf_col_end(ctx);

			gf_col_begin(ctx, COL_SELEC_W);
			{
				gf_selector(ctx,
					    (const char *[]){ "opt 1",
							      "opt 2" },
					    2);
			}
			gf_col_end(ctx);
		}
		gf_row_end(ctx);

		// MAP ROWS
		gf_row_begin(ctx, ROW_HEIGHT);
		{
			gf_col_begin(ctx, COL_LABEL_W);
			{
				gf_label(ctx, "Map rows: ");
			}
			gf_col_end(ctx);

			gf_col_begin(ctx, COL_SELEC_W);
			{
				gf_selector(ctx,
					    (const char *[]){ "opt 1",
							      "opt 2" },
					    2);
			}
			gf_col_end(ctx);
		}
		gf_row_end(ctx);

		// MAP COLS
		gf_row_begin(ctx, ROW_HEIGHT);
		{
			gf_col_begin(ctx, COL_LABEL_W);
			{
				gf_label(ctx, "Map columns: ");
			}
			gf_col_end(ctx);

			gf_col_begin(ctx, COL_SELEC_W);
			{
				gf_selector(ctx,
					    (const char *[]){ "opt 1",
							      "opt 2" },
					    2);
			}
			gf_col_end(ctx);
		}
		gf_row_end(ctx);

		// footer
		gf_window_footer_begin(ctx);
		{
			gf_row_begin(ctx, 50.0f);
			{
				gf_button_action(ctx, "Cancel");
				gf_button_action(ctx, "Accept");
			}
			gf_row_end(ctx);
		}
		gf_window_footer_end(ctx);
	}
	gf_window_end(ctx);
}

static inline void open_document(struct ui_context *ctx)
{
	const float WINDOW_WIDTH = 800;
	const float WINDOW_HEIGHT = 600;
	const float ROW_HEIGHT = 30;
	const float COL_LABEL_W = 100;
	const float COL_SELEC_W = 250;

	gf_window_begin(ctx, "Open file", WINDOW_WIDTH, WINDOW_HEIGHT);
	{
		gf_row_begin(ctx, 40.0f);
		{
			gf_window_header(ctx, "Open file", "");
		}
		gf_row_end(ctx);

		// footer
		gf_window_footer_begin(ctx);
		{
			gf_row_begin(ctx, 50.0f);
			{
				gf_button_action(ctx, "Cancel");
				gf_button_action(ctx, "Accept");
			}
			gf_row_end(ctx);
		}
		gf_window_footer_end(ctx);
	}
	gf_window_end(ctx);
}

static inline void about(struct ui_context *ctx)
{
	const float WINDOW_WIDTH = 300;
	const float WINDOW_HEIGHT = 200;

	gf_window_begin(ctx, "About", WINDOW_WIDTH, WINDOW_HEIGHT);
	{
		gf_row_begin(ctx, 30.0f);
		{
			gf_window_header(ctx, "About", "About Engine");
		}
		gf_row_end(ctx);
	}
	gf_window_end(ctx);
}

int main(int argc, char *argv[])
{
	static_assert(sizeof(struct ui_glyph) == 32,
		      "¡Alert! Compiler added hidden padding");

	if (!SDL_Init(SDL_INIT_VIDEO)) {
		SDL_Log("Error al inicializar SDL: %s", SDL_GetError());
		return -1;
	}

	SDL_Window *window = SDL_CreateWindow("Mi Motor RPG - Test", 800, 600,
					      SDL_WINDOW_RESIZABLE);
	if (!window) {
		SDL_Log("Error al crear la ventana: %s", SDL_GetError());
		SDL_Quit();
		return -1;
	}

	SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	int viewport_w, viewport_h;
	SDL_GetWindowSizeInPixels(window, &viewport_w, &viewport_h);

	struct ui_context ctx = { 0 };
	struct sdl_vertex sdl_vertices;

	gf_init_imgui(&ctx, viewport_w, viewport_h);

	int atlas_w = 2048;
	int atlas_h = 2048;

	uint32_t *global_rgba_bitmap =
		calloc(atlas_w * atlas_h, sizeof(uint32_t));

	// 2. white pixel at (0,0)
	global_rgba_bitmap[0] = 0xFFFFFFFF;

	SDL_Texture *texture;
	gf_load_font(&ctx, "/usr/share/fonts/Adwaita/AdwaitaMono-Regular.ttf",
		     13.0f, UI_FONT_STYLE_REGULAR, global_rgba_bitmap, atlas_w,
		     atlas_h);
	gf_load_font(&ctx, "/usr/share/fonts/Adwaita/AdwaitaMono-Italic.ttf",
		     13.0f, UI_FONT_STYLE_ITALIC, global_rgba_bitmap, atlas_w,
		     atlas_h);

	gf_load_font(&ctx, "/usr/share/fonts/Adwaita/AdwaitaMono-Bold.ttf",
		     13.0f, UI_FONT_STYLE_BOLD, global_rgba_bitmap, atlas_w,
		     atlas_h);

	int icon_new_symbolic =
		gf_load_icon(&ctx, "assets/icons/document-new-symbolic.bmp",
			     global_rgba_bitmap, atlas_w, atlas_h);

	int icon_open_symbolic =
		gf_load_icon(&ctx, "assets/icons/document-open-symbolic.bmp",
			     global_rgba_bitmap, atlas_w, atlas_h);

	int icon_save_symbolic =
		gf_load_icon(&ctx, "assets/icons/document-save-symbolic.bmp",
			     global_rgba_bitmap, atlas_w, atlas_h);

	int icon_save_as_symbolic =
		gf_load_icon(&ctx, "assets/icons/document-save-as-symbolic.bmp",
			     global_rgba_bitmap, atlas_w, atlas_h);

	int icon_edit_undo_symbolic =
		gf_load_icon(&ctx, "assets/icons/edit-undo-symbolic.bmp",
			     global_rgba_bitmap, atlas_w, atlas_h);

	int icon_edit_redo_symbolic =
		gf_load_icon(&ctx, "assets/icons/edit-redo-symbolic.bmp",
			     global_rgba_bitmap, atlas_w, atlas_h);

	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32,
				    SDL_TEXTUREACCESS_STATIC, atlas_w, atlas_h);

	SDL_UpdateTexture(texture, NULL, global_rgba_bitmap,
			  atlas_w * sizeof(uint32_t));
	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

	free(global_rgba_bitmap);

	printf("Ancho de la V en memoria: %f\n",
	       ctx.atlas.glyphs[86 - 32].advance_x);

	// SDL_LoadBMP("/usr/share/icons/AdwaitaLegacy/32x32/legacy/document-new");

	int running = 1;
	SDL_Event event;
	float mx, my;
	uint8_t menubar_visible = 1;
	uint8_t menu_file_visible = 0;
	uint8_t menuitem_new_visible = 0;
	uint8_t menuitem_open_visible = 0;
	uint8_t menu_edit_visible = 0;
	uint8_t menu_view_visible = 0;
	uint8_t menu_about_visible = 0;
	uint8_t menuitem_about_visible = 0;

	struct ui_input *input = &ctx.input;
	while (running) {
		while (gf_poll_event(&ctx)) {
			switch (ctx.event.type) {
			case SDL_EVENT_QUIT:
				running = 0;
				break;
			case SDL_EVENT_WINDOW_RESIZED:
				SDL_GetWindowSizeInPixels(window, &viewport_w,
							  &viewport_h);
				// update_widgets(&w_system, root_w, root_h);
				break;
			case SDL_EVENT_MOUSE_MOTION:
				SDL_GetMouseState(&mx, &my);
				input->mpos.x = mx;
				input->mpos.y = my;
				input->mouse_hover = !input->mouse_press;
				input->mouse_drag = input->mouse_press;
				// printf("mx: %f my: %f\n", mx, my);
				break;
			case SDL_EVENT_MOUSE_BUTTON_DOWN:
				SDL_GetMouseState(&mx, &my);
				input->mpos.x = mx;
				input->mpos.y = my;
				input->mouse_click = 1;
				input->mouse_press = 1;
				// TODO this has to be set when MENU_OPEN event is triggered
				// ctx.layout.dropdown.width = 0;
				// ctx.layout.dropdown.height = 0;
				gf_dropdown_reset(&ctx.layout);
				break;
			case SDL_EVENT_MOUSE_BUTTON_UP:
				input->mouse_press = 0;
			}
		}

		// gf_viewport_get(&ctx);

		// gf_input_event(&ctx);

		gf_frame_begin(&ctx);

		// TODO refactor to pass only ctx
		gf_ui_begin(&ctx, viewport_w, viewport_h);
		{
			if (menubar_visible) {
				gf_menubar_id_begin(&ctx, 24.0f,
						    GF_ID("menubar"));
				{
					gf_menu_begin(&ctx);
					{
						if (gf_menu_action(&ctx,
								   "File"))
							menu_file_visible =
								!menu_file_visible;

						if (menu_file_visible) {
							gf_menudd_begin(&ctx);
							{
								if (gf_menuitem_action(
									    &ctx,
									    0,
									    icon_new_symbolic,
									    "New...",
									    "Ctrl+N")) {
									menuitem_new_visible =
										!menuitem_new_visible;
								}

								if (gf_menuitem_action(
									    &ctx,
									    0,
									    icon_open_symbolic,
									    "Open...",
									    "Ctrl+O")) {
									menuitem_open_visible =
										!menuitem_open_visible;
								}

								gf_menuitem_separator(
									&ctx);

								if (gf_menuitem_action(
									    &ctx,
									    0,
									    icon_save_symbolic,
									    "Save...",
									    "Ctrl+S")) {
								}
								if (gf_menuitem_action(
									    &ctx,
									    0,
									    icon_save_as_symbolic,
									    "Save As...",
									    "Shift+Ctrl+S")) {
								}

								gf_menuitem_separator(
									&ctx);

								if (gf_menuitem_action(
									    &ctx,
									    0,
									    0,
									    "Quit",
									    "Ctrl+Q")) {
									running =
										0;
								}
							}
							gf_menudd_end(&ctx);
						}
					}
					gf_menu_end(&ctx);

					gf_menu_begin(&ctx);
					{
						if (gf_menu_action(&ctx,
								   "Edit"))
							menu_edit_visible =
								!menu_edit_visible;

						if (menu_edit_visible) {
							gf_menudd_begin(&ctx);
							{
								if (gf_menuitem_action(
									    &ctx,
									    0,
									    icon_edit_undo_symbolic,
									    "Undo",
									    "Ctrl+Z")) {
								}
								if (gf_menuitem_action(
									    &ctx,
									    0,
									    icon_edit_redo_symbolic,
									    "Redo",
									    "Shift+Ctrl+Z")) {
								}

								gf_menuitem_separator(
									&ctx);

								if (gf_menuitem_action(
									    &ctx,
									    0,
									    0,
									    "Cut",
									    "Ctrl+X")) {
								}
								if (gf_menuitem_action(
									    &ctx,
									    0,
									    0,
									    "Copy",
									    "Ctrl+C")) {
								}
								if (gf_menuitem_action(
									    &ctx,
									    0,
									    0,
									    "Paste",
									    "Ctrl+V")) {
								}

								gf_menuitem_separator(
									&ctx);

								if (gf_menuitem_action(
									    &ctx,
									    0,
									    0,
									    "Clear",
									    "Delete")) {
								}
							}
							gf_menudd_end(&ctx);
						}
					}
					gf_menu_end(&ctx);

					gf_menu_begin(&ctx);
					{
						if (gf_menu_action(&ctx,
								   "View"))
							menu_view_visible =
								!menu_view_visible;

						if (menu_view_visible) {
							gf_menudd_begin(&ctx);
							{
								if (gf_menuitem_action(
									    &ctx,
									    0,
									    0,
									    "Full Screen",
									    "F11")) {
								}

								gf_menuitem_separator(
									&ctx);

								if (gf_menuitem_action(
									    &ctx,
									    0,
									    0,
									    "Show Menubar",
									    "")) {
								}
								if (gf_menuitem_action(
									    &ctx,
									    0,
									    0,
									    "Show Toolbar",
									    "")) {
								}
								if (gf_menuitem_action(
									    &ctx,
									    0,
									    0,
									    "Show Statusbar",
									    "")) {
								}
							}
							gf_menudd_end(&ctx);
						}
					}
					gf_menu_end(&ctx);

					gf_menu_begin(&ctx);
					{
						if (gf_menu_action(&ctx,
								   "Help"))
							menu_about_visible =
								!menu_about_visible;

						if (menu_about_visible) {
							gf_menudd_begin(&ctx);
							{
								if (gf_menuitem_action(
									    &ctx,
									    0,
									    0,
									    "About",
									    "")) {
									menuitem_about_visible =
										!menuitem_about_visible;
								}
							}
							gf_menudd_end(&ctx);
						}
					}
					gf_menu_end(&ctx);
				}
			}
			gf_menubar_end(&ctx);

			gf_toolbar_id_begin(&ctx, UI_TOOLBAR_HEIGHT,
					    GF_ID("toolbar"));
			{
				if (gf_toolitem_action(&ctx, 512, "New")) {
					menuitem_new_visible =
						!menuitem_new_visible;
				}

				if (gf_toolitem_action(&ctx, 513, "Open")) {
					menuitem_open_visible =
						!menuitem_open_visible;
				}
			}
			gf_toolbar_end(&ctx);

			gf_panel_v_begin(&ctx, 600.0f);
			{
				gf_panel_h_begin(&ctx, 500.0f);
				{
					gf_grid_interactive(&ctx, 16, 16, 16);
				}
				gf_panel_h_end(&ctx);

				gf_panel_h_begin(&ctx, 400.0f);
				{
				}
				gf_panel_h_end(&ctx);
			}
			gf_panel_v_end(&ctx);

			gf_panel_v_begin(&ctx, 800.0f);
			{
				gf_panel_h_begin(&ctx, 600.0f);
				{
					gf_grid_interactive(&ctx, 100, 100, 32);
				}
				gf_panel_h_end(&ctx);
			}
			gf_panel_v_end(&ctx);

			if (menuitem_new_visible) {
				create_new_document(&ctx);
			}

			if (menuitem_open_visible) {
				open_document(&ctx);
			}

			if (menuitem_about_visible) {
				about(&ctx);
			}
		}

		gf_ui_end(&ctx);

		gf_frame_end(&ctx);

		gf_render_sdl(&ctx, renderer, texture, &sdl_vertices);
	}

	// Limpieza
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
