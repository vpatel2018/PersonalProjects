#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
using namespace std;

/* Creator - Vihan Patel */

/* This file has been created with the SDL2 graphics library.
 * It has been created with the help of some SDL2 tutorials from online.
 * This file has used some help from SDL Wiki. 
 * You must download SDL2 from an outside source (i.e online) if necessary.
 * Download a ttf file and provide the full path to the ttf file when using SDL2.
 * This class provides basic support for drawing lines, rectangles , text and arrows.
 */

class Graphics {
  private:
    SDL_Window * screen;
    SDL_Renderer * display;
    SDL_Surface * text;
    SDL_Texture * show;
    TTF_Font * f;
    int size;
    int width;
    int height;
    const char * pngName;
  public:
    Graphics(int width, int height, const char * pngName, int font);
    void drawRect(int x, int y, int w, int h);
    void drawLine(double x1, double y1, double x2, double y2, int r, int g, int b);
    void drawDashLn(double x1, double y1, double x2, double y2, int r, int g, int b);
    void drawTriangle(double x, double y, int r, int g, int b);
    void drawBackTri(double x, double y, int r, int g, int b);
    void drawArrow(double x, double y, int r, int g, int b);
    void drawDiamond(double x, double y, int r, int g, int b);
    void drawFillDiamond(double x, double y, int font, int r, int g, int b);
    void drawText(const char * words, int x, int y, bool addPlus);
    ~Graphics();
};

void Graphics::drawRect(int x, int y, int w, int h) {
  SDL_Rect rect = {x,y,w,h};
  SDL_RenderDrawRect(display, &rect);
}

void Graphics::drawLine(double x1, double y1, double x2, double y2, int r=0, int g=0, int b=0) {
  SDL_SetRenderDrawColor(display,r,g,b,0);
  SDL_RenderDrawLine(display, x1, y1, x2, y2);
}

void Graphics::drawDashLn(double x1, double y1, double x2, double y2, int r=0, int g=0, int b=0) {
  SDL_SetRenderDrawColor(display,r,g,b,0);
    
  if(y1 > y2) {
    int temp = y1;
    y1 = y2;
    y2 = temp;
  }
    
  if(x1 > x2) {
    int temp = x1;
    x1 = x2;
    x2 = temp;
  }
    
  if(y1 == y2 && x1 != x2) {
    for(int k = 0; x1 < x2 - 10; k++) {
      if(k % 2 == 0) {
        drawLine(x1,y1,x1 + 10,y2,r,g,b);
      }else{
        drawLine(x1,y1,x1 + 10,y2,255,255,255);
      }
      x1 += 10;
    }
    drawLine(x1,y1,x2,y2,r,g,b);
  }else if(y1 != y2 && x1 == x2) {
    int end = (y2 - y1) / 10;
    for(int k = 0; k < end + 1; k++) {
      if(k % 2 == 0) {
        drawLine(x1,y1,x1,y2 + 10,r,g,b);
      }else{
        drawLine(x1,y1,x1,y2 + 10,255,255,255);
      }
      y1 += 10;
    }
  }
      
}

void Graphics::drawTriangle(double x, double y, int r=0, int g=0, int b=0) {
    double k = (size - size/4);
    drawLine(x, y, x + k, y + k, r, g, b);
    drawLine(x, y, x + k, y - k, r, g, b);
    drawLine(x + k, y - k, x + k, y + k, r, g, b);
}

void Graphics::drawBackTri(double x, double y, int r=0, int g=0, int b=0) {
    double k = (size - size/4);
    drawLine(x, y, x - k, y + k, r, g, b);
    drawLine(x, y, x - k, y - k, r, g, b);
    drawLine(x - k, y + k, x - k, y - k, r, g, b);
}

void Graphics::drawArrow(double x, double y, int r=0, int g=0, int b=0) {
    double k = (size - size/4);
    drawLine(x, y, x + k, y + k, r, g, b);
    drawLine(x, y, x + k, y - k, r, g, b);
}

void Graphics::drawDiamond(double x, double y, int r=0, int g=0, int b=0) {
    drawLine(x, y, x + size, y + (size * 0.5), r, g, b);
    drawLine(x, y, x + size, y - (size * 0.5), r, g, b);
    drawLine(x + size, y - (size * 0.5), x + (2 * size), y, r, g, b);
    drawLine(x + size, y + (size * 0.5), x + (2 * size), y, r, g, b);
}

