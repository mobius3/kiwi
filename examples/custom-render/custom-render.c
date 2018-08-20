#include <stdio.h>
#include "SDL.h"
#include "SDL_image.h"
#include "KW_gui.h"
#include "KW_frame.h"
#include "KW_label.h"
#include "KW_renderdriver_sdl2.h"

#define rightOf(a) (a).x + (a).w
#define bottomOf(a) (a).y + (a).h

int min3(int a, int b, int c) {
  return (a < b) ? (a < c ? a : c) : (b < c ? b : c);
}

int max3(int a, int b, int c) {
  return (a > b) ? (a > c ? a : c) : (b > c ? b : c);
}

/* Skin texture for the frame */
SDL_Texture * skin;

/* Skin clipping setup for frame widgets */
struct WidgetFace {
  /* Client area background clip */
  SDL_Rect background;
  
  /* If true, background will be tiled (not stretched) */
  int backgroundTiled;
    
  /* Cliprects for the border parts
   * Vertical and horizontal parts will be tiled when the widget texture compiles
   */
  SDL_Rect left;
  SDL_Rect right;
  SDL_Rect top;
  SDL_Rect bottom;
  
  SDL_Rect top_left;
  SDL_Rect top_right;
  SDL_Rect bottom_left;
  SDL_Rect bottom_right;
  
  /* Border part glyphs may extend out of the widget's rect.
   * These overlapped stripes form a margin around the widget box.
   */
  int left_dx;
  int right_dx;
  int top_dy;
  int bottom_dy;
  
  SDL_Point top_left_d;
  SDL_Point top_right_d;
  SDL_Point bottom_left_d;
  SDL_Point bottom_right_d;
  
  int margin_left;
  int margin_right;
  int margin_top;
  int margin_bottom;
} widgetFace;

/* Draw texture tiled within clip region */
int tileClipped(SDL_Renderer * renderer, SDL_Texture * texture, const SDL_Rect * source, const SDL_Rect * clip) {
  SDL_Rect dst;
	dst.x = clip->x; dst.y = clip->y; dst.w = source->w; dst.h = source->h;
  
  SDL_RenderSetClipRect(renderer, clip);
    
  while (dst.y < clip->h) {
    dst.x = clip->x;
    while (dst.x < clip->w) {
      SDL_RenderCopy(renderer, texture, source, &dst);
      dst.x += dst.w;
    }
    dst.y += dst.h;
  }

  SDL_RenderSetClipRect(renderer, NULL);
  
  return 1;
}

