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
extern std::string KeenTextUseFile;
extern std::string KeenTextBmpPref;

extern bool EditorIsRunning;

std::vector<SDL_Surface*> KeenGraphics;

SDL_Surface* Keenicons;
SDL_Surface* KeenColors;

extern Uint32 EGA_Colors[16];


void KeenTextEditor_LoadGfx(){
	for(int bmpID = 0; bmpID < 100; bmpID ++){
		std::string bmpName = "BMP/"+KeenTextBmpPref;
		if(bmpID < 10) bmpName += "000";
		else if(bmpID < 100) bmpName += "00";
		else if(bmpID < 1000) bmpName += "0";

		bmpName +=  std::to_string(bmpID);

		bool FileExist = false;
		std::ifstream bmpf(bmpName+".bmp");
		if(bmpf.is_open()){
			bmpf.close();
			FileExist = true;
			bmpName += ".bmp";
		}else{
			// Check for capitols
			bmpf.open(bmpName+".BMP");
			if(bmpf.is_open()){
				bmpf.close();
				FileExist = true;
				bmpName += ".BMP";
			}
		}
		if(FileExist){
			KeenGraphics.push_back(nullptr);
			KeenGraphics.back() = SDL_LoadOptBmp(bmpName.c_str());
			if(KeenGraphics.back()!=nullptr){
				std::cout << "Loaded " << bmpName << std::endl;
			}
		}else{
//			std::cout << "Could not open " << bmpName << std::endl;
		}
	}

	Keenicons = SDL_LoadOptBmp2("KeenEdit.bmp",EGA_Colors[13]);
	KeenColors = SDL_LoadOptBmp("colors.bmp");

	std::cout << "Done loading graphics " << std::endl;
};

void KeenTextEditor_UnloadGfx(){
	for(int i = 0; i < (int)KeenGraphics.size(); i++){
		SDL_FreeSurface(KeenGraphics[i]);
	}
	KeenGraphics.clear();

	SDL_FreeSurface(Keenicons);
};

bool IconsOpen = false;
bool FloppyMenuOpen = false;
bool EditingText = false;
bool StickersOpen = false;
bool TimerSticker = false;
int StickerId = 0;
int StickerTimeOut = 0;

std::string FloppyLoadStr = "CKD_txt_0000.txt";

extern std::vector <std::string> Keen_Pages;

int KeenPageOn = 0;
int KeenTextX = 10;
int KeenTextY = 10;
int KeenTextColor = 10;

int leftmargin[18],rightmargin[18];
int rowon = 0;

int GraphicsGlobalTimer = 0;
int GraphicsGT = 0;
bool ShowTimedGfxs = true;
int GraphicTimerOn = -1;

