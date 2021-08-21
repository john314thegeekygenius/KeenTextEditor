///////////////////////////////////////
///
///  Why you look'n at the headers?
///

#pragma once


#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string>
#include <vector>


// Include SDL2

#include <SDL2/SDL.h>
//#include <SDL2/SDL_ttf.h>

///////
// Keen Stuff

void LoadKeenText(std::string name);
void SaveKeenText(std::string name);
void CoutKeenText(void);
int ParseNumber(std::string str,int &off);
int ParseHex(char hexc);
int MeasureLine(std::string str);
int GetOffsetInStr(std::string str);
void WriteKeenChar(char ch,int x,int y,Uint32 col);
void WriteKeenString(std::string text,int x,int y,Uint32 col,int fontsize = 1);
void WriteKeenStringSmall(std::string text,int x,int y,Uint32 col);
void WriteKeenStringSmart(const std::string &text,int x,int y,int mx,Uint32 col);

void AppendString2(std::string &orgin,std::string append,int offset);

////////
// SDL Stuff

// Returns a value when failed
int SetupSDLVideo();
void ShutdownSDLVideo();

void SDL_ClearScreen(Uint32 col);
void SDL_DrawBar(int x,int y,int w,int h, Uint32 col);
void SDL_DrawBarShrunk(int x,int y,int w,int h, Uint32 col);
SDL_Surface* SDL_LoadOptBmp( std::string path );
SDL_Surface* SDL_LoadOptBmp2( std::string path , Uint32 col);
void SDL_DrawBmp(SDL_Surface* bmp, int x,int y);
void SDL_DrawPartBmp(SDL_Surface* bmp, int x,int y,int offx,int offy,int imgw,int imgh);
void SDL_DrawPartBmpShrunk(SDL_Surface* bmp, int x,int y,int offx,int offy,int imgw,int imgh);
void SDL_WriteString(std::string text,int x,int y,Uint32 col,int fontsize = 1);

void UpdateSDL();
void RenderSDL();

extern int MouseX,MouseY;
extern bool MouseButton;

extern bool TypeFeildEnabled;
extern std::string TypeFeildString;
extern int TypeFeildLocator;
extern int TypeFeildSelectStart;
extern SDL_Surface* Keen_Font2;
extern int SDL_FPS;
extern int SDL_FrameTime;

//////////
// Editor stuff

void KeenTextEditor_LoadGfx();
void KeenTextEditor_UnloadGfx();
int KeenTextEditorLoop();
void KeenHandleKey(int sdlKey);


template<typename T> void VectorPop(std::vector<T> &v, int id){
	v.erase (v.begin() + id);
};

