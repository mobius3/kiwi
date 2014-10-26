#ifndef RENDERDRIVER_H
#define RENDERDRIVER_H

typedef void * KW_Texture;
typedef void * KW_Font;

typedef struct KW_Rect {
  unsigned int x;
  unsigned int y;
  unsigned int w;
  unsigned int h;
} KW_Rect;

typedef struct KW_Color {
  unsigned char r;
  unsigned char g;
  unsigned char b;
  unsigned char a;
} KW_Color;

typedef struct KW_RenderDriver KW_RenderDriver;

typedef enum KW_RenderDriver_TextStyle {
  KW_TTF_STYLE_NORMAL        = 0x00,
  KW_TTF_STYLE_BOLD          = 0x01,
  KW_TTF_STYLE_ITALIC        = 0x02,
  KW_TTF_STYLE_UNDERLINE     = 0x04,
  KW_TTF_STYLE_STRIKETHROUGH = 0x08
} KW_RenderDriver_TextStyle;

struct KW_RenderDriver {
  void (*renderCopy)(KW_RenderDriver * driver, KW_Texture src, const KW_Rect * clip, const KW_Rect * dstRect);
  KW_Texture (*renderText)(KW_RenderDriver * driver, const KW_Font font, const char * text, KW_Color color, KW_RenderDriver_TextStyle style);
  KW_Font (*loadFont)(KW_RenderDriver * driver, const char * fontFile, unsigned ptSize);
  KW_Texture (*loadTexture)(KW_RenderDriver * driver, const char * textureFile);
  void (*releaseTexture)(KW_RenderDriver * driver, KW_Texture texture);
  void (*releaseFont)(KW_RenderDriver * driver, KW_Font font);
  void * priv;
};

#endif