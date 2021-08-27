///////////////////////////////////////////////////////////////
///
///         Commander Keen Galaxy Text Editor
///                     V.1.1
///
///              Written By: John314
///                 on Aug 20, 2021
///
///                   Updated on 
///                 on Aug 27, 2021
///
/////////////////////////////////////////////////////////////////
///
///          NOTE: This program is not bug free, and may have problems.
///                If you encounter a problem, please let me know so I can
///                attempt to fix it.
///
///                There are a few compatibility issues in this program, such
///                as timed graphics not timed correctly.
///
///                Please always make sure the final result is correct by checking in
///                your mod.
///
/////////////////////////////////////////////////////////////////
///
///                   About the code:
///
///           The code I've written isn't really "readable", so
///           don't go snooping around for great programing practices.
///
///           Here's a break down of the source files:
///
///                  main.cpp       -> The file that hold the main function
///                  Editor.cpp     -> The file that holds most of the editor logic
///                  Keen_Text.cpp  -> Contains most things Keen Text related
///                  SDL_Stuff.cpp  -> Holds most of the SDL call stuff
///
/////////////////////////////////////////////////////////////////
///
///   The code written is "As is", and I'm not responsible if you use it and have problems.
///   
///   This software comes with no guarantee that it will not destroy your computer.
///   If it does, I have no idea how that would happen.
///
/////////////////////////////////////////////////////////////////
///
///
///
///
///
///
///
///
//////////////////////////////////////////////////////////////////
///
///  At this point I just don't want you to look at the code.
///
//////////////////////////////////////////////////////////////////


#include "heads.h"

extern bool KeenTextExtended;

/*

From the Keen Source Code


TEXT FORMATTING COMMANDS
------------------------
^C<hex digit>  			Change text color
^E[enter]				End of layout (all pages)
^G<y>,<x>,<pic>[ENTER]	Draw a graphic and push margins
^P[enter]				start new page, must be the first chars in a layout
^L<x>,<y>[ENTER]		Locate to a specific spot, x in pixels, y in lines

^B<y>,<x>,<w>,<h>,<col>[ENTER]    Draw a bar 
^; coment goes here[ENTER]        Comment (Doesn't show up)
^T<y>,<x>,<pic>,<ticks>[ENTER]    Waits for a number of tics, then draws pic
^>                                Indent halfway

K1n9_Duk3 additions:

^M<x>                   Play Music for level x
^S<x>                   Play sound number x
^W                      Wait until sound is done
^D<ticks>               Waits for a number of tics
^Q                      Stop sound and music

*/


Uint32 EGA_Colors[16] = {
	0x000000,
	0x0000a8,
	0x00a800,
	0x00a8a8,
	0xa80000,
	0xa800a8,
	0xa85400,
	0xa8a8a8,
	0x545454,
	0x5454fc,
	0x54fc54,
	0x54fcfc,
	0xfc5454,
	0xfc54fc,
	0xfcfc54,
	0xfcfcfc,
};



std::vector <std::string> Keen_Pages;

int GetChar(FILE *fp){
	int ch = fgetc(fp);
	if(ch==-1){
		std::cout << "Error! Found end of file." << std::endl;
		fclose(fp);
		exit(0);
		return 0;
	}
	return ch;
};

void LoadKeenText(std::string name){

	FILE *fp;
	fp = fopen(name.c_str(),"rb");
	if(!fp){
		std::cout << "Error opening " << name << std::endl;
		return;
	}
	Keen_Pages.clear();
	while(1){
		int ch = GetChar(fp);
		if(ch==EOF){ fclose(fp); return; }
		if(ch=='^'){
			// It's a command
			ch = GetChar(fp);
			// Changed the way pages are read in v.1.1
			if(ch == 'P' || ch == 'E'){
				if(ch=='E'){
					std::cout << "Found end of file." << std::endl;
					std::cout << "Loaded " << std::to_string(Keen_Pages.size()) << " pages!" << std::endl;
					fclose(fp);
					return;
				}
				Keen_Pages.push_back("");
			}else {
				if(Keen_Pages.size()){
					Keen_Pages.back().push_back('^');
					Keen_Pages.back().push_back(ch);
				}
			}
		}else{
			if(Keen_Pages.size()){
				Keen_Pages.back().push_back(ch);
			}
		}
	}
};