void Graphics::drawFillDiamond(double x, double y, int font, int r=0, int g=0, int b=0) {
    
    if(font == 0) {
      return;
    }
    
    int k = font;
    drawLine(x, y, x + k, y + (k * 0.5), r, g, b);
    drawLine(x, y, x + k, y - (k * 0.5), r, g, b);
    drawLine(x + k, y - (k * 0.5), x + (k * 2), y, r, g, b);
    drawLine(x + k, y + (k * 0.5), x + (k * 2), y, r, g, b);
    
    drawFillDiamond(x, y, k - 1, r, g, b);
    
}

void Graphics::drawText(const char * words, int x, int y, bool addPlus) {
    string wo = words;
    string blank = "";
    if(wo.find("\e[3m") != string::npos && wo.find("\e[4m") != string::npos) {
        if(wo.find("\e[3m") < wo.find("\e[4m")) {
            wo = wo.replace(0, wo.find("[3m") + 3, blank);
            wo = wo.substr(0, wo.find("\e[0m"));
            wo = wo.replace(0, wo.find("[4m") + 3, blank);
            wo = wo.substr(0, wo.find("\e[0m"));
        }else if(wo.find("\e[4m") < wo.find("\e[3m")) {
            wo = wo.replace(0, wo.find("[4m") + 3, blank);
            wo = wo.substr(0, wo.find("\e[0m"));
            wo = wo.replace(0, wo.find("[3m") + 3, blank);
            wo = wo.substr(0, wo.find("\e[0m"));
        }
        TTF_SetFontStyle(f, TTF_STYLE_ITALIC | TTF_STYLE_UNDERLINE);
    }else if(wo.find("\e[3m") != string::npos) {
        wo = wo.replace(0, wo.find("[3m") + 3, blank);
        wo = wo.substr(0, wo.find("\e[0m"));
        TTF_SetFontStyle(f,TTF_STYLE_ITALIC);
    }else if(wo.find("\e[4m") != string::npos) {
        wo = wo.replace(0, wo.find("[4m") + 3, blank);
        wo = wo.substr(0, wo.find("\e[0m"));
        TTF_SetFontStyle(f,TTF_STYLE_UNDERLINE);
    }
    if(addPlus == 1) {
      if(wo.find("_") == 0) {
        wo = "-" + wo;
      }else{
        wo = "+" + wo;
      }
    }
    SDL_Color rgb = {0,0,0};
    text = TTF_RenderText_Solid(f, wo.c_str(), rgb);
    show = SDL_CreateTextureFromSurface(display, text);
    int w;
    int h;
    SDL_QueryTexture(show, NULL, NULL, &w, &h);
    SDL_Rect params = {x, y, w, h};
    SDL_RenderCopy(display, show, NULL, &params);
    TTF_SetFontStyle(f,TTF_STYLE_NORMAL);
    SDL_FreeSurface(text);
    SDL_DestroyTexture(show);
}

Graphics::Graphics(int width, int height, const char * pngName, int font=0) {
    this->width = width;
    this->height = height;
    this->pngName = pngName;
    screen = SDL_CreateWindow("UML", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
    display = SDL_CreateRenderer(screen, -1, 0);
    SDL_SetRenderDrawColor(display, 255, 255, 255, 255);
    SDL_RenderClear(display);
    SDL_SetRenderDrawColor(display, 0, 0, 0, 0);
    TTF_Init();
    f = TTF_OpenFont("Times_New_Roman.ttf", font);
    size = font;
}

Graphics::~Graphics() {
    
    //four lines of code converting a window to png file were borrowed from Gigi Labs tutorial online
    SDL_Surface *surface = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
    SDL_RenderReadPixels(display, NULL, SDL_PIXELFORMAT_ARGB8888, surface->pixels, surface->pitch);
    IMG_SavePNG(surface, pngName);
    SDL_FreeSurface(surface);
    
    SDL_DestroyWindow(screen);
    SDL_DestroyRenderer(display);
    TTF_CloseFont(f);
    
}
