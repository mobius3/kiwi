#include "KW_renderdriver_sdl2.h"
#include "KW_renderdriver.h"
#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_image.h"


typedef struct KWSDL {
  SDL_Renderer * renderer;
  SDL_Window * window;
} KWSDL;

static KW_Font KWSDL_loadFont(KW_RenderDriver * driver, const char * font, unsigned ptSize);
static KW_Texture KWSDL_loadTexture(KW_RenderDriver * driver, const char * texturefile);
static KW_Texture KWSDL_renderText(KW_RenderDriver * driver, const KW_Font font, const char * text, KW_Color color, KW_RenderDriver_TextStyle style);
static void KWSDL_releaseTexture(KW_RenderDriver * driver, KW_Texture texture);
static void KWSDL_releaseFont(KW_RenderDriver * driver, KW_Font font);
static void KWSDL_renderCopy(KW_RenderDriver * driver, KW_Texture texture, const KW_Rect * src, const KW_Rect * dst);

struct KW_RenderDriver * KW_CreateSDL2RenderDriver(SDL_Renderer * renderer, SDL_Window * window){
  struct KWSDL * kwsdl = calloc(sizeof(*kwsdl), 1);
  struct KW_RenderDriver * rd = calloc(sizeof(*rd), 1);

  kwsdl->renderer = renderer;
  kwsdl->window = window;

  rd->loadFont = KWSDL_loadFont;
  rd->loadTexture = KWSDL_loadTexture;
  rd->renderText = KWSDL_renderText;
  rd->releaseFont = KWSDL_releaseFont;
  rd->releaseTexture = KWSDL_releaseTexture;
  rd->renderCopy = KWSDL_renderCopy;

  rd->priv = kwsdl;
  return rd;
}

static KW_Font KWSDL_loadFont(KW_RenderDriver * driver, const char * font, unsigned ptSize) {
  TTF_Font * f = TTF_OpenFont(font, ptSize);
  (void)driver;
  if (f == NULL) {
    fprintf(stderr, "KW_RenderDriver_SDL: Could not load font %s: %s\n", font, TTF_GetError());
    return NULL;
  }
  return f;
}

static KW_Texture KWSDL_loadTexture(KW_RenderDriver * driver, const char * texturefile) {
  KWSDL * kwsdl = (KWSDL *) driver->priv;
  SDL_Texture * t = IMG_LoadTexture(kwsdl->renderer, texturefile);
  if (t == NULL) {
    fprintf(stderr, "KW_RenderDriver_SDL: Could not load texture %s: %s\n", texturefile, IMG_GetError());
    return NULL;
  }
  return t;
}

static KW_Texture KWSDL_renderText(KW_RenderDriver * driver, const KW_Font font, const char * text, KW_Color color, KW_RenderDriver_TextStyle style) {
  KWSDL * kwsdl = (KWSDL *) driver->priv;
  int previousstyle;
  SDL_Color sdlcolor;
  SDL_Surface * textsurface;
  SDL_Texture * ret;

  sdlcolor.r = color.r, sdlcolor.g = color.g, sdlcolor.b = color.b, sdlcolor.a = color.a;
  if (font == NULL || text == NULL)
    return NULL;

  previousstyle = TTF_GetFontStyle(font);
  TTF_SetFontStyle(font, style);
  textsurface = TTF_RenderUTF8_Blended(font, text, sdlcolor);
  ret = SDL_CreateTextureFromSurface(kwsdl->renderer, textsurface);
  SDL_FreeSurface(textsurface);
  TTF_SetFontStyle(font, previousstyle);
  return ret;
}

static void KWSDL_releaseTexture(KW_RenderDriver * driver, KW_Texture texture) {
  (void)driver;
  SDL_DestroyTexture(texture);
}
static void KWSDL_releaseFont(KW_RenderDriver * driver, KW_Font font) {
  (void)driver;
  TTF_CloseFont(font);
}

static void KWSDL_renderCopy(KW_RenderDriver * driver, KW_Texture texture, const KW_Rect * src, const KW_Rect * dst) {
  KWSDL * kwsdl = (KWSDL *) driver->priv;
  SDL_Rect srcRect, dstRect;
  srcRect.x = src->x; srcRect.y = src->y; srcRect.w = src->w; srcRect.h = src->h;
  dstRect.x = dst->x; dstRect.y = dst->y; dstRect.w = dst->w; dstRect.h = dst->h;
  SDL_RenderCopy(kwsdl->renderer, texture, &srcRect, &dstRect);
}