void SaveKeenText(std::string name){
	// Changed the way pages are written in v.1.1

	FILE *fp;
	fp = fopen(name.c_str(),"wb");
	if(!fp){
		std::cout << "Error opening " << name << std::endl;
		return;
	}
	fputc('^',fp);
	fputc('P',fp);
	fputc('\n',fp);
	for(int e = 0; e < Keen_Pages.size()-1; e++){
		for(int i = 0; i < Keen_Pages[e].length(); i++)
			fputc(Keen_Pages[e][i],fp);
		fputc('\n',fp);
		fputc('^',fp);
		fputc('P',fp);
		fputc('\n',fp);
	}
	for(int i = 0; i < Keen_Pages.back().length(); i++)
		fputc(Keen_Pages.back()[i],fp);
	/// make sure there is always a end
	fputc('^',fp);
	fputc('E',fp);
	fputc('\n',fp);

	fclose(fp);

	std::cout << "Saved to " << name << std::endl;

};

void CoutKeenText(void){
	for(std::string s : Keen_Pages){
		std::cout << "Page >>" << s << std::endl;
	}
};


int ParseNumber(std::string str,int &off){
	int val = 0;
	// Find Number
	for(int i = 0; i < (int)str.length()-off; i++){
		if(str[i+off]>='0'&&str[i+off]<='9'){
			off += i;
			goto donesearch;
		}
	}
	donesearch:
	// Parse it
	for(int i = off; i < (int)str.length(); i++){
		if(str[i]>='0'&&str[i]<='9'){
			val *= 10;
			val += str[i]-'0';
			off += 1;
		}else{
			return val;
		}
	}
	return val;
};

int ParseHex(char hexc){
	for(int i = 0; i < 16; i ++){
		if(hexc=="0123456789ABCDEF"[i])
			return i;
	}
	return 0;
};


int Keen_Font_Widths[128] = {
	5, 3, 6, 8, 7, 8, 8, 4, 5, 5, 9, 7, 5, 7, 3, 8,
	8, 5, 7, 7, 8, 7, 7, 7, 7, 7, 3, 4, 6, 7, 7, 7,
	8, 7, 7, 7, 7, 7, 7, 7, 7, 5, 7, 8, 7, 8, 8, 7,
	7, 7, 7, 7, 7, 7, 7, 8, 8, 7, 8, 5, 8, 5, 7, 9,
	4, 7, 7, 7, 7, 7, 7, 7, 7, 3, 6, 7, 3,11, 7, 7,
	7, 7, 7, 7, 7, 7, 7,11, 8, 7, 7, 7, 3, 7, 8, 8,
	2, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,11,11
};

int MeasureLine(std::string str){
	int length_pix = 0;
	for(char ch : str){
		if(ch >= 32 && ch < 128)
			length_pix += Keen_Font_Widths[ch-32];
	}
	return length_pix;
};

int GetOffsetInStr(std::string str){
	int length_pix = 0;
	if(TypeFeildLocator==-1){
		return -10;
	}
	for(int i = 0; i < str.length(); i++){
		char ch = str[i];
		if(i==TypeFeildLocator)
			return length_pix;
		if(ch >= 32 && ch < 128)
			length_pix += Keen_Font_Widths[ch-32];
	}
	return length_pix;
};

void WriteKeenChar(char ch,int x,int y,Uint32 col){
	SDL_Color txtcol = {
		static_cast<Uint8>((col>>16)&0xFF),
		static_cast<Uint8>((col>>8)&0xFF),
		static_cast<Uint8>((col&0xFF))
	};
	if(ch>=32){
		int dchy = (ch-32) / 16;
		int dchx = (ch-32) - (dchy * 16);
		SDL_SetSurfaceColorMod(Keen_Font2,txtcol.r,txtcol.g,txtcol.b);
		SDL_DrawPartBmp(Keen_Font2,x,y,dchx*11,dchy*10,11 ,10 );
		SDL_SetSurfaceColorMod(Keen_Font2,255,255,255);
	}
};