int WriteParseString(){
	for (int i=0;i<18;i++) {
		leftmargin[i] = 10;
		rightmargin[i] = 310;
	}

	KeenTextX = 10;
	KeenTextY = 10;
	rowon = 0;

	std::string parsestr = "";

	if(Keen_Pages.size()){
		bool DoneWithLayout = false;
		std::string data = Keen_Pages[KeenPageOn];
		int TimePicCount = 0;
		int TimeOffset = 0;
		int ch = 0;
		int TimerCounter = 0;

		//
		// make sure we are starting layout text (^P first command)
		//
		//while (data[ch] <= 32){
		//	ch++;
		//}

		//if (data[ch] != '^' && toupper(data[++ch]) != 'P'){
		//	std::cout << "Text not headed with ^P" << std::endl;
		//	return -1;
		//}

		while (data[ch++] != '\n');

		do
		{
			if(ch >= (int)data.length()){
				return 0;
//				std::cout << "Read too many chars!" << std::endl;
//				return -1;
			}
			// Process commands
			if(data[ch]=='^'){
				ch ++;
				// Get command identifyer
				if(toupper(data[ch])=='P'||toupper(data[ch])=='E'){
					// It's the end
					DoneWithLayout = true;
				//	PlayedAnimation = true;
					ch--;
				}
				else if(toupper(data[ch])=='C'){
					// Change the color
					KeenTextColor = ParseHex(toupper(data[++ch]));
					ch ++;
				}
				else if(toupper(data[ch])=='B'){
					// Draw a bar
					ch++;
					int gfY = ParseNumber(data,ch);
					ch++;//,
					int gfX = ParseNumber(data,ch);
					ch++;//,
					int gfW = ParseNumber(data,ch);
					ch++;//,
					int gfH = ParseNumber(data,ch);
					ch++;//,
					int gfC = ParseNumber(data,ch);
					SDL_DrawBar(gfX,gfY,gfW,gfH,EGA_Colors[gfC]);
				}else if(toupper(data[ch])=='T'){
					// Draw a bar
					ch++;
					int gfY = ParseNumber(data,ch);
					ch++;//,
					int gfX = ParseNumber(data,ch);
					ch++;//,
					int gfId = ParseNumber(data,ch);
					ch++;//,
					int gfT = ParseNumber(data,ch);
					
					// Fix the graphic offset
					gfId -= 6;
					TimerCounter += 1;
					//std::cout << "Timer: " << TimerCounter << " -> " << gfT << std::endl;

					if(GraphicTimerOn==-1){
						GraphicTimerOn=TimerCounter;
					}
					if(ShowTimedGfxs){
						if(GraphicsGlobalTimer >= gfT+TimeOffset){
							GraphicTimerOn=TimerCounter;
						}
						if(GraphicsGlobalTimer >= gfT+TimeOffset&&GraphicTimerOn==TimerCounter){
							//std::cout <<"Glob:" << GraphicsGlobalTimer << " T1:" << gfT << " T2:" << TimeOffset << std::endl;
							if(gfId>=0 && gfId < (int)KeenGraphics.size()){
								SDL_DrawBmp(KeenGraphics[gfId],gfX,gfY);
							}
							TimeOffset += gfT;
							goto skipbad;
						}
						if(GraphicsGlobalTimer < gfT+TimeOffset&&GraphicTimerOn==TimerCounter){
							return 0;
						}

						if(TimerCounter>GraphicTimerOn)
	 						return 0;
	 					skipbad:;
						
					}
					while (data[ch++] != '\n');		// scan to end of line

				}
				else if(toupper(data[ch])=='G'){
					// Draw a graphic
					ch++;
					int gfY = ParseNumber(data,ch);
					ch++;//,
					int gfX = ParseNumber(data,ch);
					ch++;//,
					int gfId = ParseNumber(data,ch);
					// Fix the graphic offset
					gfId -= 6;

					// Draw the graphic
					if(gfId>=0 && gfId < (int)KeenGraphics.size()){
						SDL_DrawBmp(KeenGraphics[gfId],gfX,gfY);
						int picmid = gfX + KeenGraphics[gfId]->w/2;
						int TextMargin = 0;
						if (picmid > 160)
							TextMargin = gfX-8;			// new right margin
						else
							TextMargin = gfX+KeenGraphics[gfId]->w + 8;	// new left margin

						int top = (gfY-10)/10;
						if (top<0)
							top = 0;
						int bottom = (gfY+KeenGraphics[gfId]->h-10)/10;
						if (bottom>=18)
							bottom = 17;

						for (int ii=top;ii<=bottom;ii++)
							if (picmid > 160)
								rightmargin[ii] = TextMargin;
							else
								leftmargin[ii] = TextMargin;

						//
						// adjust this line if needed
						//

						if(KeenTextX < leftmargin[rowon]){
							KeenTextX = leftmargin[rowon];
						}
					}

					while (data[ch++] != '\n');		// scan to end of line
				}else if(toupper(data[ch])==';'){
					// Comment
					while (data[ch++] != '\n');		// scan to end of line	
				}else if(toupper(data[ch])=='>'){
					//Indent halfway
					KeenTextX = 160;
					ch ++;
				}else if(toupper(data[ch])=='L'){
					// Position the text
					ch ++;
					KeenTextY = ParseNumber(data,ch);
					rowon = (KeenTextY-10)/10;
					KeenTextY = 10+rowon*10;
					ch ++;
					KeenTextX = ParseNumber(data,ch);
					while (data[ch++] != '\n');		// scan to end of line	
				}
				else if(KeenTextExtended){
					if(toupper(data[ch])=='M'){
						// Do nothing
						while (data[ch++] != '\n');		// scan to end of line	
					}else if(toupper(data[ch])=='S'){
						// Do nothing
						while (data[ch++] != '\n');		// scan to end of line	
					}else if(toupper(data[ch])=='D'){
						// Do nothing
						while (data[ch++] != '\n');		// scan to end of line	
					}else if(toupper(data[ch])=='W'){
						// Do nothing
					}
				}
			}else if (data[ch] == 9){
				//Tab
				KeenTextX += 8;
				ch ++;
			}
			else if (data[ch] <= 32){
				// Handle ctrls
				if(data[ch]=='\n'){
					// Next line!
					KeenTextY += 10;
					KeenTextX = leftmargin[++rowon];
					if (rowon == 18){
						// Done 
						DoneWithLayout = true;
						return 0;
					}
				}
				ch += 1;
			}
			else{
				parsestr = "";
				parsestr.push_back(data[ch++]);

				while(data[ch]>32){
					parsestr.push_back((char)data[ch]);
					if(++ch >= (int)data.length()){
						// Uhhh
						std::cout << "Word limit exceeded! --> " << std::string(parsestr) << std::endl;
						return -1;
					}
				}
				if((int)parsestr.length()>=80){
					std::cout << "Word too long!" << std::string(parsestr) << std::endl;
					return -1;
				}
				if(KeenTextX + ((int)parsestr.length()*7) > rightmargin[rowon]){
					// Next line!
					KeenTextY += 10;
					KeenTextX = leftmargin[++rowon];
					if (rowon == 18){
						// Done 
						DoneWithLayout = true;
						return 0;
					}
				}
//				SDL_WriteString(parsestr,KeenTextX,KeenTextY,EGA_Colors[KeenTextColor]);
				WriteKeenString(parsestr,KeenTextX,KeenTextY,EGA_Colors[KeenTextColor]);

				//SDL_DrawBar(KeenTextX,KeenTextY,parsestr.length()*7,8,EGA_Colors[KeenTextColor]);
				//std::cout << parsestr << " " << std::flush;
				KeenTextX += MeasureLine(parsestr);

				while(data[ch]==32){ KeenTextX += 7; ch ++; }
			}
		} while (!DoneWithLayout);
	}
	return 0;
};