/* Get texture for a frame with the given size */
SDL_Texture *getWidgetTexture(const struct WidgetFace * face, SDL_Renderer * renderer, SDL_Texture * skin, int w, int h) {
	SDL_Rect clip;
	SDL_Rect bar_segment;

  int bar_right = w + face->margin_left;
  int bar_bottom = h + face->margin_top;

  SDL_Texture *tex = SDL_CreateTexture(renderer,
   SDL_PIXELFORMAT_RGBA8888,
   SDL_TEXTUREACCESS_TARGET,
   face->margin_left + w + face->margin_right,
   face->margin_top + h + face->margin_bottom);
  if (!tex) return NULL;
  
  SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
  SDL_SetRenderTarget(renderer, tex);
  SDL_RenderClear(renderer);

  // Background
  clip.x = face->margin_left; clip.y= face->margin_top; clip.w = w; clip.h = h;
  if (face->backgroundTiled) {
    // Tiled
    tileClipped(renderer, skin, &face->background, &clip);
  } else {
    // Stretched
    SDL_RenderCopy(renderer, skin, &face->background, &clip);
  }
  
  // Left
  clip.x = face->margin_left - face->left_dx;
  clip.y = face->margin_top;
  clip.w = face->left.w;
  clip.h = h;
  SDL_RenderSetClipRect(renderer, &clip);
  bar_segment.x = clip.x; bar_segment.y = clip.y; bar_segment.w = face->left.w; bar_segment.h = face->left.h;
  while (bar_segment.y < clip.h) {
    SDL_RenderCopy(renderer, skin, &face->left, &bar_segment);
    bar_segment.y += bar_segment.h;
  }
  
  // Right
  clip.x = bar_right - face->right_dx;
  clip.w = face->right.w;
  SDL_RenderSetClipRect(renderer, &clip);
  bar_segment.x = clip.x; bar_segment.y = clip.y; bar_segment.w = face->right.w; bar_segment.h = face->right.h;
  while (bar_segment.y < clip.h) {
    SDL_RenderCopy(renderer, skin, &face->right, &bar_segment);
    bar_segment.y += bar_segment.h;
  }
  
  // Top
  clip.x = face->margin_left;
  clip.y = face->margin_top - face->top_dy;
  clip.w = w;
  clip.h = face->top.h;
  SDL_RenderSetClipRect(renderer, &clip);
  bar_segment.x = clip.x; bar_segment.y = clip.y; bar_segment.w = face->top.w; bar_segment.h = face->top.h;
  while (bar_segment.x < clip.w) {
    SDL_RenderCopy(renderer, skin, &face->top, &bar_segment);
    bar_segment.x += bar_segment.w;
  }
  
  // Bottom
  clip.y = bar_bottom - face->bottom_dy;
  clip.h = face->bottom.h;
  SDL_RenderSetClipRect(renderer, &clip);
  bar_segment.x = clip.x; bar_segment.y = clip.y; bar_segment.w = face->bottom.w; bar_segment.h = face->bottom.h;
  while (bar_segment.x < clip.w) {
    SDL_RenderCopy(renderer, skin, &face->bottom, &bar_segment);
    bar_segment.x += bar_segment.w;
  }
  
  SDL_RenderSetClipRect(renderer, NULL);

  // Top left
  clip.w = face->top_left.w;
  clip.h = face->top_left.h;
  clip.x = face->margin_left - face->top_left_d.x;
  clip.y = face->margin_top - face->top_left_d.y;
  SDL_RenderCopy(renderer, skin, &face->top_left, &clip);
  
  // Top right
  clip.w = face->top_right.w;
  clip.h = face->top_right.h;
  clip.x = bar_right - face->top_right_d.x;
  clip.y = face->margin_top - face->top_right_d.y;
  SDL_RenderCopy(renderer, skin, &face->top_right, &clip);

  // Bottom left
  clip.w = face->bottom_left.w;
  clip.h = face->bottom_left.h;
  clip.x = face->margin_left - face->bottom_left_d.x;
  clip.y = bar_bottom - face->bottom_left_d.y;
  SDL_RenderCopy(renderer, skin, &face->bottom_left, &clip);

  // Bottom right
  clip.w = face->bottom_right.w;
  clip.h = face->bottom_right.h;
  clip.x = bar_right - face->bottom_right_d.x;
  clip.y = bar_bottom - face->bottom_right_d.y;
  SDL_RenderCopy(renderer, skin, &face->bottom_right, &clip);

  SDL_SetRenderTarget(renderer, NULL);
  return tex;
}