void WriteKeenString(std::string text,int x,int y,Uint32 col,int fontsize){
	if(text.length() <= 0) return;
	SDL_Color txtcol = {
		static_cast<Uint8>((col>>16)&0xFF),
		static_cast<Uint8>((col>>8)&0xFF),
		static_cast<Uint8>((col&0xFF))
	};
	int charX = x;
	SDL_SetSurfaceColorMod(Keen_Font2,txtcol.r,txtcol.g,txtcol.b);
	for(char ch : text){
		if(ch>=32){
			int dchy = (ch-32) / 16;
			int dchx = (ch-32) - (dchy * 16);
			SDL_DrawPartBmp(Keen_Font2,charX,y,dchx*11 * fontsize,dchy*10 * fontsize,11 * fontsize,10 * fontsize);
			charX += Keen_Font_Widths[ch-32]*fontsize;
		}
	}
	SDL_SetSurfaceColorMod(Keen_Font2,255,255,255);
};


void WriteKeenStringSmall(std::string text,int x,int y,Uint32 col){
	if(text.length() <= 0) return;
	SDL_Color txtcol = {
		static_cast<Uint8>((col>>16)&0xFF),
		static_cast<Uint8>((col>>8)&0xFF),
		static_cast<Uint8>((col&0xFF))
	};
	int charX = x;
	int charY = y;
	SDL_SetSurfaceColorMod(Keen_Font2,txtcol.r,txtcol.g,txtcol.b);
	for(char ch : text){
		if(ch>=32){
			int dchy = (ch-32) / 16;
			int dchx = (ch-32) - (dchy * 16);
			SDL_DrawPartBmpShrunk(Keen_Font2,charX,charY,dchx*11,dchy*10,11 ,10);
			charX += Keen_Font_Widths[ch-32];
		}else{
			if(ch=='\n'){
				charX = x;
				charY += 10;
			}
		}
	}
	SDL_SetSurfaceColorMod(Keen_Font2,255,255,255);
};


void WriteKeenStringSmart(const std::string &text,int x,int y,int mx,Uint32 col){
	if(SDL_GetTicks()%100 > 50){
		if(TypeFeildLocator<=0){
			// Show it a x,y
			int locX = 7;
			SDL_DrawPartBmpShrunk(Keen_Font2,x+locX-5,y+8,15*11,5*10,11,10);
		}
	}
	if(text.length() <= 0) return;
	SDL_Color txtcol = {
		static_cast<Uint8>((col>>16)&0xFF),
		static_cast<Uint8>((col>>8)&0xFF),
		static_cast<Uint8>((col&0xFF))
	};
	int charX = x;
	int charY = y;
	SDL_SetSurfaceColorMod(Keen_Font2,txtcol.r,txtcol.g,txtcol.b);
	int charcount = 0;
	for(int i = 0; i < text.length(); i++){
		char ch = text[i];

		if(TypeFeildSelectStart!=-1){
			int TSS = TypeFeildSelectStart;
			int TSE = TypeFeildLocator;
			if(TSS > TSE){ TSE = TypeFeildSelectStart; TSS = TypeFeildLocator;}
			if(charcount>= TSS && charcount < TSE){
				int locX = 7;
				if(ch>32){locX = (Keen_Font_Widths[ch-32]);}
				SDL_DrawBarShrunk(charX,charY,locX,10,EGA_Colors[8]);
			}
		}
		if(text[i]>32){
			int dchy = (ch-32) / 16;
			int dchx = (ch-32) - (dchy * 16);
			if(dchx>15) dchx = 15;
			if(dchy>6) dchx = 6;
			SDL_DrawPartBmpShrunk(Keen_Font2,charX,charY,dchx*11,dchy*10,11,10);
		}else{
			if(text[i]=='\n'){
				charX = x;
				charY += 10;
			}
		}
		if(SDL_GetTicks()%100 > 50){
			if(charcount == TypeFeildLocator){
				int locX = 7;
				if(ch>32){locX = (Keen_Font_Widths[ch-32]);}
				SDL_DrawPartBmpShrunk(Keen_Font2,charX+locX-5,charY+8,15*11,5*10,11,10);
			}
		}
		charcount += 1;
		if(text[i]>=32){
			charX += Keen_Font_Widths[ch-32];
		}
		if(charX > mx){
			charX = x;
			charY += 10;
		}
	}

	SDL_SetSurfaceColorMod(Keen_Font2,255,255,255);
};