std::string EditTextStr = "";

void CopyPageText(){
	int i = 0;
	// Changed the way pages are written in v.1.1
	// make sure we are starting layout text (^P first command)
//	while (Keen_Pages[KeenPageOn][i++] <= 32);
//	while (Keen_Pages[KeenPageOn][i++] != '\n');
	EditTextStr = "";
	while(i < Keen_Pages[KeenPageOn].length()){
		EditTextStr += Keen_Pages[KeenPageOn][i++];
	}
//	EditTextStr.pop_back();
//	EditTextStr.pop_back();
//	EditTextStr.pop_back(); // Remove the ^P					
};

void CopyEditText(){
	// Changed the way pages are written in v.1.1
/*	Keen_Pages[KeenPageOn] = "^P\n"; // Start with page identifyer
	Keen_Pages[KeenPageOn] += EditTextStr;
	if(KeenPageOn==Keen_Pages.size()-1)
		Keen_Pages[KeenPageOn] += "\n^E\n"; // End it.
	else
		Keen_Pages[KeenPageOn] += "\n^P\n"; // End it.
	*/
	Keen_Pages[KeenPageOn] = EditTextStr;
	EditTextStr = "";
};

void AddNewPage(){
	// Changed the way pages are written in v.1.1
	/*
	if(Keen_Pages.size()){
		Keen_Pages.back().pop_back();
		Keen_Pages.back().pop_back();
		Keen_Pages.back().pop_back();
		Keen_Pages.back() += "^P\n"; // Replace the ^E with ^P ???
	}*/
	Keen_Pages.push_back("^P\nNothing Here Yet!\n");
//	Keen_Pages.back() += "^E\n";
};

bool TerminalShow = false;
int TerminateId = 0;

void RemovePage(){
	if(Keen_Pages.size()){
		VectorPop(Keen_Pages,KeenPageOn);
		if(Keen_Pages.size()&&KeenPageOn>=Keen_Pages.size()-1){
			// Remove any old ^P???
	// Changed the way pages are written in v.1.1
//			Keen_Pages.back().pop_back();
//			Keen_Pages.back().pop_back();
//			Keen_Pages.back().pop_back();
//			Keen_Pages.back() += "^E\n";
			KeenPageOn = Keen_Pages.size()-1;
		}
	}else{
		TerminalShow = true;
		srand(SDL_GetTicks());
		TerminateId = rand()%16;
	}
};