/* Load texture skin clipping setup for frames */
void loadFrameSkin(struct WidgetFace * face) {
	SDL_Rect box;
	box.x = 23; box.y = 119; box.w = 118; box.h = 87;

	face->background.x = 0; face->background.y = 0; face->background.w = 100; face->background.h = 100;
	face->left.x = 14; face->left.y = 137; face->left.w = 19; face->left.h = 19;
	face->right.x = 131; face->right.y = 137; face->right.w = 19; face->right.h = 19;
	face->top.x = 72; face->top.y = 110; face->top.w = 19; face->top.h = 19;
	face->bottom.x = 72; face->bottom.y = 196; face->bottom.w = 19; face->bottom.h = 19;
	face->top_left.x = 14; face->top_left.y = 110; face->top_left.w = 19; face->top_left.h = 19;
	face->top_right.x = 131; face->top_right.y = 110; face->top_right.w = 19; face->top_right.h = 19;
	face->bottom_left.x = 16; face->bottom_left.y = 159; face->bottom_left.w = 55; face->bottom_left.h = 55;
	face->bottom_right.x = 94; face->bottom_right.y = 159; face->bottom_right.w = 55; face->bottom_right.h = 55;
	
  int right_bar = box.x + box.w;
  int bottom_bar = box.y + box.h;

  face->margin_left = box.x - min3(face->top_left.x, face->left.x, face->bottom_left.x);
  face->margin_right = max3(rightOf(face->top_right), rightOf(face->right), rightOf(face->bottom_right)) - right_bar;
  face->margin_top = box.y - min3(face->top_left.y, face->top.y, face->top_right.y);
  face->margin_bottom = max3(bottomOf(face->bottom_left), bottomOf(face->bottom), bottomOf(face->bottom_right)) - bottom_bar;
  
  face->left_dx = box.x - face->left.x;
  face->right_dx = right_bar - face->right.x;
  face->top_dy = box.y - face->top.y;
  face->bottom_dy = bottom_bar - face->bottom.y;
  
  face->top_left_d.x = box.x - face->top_left.x;
  face->top_left_d.y = box.y - face->top_left.y;
  
  face->top_right_d.x = right_bar - face->top_right.x;
  face->top_right_d.y = box.y - face->top_right.y;
  
  face->bottom_left_d.x = box.x - face->bottom_left.x;
  face->bottom_left_d.y = bottom_bar - face->bottom_left.y;

  face->bottom_right_d.x = right_bar - face->bottom_right.x;
  face->bottom_right_d.y = bottom_bar - face->bottom_right.y;
	
	face->backgroundTiled = 1;
}

/* Rendering function for frame */
KW_Texture * renderFrame (KW_RenderDriver * renderer, KW_Widget * widget, KW_Surface * tileset, int w, int h) {
	KW_Texture * result = SDL_malloc(sizeof(KW_Texture));
	printf("Render frame\n");
	result->texture = getWidgetTexture(&widgetFace, KW_RenderDriverGetSDL2Renderer(renderer), skin, w, h);
	return result;
}

int main(int argc, char ** argv) {
  /* initialize window and renderer */
  KW_RenderDriver * driver;
  SDL_Window * window;
  SDL_Renderer * renderer;
  KW_Surface * set;
  KW_GUI * gui;
  KW_Font * font;
  KW_Rect framegeom, labelgeom;
  KW_Widget * frame;
	KW_Color color;
	
  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_CreateWindowAndRenderer(320, 240, 0, &window, &renderer);
  SDL_SetRenderDrawColor(renderer, 200, 100, 100, 1);
  driver = KW_CreateSDL2RenderDriver(renderer, window);

  /* load tileset */
  set = KW_LoadSurface(driver, "tileset.png");
	skin = IMG_LoadTexture(renderer, "skin.png");
	loadFrameSkin(&widgetFace);
  
  /* initialize gui */
  gui = KW_Init(driver, set);
  font = KW_LoadFont(driver, "Fontin-Regular.ttf", 20);
  KW_SetFont(gui, font);
	color.r = color.g = color.b = color.a = 255;
	KW_SetTextColor(gui, color);
  
  framegeom.x = 20, framegeom.y = 20, framegeom.w = 160, framegeom.h = 120;
  labelgeom = framegeom; labelgeom.x = labelgeom.y = 0;

  /* create frame with label */
  frame =  KW_CreateFrame(gui, NULL, &framegeom);
	KW_CreateLabel(gui, frame, "Frame", &labelgeom);

	/* assign custom rendering function to the frame */
	KW_SetWidgetCustomRenderFunction(frame, renderFrame);
  
	while (!SDL_QuitRequested()) {
    SDL_RenderClear(renderer);
    KW_ProcessEvents(gui);
    KW_Paint(gui);
    SDL_RenderPresent(renderer);
    SDL_Delay(1);
  }
  
  /* free stuff */
  KW_Quit(gui);
  KW_ReleaseFont(driver, font);
  KW_ReleaseSurface(driver, set);
  KW_ReleaseRenderDriver(driver);
	SDL_DestroyTexture(skin);
  SDL_Quit();
  
  return 0;
}
