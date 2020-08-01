#include "sdl2-font.h"
#include "font-chef/font-chef.h"
#include "sdl2-texture.h"

typedef struct KWSDL2_Font {
  struct fc_font * fcFont;
  KW_Texture * kwTexture;
} KWSDL2_Font;

void fc_rect2KW_Rect(struct fc_rect const * src, KW_Rect * dst) {
  dst->x = (int) src->left;
  dst->y = (int) src->top;
  dst->w = (int) (src->right - src->left);
  dst->h = (int) (src->bottom - src->top);
}

KW_Font * KWSDL2_CreateFont(KW_Driver * driver, void * data, float size, KW_Color color) {
  (void) driver;
  KW_Font * kwFont = SDL_malloc(sizeof(*kwFont));
  KWSDL2_Font * sdl2Font = SDL_malloc(sizeof(*sdl2Font));

  struct fc_color fcColor = {
      .r = color.r,
      .g = color.g,
      .b = color.b,
      .a = color.a
  };
  struct fc_font * fcFont = fc_construct(data, fc_px(size), fcColor);
  sdl2Font->fcFont = fcFont;
  kwFont->priv = sdl2Font;
  fc_add(fcFont, fc_basic_latin.first, fc_arabic_supplement.last); // too much? too little? user defined? how?
  fc_cook(fcFont);
  struct fc_pixels const * fcPixels = fc_get_pixels(fcFont);
  sdl2Font->kwTexture = KWSDL2_CreateTexture(driver, fcPixels->data, fcPixels->dimensions.width, fcPixels->dimensions.height, 4);
  return kwFont;
}

void KWSDL2_DestroyFont(KW_Driver * driver, KW_Font * font) {
  (void) driver;
  KWSDL2_Font * sdl2Font = font->priv;

  KWSDL2_DestroyTexture(driver, sdl2Font->kwTexture);
  fc_destruct(sdl2Font->fcFont);
  SDL_free(sdl2Font);
  SDL_free(font);
}

void KWSDL2_RenderText(KW_Driver * driver, KW_Font * font, uint8_t const * text, KW_TextTarget target,  KW_Rect const * clip) {
  KWSDL2_Font * kwsdl2Font = font->priv;
  struct fc_font * fcFont = kwsdl2Font->fcFont;
  size_t len = strlen((char *)text);
  struct fc_character_mapping * mappingBuffer = SDL_malloc(sizeof(*mappingBuffer) * len);
  struct fc_size spaceMetrics = fc_get_space_metrics(fcFont);
  size_t glyphCount = fc_render(fcFont, text, len, mappingBuffer);
  enum fc_alignment alignment = fc_align_left;
  if (target.alignment == KW_ALIGN_CENTER) alignment = fc_align_center;
  if (target.alignment == KW_ALIGN_RIGHT) alignment = fc_align_right;
  if (target.wrap) {
    fc_wrap(mappingBuffer, len, glyphCount, spaceMetrics.height + (float) target.line_spacing, spaceMetrics.width, alignment);
  }

  fc_move(mappingBuffer, glyphCount, 0.0f, target.baseline);

  KW_Rect src, dst;
  for (size_t i = 0; i < glyphCount; i++) {
    fc_rect2KW_Rect(&mappingBuffer[i].source, &src);
    fc_rect2KW_Rect(&mappingBuffer[i].target, &dst);
    KWSDL2_RenderTexture(driver, kwsdl2Font->kwTexture, &src, &dst, clip);
  }

  SDL_free(mappingBuffer);
}