void PlopSticker(int x,int y,int id){
	int i = 0; 

	while (Keen_Pages[KeenPageOn][i++] == '\n'); i++; // Skip the thingy
	// Skip to the end of the graphics
	while(Keen_Pages[KeenPageOn][i] == '^'){
		while (Keen_Pages[KeenPageOn][i] != '\n'){
			i++;
		}
		i++; // Skip to end?
	}
	std::string GfxStr = ""; 
	if(TimerSticker){
		GfxStr = "^T";
		GfxStr+=std::to_string(y);
		GfxStr+=",";
		GfxStr+=std::to_string(x);
		GfxStr+=",";
		GfxStr+=std::to_string(id+6);
		GfxStr+=",";
		GfxStr+=std::to_string(StickerTimeOut);
		GfxStr+="\n";
	}else{
		GfxStr = "^G"; 
		GfxStr+=std::to_string(y);
		GfxStr+=",";
		GfxStr+=std::to_string(x);
		GfxStr+=",";
		GfxStr+=std::to_string(id+6);
		GfxStr+="\n";
	}
	AppendString2(Keen_Pages[KeenPageOn],GfxStr,i);
};

// Some dumb joke strings
std::string TerminateText[16] = {
	"Welp, you deleted the program.",
	"There's nothing to delete dummy.",
	"Rage deleteing is a bad habit.",
//	"Welcome to MS-DOS v5.0, Hall.", (My brother said to remove this)
	"Also try TED5!",
	"The End.",
	"End of Text.",
	"Segmentation fault",
	"PageLayout: Word limit exceeded",
	"PageLayout: Text not headed with ^P",
	"CacheLayoutGraphics: No ^E to terminate file!",
	"PageLayout: No page to remove",
	"Baby shark do do do do do do!",
	"Too many TSRs! unload some TSRs or rename your CONFIG.SYS\n and AUTOEXEC.BAT to free up more memory.",
	"Demo loop exited???",
	"Improper video card!  If you really have an EGA/VGA card \nthat I am not detecting, use the -HIDDENCARD command line parameter!",
	"Please check your manual and try again."
};
int T = 0;
int KeenTextEditorLoop(){

	// Troll for people who delete pages they shouldn't
	if(TerminalShow){
		SDL_ClearScreen(EGA_Colors[0]);
		std::string termTxt = TerminateText[TerminateId]+std::string("\nC:\\>");
		if(T>=250){
			termTxt += "_";
		}
		WriteKeenStringSmall(termTxt,0,0,EGA_Colors[7]);
		T++;
		if(T>500){
			T = 0;
		}
		return 0;
	}

	// Input Logic
	if(!StickersOpen){
		if(MouseY <= 0){
			IconsOpen = true;
		}
		if(IconsOpen && MouseY > 40){
			IconsOpen = false;
		}
	}

	if(FloppyMenuOpen){
		if(MouseX > 80&& MouseY > 60 && MouseX < 80+160 && MouseY < 60+80){
		}else if(MouseButton){
			// Close
			TypeFeildEnabled = false;
			FloppyMenuOpen = false;
		}
	}

	if(IconsOpen){
		if(MouseX < 32){
			if(MouseButton){
				// Save / Load Menu
				FloppyMenuOpen = true;
				IconsOpen = false;
				StickersOpen = false;
				if(EditingText){
					CopyEditText();
					EditingText = false;
					TypeFeildEnabled = true;
					TypeFeildString = "";
				}
			}
		}else if(MouseX < 64){
			if(MouseButton){
				// Edit the text
				EditingText = true;
				if(Keen_Pages.size()<=0){
					AddNewPage();
				}
				TypeFeildEnabled = true;
				StickersOpen = false;
				IconsOpen = false;
				FloppyMenuOpen = false;
				CopyPageText();
				TypeFeildString = EditTextStr;
			}
		}else if(MouseX < 96){
			if(MouseButton){
				// Stickers!
				StickersOpen = true;
				TimerSticker = false;
				IconsOpen = false;
				MouseButton = false;
				FloppyMenuOpen = false;
				if(EditingText){
					CopyEditText();
					EditingText = false;
					TypeFeildEnabled = true;
					TypeFeildString = "";
				}
			}
		}else if(MouseX < 128){
			if(MouseButton){
				// Timed Stickers!
				StickersOpen = true;
				TimerSticker = true;
				StickerTimeOut = 30;
				IconsOpen = false;
				MouseButton = false;
				FloppyMenuOpen = false;
				if(EditingText){
					CopyEditText();
					EditingText = false;
					TypeFeildEnabled = true;
					TypeFeildString = "";
				}
			}
		}else if(MouseX < 160){
			if(MouseButton){
				// Add Page
				StickersOpen = false;
				IconsOpen = false;
				MouseButton = false;
				FloppyMenuOpen = false;
				if(EditingText){
					CopyEditText();
					EditingText = false;
					TypeFeildEnabled = true;
					TypeFeildString = "";
				}
				AddNewPage();
			}
		}else if(MouseX < 192){
			if(MouseButton){
				// Remove Page
				StickersOpen = false;
				IconsOpen = false;
				MouseButton = false;
				FloppyMenuOpen = false;
				EditingText = false;
				TypeFeildEnabled = true;
				TypeFeildString = "";
				RemovePage();
			}
		}

	}

	if(EditingText)
		if(TypeFeildEnabled)
				EditTextStr = TypeFeildString;
	if(FloppyMenuOpen){
		if(MouseButton){
			if(MouseX > 96 && MouseY > 68 && MouseX < 96+56 && MouseY < 68+16){
				// Save
				SaveKeenText(FloppyLoadStr);
				FloppyMenuOpen = false;
				TypeFeildEnabled = false;
			}
			if(MouseX > 168 && MouseY > 68 && MouseX < 168+56 && MouseY < 68+16){
				// Load
				LoadKeenText(FloppyLoadStr);
				KeenPageOn = 0;
				FloppyMenuOpen = false;
				TypeFeildEnabled = false;
				GraphicsGlobalTimer = 0;
				GraphicTimerOn = -1;
			}
			if(MouseX > 96 && MouseY > 100 && MouseX < 96+128 && MouseY < 124){
				TypeFeildEnabled = true;
				TypeFeildString = FloppyLoadStr;
			}else{
				TypeFeildEnabled = false;
			}
		}

		if(TypeFeildEnabled)
			FloppyLoadStr = TypeFeildString;
	}


	// Draw the background
	SDL_ClearScreen(EGA_Colors[4]);
	if(KeenGraphics.size()>=78){
		SDL_DrawBmp(KeenGraphics[74],0,0);
		SDL_DrawBmp(KeenGraphics[75],0,8);
		SDL_DrawBmp(KeenGraphics[76],312,8);
		SDL_DrawBmp(KeenGraphics[77],8,176);
	}

	// Write the page info
	if(Keen_Pages.size()){
		WriteKeenString(std::string("pg ")+std::to_string(KeenPageOn+1)+ std::string(" of ") + std::to_string(Keen_Pages.size()),218,186,EGA_Colors[12]);
	}else{
		WriteKeenString("Add new page",218,186,EGA_Colors[12]);		
	}

	// Write the text to the screen
	if(EditingText){
		SDL_DrawBmp(KeenColors,80,190);
		WriteKeenStringSmart(TypeFeildString,40,40,600,EGA_Colors[15]);
	}else{
		int errorCode = WriteParseString();
		if(errorCode!=0){
			return errorCode;
		}
	}

	if(IconsOpen)
		SDL_DrawPartBmp(Keenicons,0,0,32,0,192,32);

	if(StickersOpen){
		if(MouseButton){
			// Add sticker! :D
			PlopSticker(MouseX&0xFFFE,MouseY,StickerId);
		}
		if(StickerId < KeenGraphics.size()){
			SDL_DrawBmp(KeenGraphics[StickerId],MouseX&0xFFFFFE,MouseY);
			if(TimerSticker){
				WriteKeenStringSmall(std::to_string(StickerTimeOut),
					((MouseX&0xFFFFFE)+(KeenGraphics[StickerId]->w))*2,MouseY*2,EGA_Colors[0]);
			}
		}
	}

	if(FloppyMenuOpen){
		// Draw
		SDL_DrawBar(80,60,160,80,EGA_Colors[7]);
		// Buttons
		if(MouseX > 96 && MouseY > 68 && MouseX < 96+56 && MouseY < 68+16){
			SDL_DrawBar(96,68,56,16,EGA_Colors[0]);
		}else{
			SDL_DrawBar(96,68,56,16,EGA_Colors[8]);
		}
		WriteKeenString("SAVE",98,68,EGA_Colors[15]);

		if(MouseX > 168 && MouseY > 68 && MouseX < 168+56 && MouseY < 68+16){
			SDL_DrawBar(168,68,56,16,EGA_Colors[0]);
		}else{
			SDL_DrawBar(168,68,56,16,EGA_Colors[8]);
		}
		WriteKeenString("LOAD",168,68,EGA_Colors[15]);

		// Text Input
		if(TypeFeildEnabled){
			SDL_DrawBar(96,100,128,24,EGA_Colors[0]);
		}else{
			SDL_DrawBar(96,100,128,24,EGA_Colors[8]);
		}
		WriteKeenString(FloppyLoadStr,98,108,EGA_Colors[15]);
		if(TypeFeildEnabled){
			if(SDL_GetTicks()%100 > 50)
				WriteKeenChar(127,98+GetOffsetInStr(FloppyLoadStr)+5,118,EGA_Colors[15]);
		}
	}

	if(ShowTimedGfxs){
		WriteKeenStringSmall("On",0,0,EGA_Colors[15]);
	}else{
		WriteKeenStringSmall("Off",0,0,EGA_Colors[15]);	
	}

	int fpscol = 15;
	if(SDL_FPS>=24)
		fpscol = 15;
	else if(SDL_FPS>20)
		fpscol = 12;
	else if(SDL_FPS>15)
		fpscol = 6;
	else
		fpscol = 4;
	WriteKeenStringSmall("FPS:"+std::to_string(SDL_FPS),0,10,EGA_Colors[fpscol]);
	if(SDL_FrameTime<50)
		fpscol = 15;
	else if(SDL_FrameTime<100)
		fpscol = 12;
	else if(SDL_FrameTime<200)
		fpscol = 6;
	else
		fpscol = 4;
	WriteKeenStringSmall("Frame Time:"+std::to_string(SDL_FrameTime)+"ms",0,20,EGA_Colors[fpscol]);

	// Always draw mouse on top
	if(!StickersOpen){
		SDL_DrawPartBmp(Keenicons,MouseX,MouseY,0,0,24,24);
	}

	// TODO:
	// Make it update at the correct tps
//	GraphicsGT += 1;
//	if(GraphicsGT>10){
//		GraphicsGT = 0;
//		GraphicsGlobalTimer += 1;
//		std::cout << GraphicsGlobalTimer << std::endl;
//	}
	GraphicsGlobalTimer += 1;
	return 0;
};




void KeenHandleKey(int sdlKey){
	if(TerminalShow){
		return;
	}
	switch(sdlKey){
		case SDLK_LEFT:
			if(IconsOpen){
				MouseX -= 32;
				if(MouseX < 0) MouseX = 0;
			}else if(StickersOpen){
				StickerId -= 1;
				if(StickerId<0) StickerId = 0;
			}else if(!EditingText && !FloppyMenuOpen){
				KeenPageOn -= 1;
				GraphicsGlobalTimer = 0;
				GraphicTimerOn = -1;
				if(KeenPageOn<0) KeenPageOn = 0;
			}

		break;
		case SDLK_RIGHT:
			if(IconsOpen){
				MouseX += 32;
				if(MouseX > 160) MouseX = 160;
			}else if(StickersOpen){
				StickerId += 1;
				if(StickerId >= KeenGraphics.size()) StickerId = KeenGraphics.size()-1;
			}else if(!EditingText&& !FloppyMenuOpen){
				KeenPageOn += 1;
				GraphicsGlobalTimer = 0;
				GraphicTimerOn = -1;
				if(KeenPageOn>=Keen_Pages.size()) KeenPageOn = Keen_Pages.size()-1;
			}
		break;
		case SDLK_ESCAPE:
			if(FloppyMenuOpen){
				TypeFeildEnabled = false;
				FloppyMenuOpen = false;
			}
			if(EditingText){
				CopyEditText();
				EditingText = false;
				TypeFeildEnabled = true;
				TypeFeildString = "";
			}
			if(StickersOpen){
				StickersOpen = false;
			}
			MouseX = 0;
			MouseY = 0;
		break;
		case SDLK_EQUALS:
			if(StickersOpen){
				if(TimerSticker){
					StickerTimeOut += 1;
				}
			}
			break;
		case SDLK_MINUS:
			if(StickersOpen){
				if(TimerSticker){
					StickerTimeOut -= 1;
				}
			}
			break;
		case SDLK_TAB:
			ShowTimedGfxs = !ShowTimedGfxs;
			break;

	}
};