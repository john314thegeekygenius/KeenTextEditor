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


// The window we'll be rendering to
SDL_Window* Keen_Window = NULL;

// The window surface
SDL_Surface* Keen_Surface = NULL;

//Globally used font
//TTF_Font *Keen_Font = NULL;
SDL_Surface* Keen_Font2;

int SDL_FPS = 0;
int SDL_FrameTime = 0;
int frames = 0;
Uint32 Fstart = 0;
Uint32 lastTime = 0;

int SetupSDLVideo(){
	// Create a window
	Keen_Window = SDL_CreateWindow(
		((std::string)("Keen Text Editor - v.1.0")).c_str(),// Window title
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		640,      // width, in pixels
		400,     // height, in pixels
		0 // Flags
		);

	// Check that the window was successfully created
	if (Keen_Window == NULL) {
		std::cout << "Failed to create SDL Window!" << 
			"SDL-Error:: " << SDL_GetError() << std::endl;
		return - 1;
	}

	std::cout << ("Created SDL Window!") << std::endl;

	SDL_SetWindowMinimumSize(Keen_Window, 640, 400);
	SDL_SetWindowMaximumSize(Keen_Window, 640, 400);


	Keen_Surface = SDL_GetWindowSurface( Keen_Window );

	if(Keen_Surface == NULL){
		// WHAT???
		std::cout << "Failed to get SDL surface!" << 
			"SDL-Error:: " << SDL_GetError() << std::endl;
		return -2;
	}

	std::cout << ("Got SDL surface!") << std::endl;


	// Initialize SDL_ttf
	/*
	if( TTF_Init() == -1 ) {
		std::cout <<  "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << std::endl;
		return -3;
	}*/
	// Open the font
	/*
    Keen_Font = TTF_OpenFont( "ck_small.fon", 16 );
    if( Keen_Font == NULL ) {
        std::cout << "Failed to load keen font! SDL_ttf Error:" << TTF_GetError() << std::endl;
        return -4;
    }
    */
    Keen_Font2 = SDL_LoadOptBmp2("font.bmp",0);

	return 0;
};

void ShutdownSDLVideo(){
	// Close and destroy the window
	if (Keen_Window) {
		SDL_DestroyWindow(Keen_Window);
		Keen_Window = NULL;
		std::cout << ("Destoyed Window") << std::endl;
	}

	if(Keen_Font2){
		SDL_FreeSurface(Keen_Font2);
	}

    //Quit SDL subsystems
//    TTF_Quit();
    SDL_Quit();
};

void SDL_ClearScreen(Uint32 col){
	SDL_DrawBar(0,0,320,200,col);
};

void SDL_DrawBar(int x,int y,int w,int h, Uint32 col){
	SDL_Rect drawRect = { x*2, y*2, w*2, h*2};
	SDL_FillRect(Keen_Surface,&drawRect,col);

};

void SDL_DrawBarShrunk(int x,int y,int w,int h, Uint32 col){
	SDL_Rect drawRect = { x, y, w, h};
	SDL_FillRect(Keen_Surface,&drawRect,col);
};

void SDL_DrawBmp(SDL_Surface* bmp, int x,int y){
	SDL_Rect drawRect = { x*2, y*2, bmp->w*2, bmp->h*2};
	if(bmp==NULL || bmp==nullptr)
		return;
	SDL_BlitScaled( bmp, NULL, Keen_Surface, &drawRect );
};

void SDL_DrawPartBmp(SDL_Surface* bmp, int x,int y,int offx,int offy,int imgw,int imgh){
	SDL_Rect drawRect = { x*2, y*2, imgw*2, imgh*2};
	SDL_Rect getRect = { offx, offy, imgw, imgh};
	if(bmp==NULL || bmp==nullptr)
		return;
	SDL_BlitScaled( bmp, &getRect, Keen_Surface, &drawRect );
};


void SDL_DrawPartBmpShrunk(SDL_Surface* bmp, int x,int y,int offx,int offy,int imgw,int imgh){
	SDL_Rect drawRect = { x, y, imgw, imgh};
	SDL_Rect getRect = { offx, offy, imgw, imgh};
	if(bmp==NULL || bmp==nullptr)
		return;
	SDL_BlitScaled( bmp, &getRect, Keen_Surface, &drawRect );
};

void RenderSDL(){
	
	SDL_UpdateWindowSurface( Keen_Window );
	frames += 1;

	Uint32 timeout = SDL_GetTicks();
	if(lastTime <= timeout){
		SDL_FPS = frames;
		lastTime = SDL_GetTicks() + 1000;
		frames = 0;
	}
	// Cap the FPS to 24 fps
	SDL_FrameTime = (SDL_GetTicks()-(Fstart));
    int wait_time = (1000.0f / (float)(60)) - SDL_FrameTime;
    if(wait_time>0){
        SDL_Delay(wait_time);
    }
	Fstart = SDL_GetTicks();

};


SDL_Surface* SDL_LoadOptBmp( std::string path ) {
	//The final optimized image
	SDL_Surface* optimizedSurface = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = SDL_LoadBMP( path.c_str() );
	if( loadedSurface == NULL ) {
		std::cout << "Unable to load image " << path << std::endl;
		std::cout << "SDL Error: " << SDL_GetError() << std::endl;
	}
	else
	{
		//Convert surface to screen format
		optimizedSurface = SDL_ConvertSurface( loadedSurface, Keen_Surface->format, 0 );
		if( optimizedSurface == NULL )
		{
			std::cout << "Unable to optimize image " << path << std::endl;
			std::cout << "SDL Error: " << SDL_GetError() << std::endl;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	return optimizedSurface;
};

SDL_Surface* SDL_LoadOptBmp2( std::string path , Uint32 col) {
	//The final optimized image
	SDL_Surface* optimizedSurface = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = SDL_LoadBMP( path.c_str() );
	if( loadedSurface == NULL ) {
		std::cout << "Unable to load image " << path << std::endl;
		std::cout << "SDL Error: " << SDL_GetError() << std::endl;
	}
	else
	{
		//Convert surface to screen format
		optimizedSurface = SDL_ConvertSurface( loadedSurface, Keen_Surface->format, 0 );
		if( optimizedSurface == NULL )
		{
			std::cout << "Unable to optimize image " << path << std::endl;
			std::cout << "SDL Error: " << SDL_GetError() << std::endl;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );

		SDL_SetColorKey(optimizedSurface,SDL_TRUE,col);
	}

	return optimizedSurface;
};



// Text

//Render text surface
void SDL_WriteString(std::string text,int x,int y,Uint32 col,int fontsize){
	if(text.length() <= 0) return;
	SDL_Color txtcol = {
		static_cast<Uint8>((col>>16)&0xFF),
		static_cast<Uint8>((col>>8)&0xFF),
		static_cast<Uint8>((col&0xFF))
	};
/*
    SDL_Surface* textSurface = TTF_RenderText_Solid( Keen_Font, text.c_str(), txtcol );
    if( textSurface == NULL ){
        std::cout << "Unable to render text surface! SDL_ttf Error:" << TTF_GetError() << std::endl;
    }
    SDL_Rect drawRect = { x*2, y*2, (int)text.length()*32*fontsize, 32*fontsize};
    SDL_BlitSurface(textSurface,NULL,Keen_Surface,&drawRect);
    SDL_FreeSurface(textSurface);
    */

};

///////////////////////////////////////////
/////////////////////////////////////////
// Input


SDL_Event Keen_Events;

int MouseX = 0,MouseY = 0;
bool MouseButton = false;

extern bool EditorIsRunning;

bool TypeFeildEnabled = false;
bool TypeFeildBoolean = false;
std::string TypeFeildString = "";
int TypeFeildSelectStart = -1;
int TypeFeildLocator = 0;

// From Editor.cpp
extern bool EditingText;

void AppendString(std::string append){
	// make sure it's not on the end
	if(TypeFeildLocator==TypeFeildString.length()-1){
		TypeFeildString+=append;
		return;
	}

	std::string oldstr = TypeFeildString;
	TypeFeildString = "";
	for(int i = 0; i < oldstr.length(); i++){
		if(i==TypeFeildLocator+1){
         	TypeFeildString += append;
		}
   		TypeFeildString.push_back(oldstr[i]);
	}
};


void AppendString2(std::string &orgin,std::string append,int offset){
	// make sure it's not on the end
	if(offset==orgin.length()-1){
		orgin+=append;
		return;
	}

	std::string oldstr = orgin;
	orgin = "";
	for(int i = 0; i < oldstr.length(); i++){
		if(i==offset){
         	orgin += append;
		}
   		orgin.push_back(oldstr[i]);
	}
};

void StringPop(std::string &s, int id){
	if(id < 0 || id >= s.length())
		return;
	s.erase (s.begin() + id);
};

void UpdateSDL(){
	MouseButton = false;

	if(TypeFeildEnabled){
		if(TypeFeildBoolean==false){
			SDL_StartTextInput();
			TypeFeildBoolean = true;
			TypeFeildLocator = TypeFeildString.length()-1;
		}
	}else{
		if(TypeFeildBoolean==true){
			SDL_StopTextInput();
			TypeFeildBoolean = false;
		}
	}
	while (SDL_PollEvent(&Keen_Events)) {
		switch (Keen_Events.type) {
			case SDL_QUIT:
				std::cout << "Closed Application." << std::endl;
				EditorIsRunning = false;
				break;
			case SDL_KEYDOWN:
				if(TypeFeildEnabled){
					//Handle backspace
	                if( Keen_Events.key.keysym.sym == SDLK_BACKSPACE && TypeFeildString.length() > 0 && TypeFeildLocator > -1)
	                {
	                    //lop off character
	                    StringPop(TypeFeildString,TypeFeildLocator);
	                    TypeFeildLocator -= 1;
	                    //TypeFeildString.pop_back();
	                }
	                // Handle Delete
	                else if( Keen_Events.key.keysym.sym == SDLK_DELETE && TypeFeildString.length() > 0 )
	                {
	                    // Delete all selected text
	                    if(TypeFeildSelectStart!=-1){
							int x = TypeFeildSelectStart, y = TypeFeildLocator;
	                		if(y < x) { int t = y; y = x; x = t;} // swap
	                		for(int txti = x; txti < y; txti++){
			                    StringPop(TypeFeildString,x);
	                		}
	                    }else{
	                    	if(TypeFeildLocator < TypeFeildString.length()-1){
			                    //lop off character
			                    StringPop(TypeFeildString,TypeFeildLocator+1);
			                }
		                }
	                } // handle enter
	                else if( Keen_Events.key.keysym.sym == SDLK_RETURN){
	                	if(EditingText){
							AppendString("\n");
							TypeFeildLocator += 1;
						}else{
		                	TypeFeildEnabled = false;
		                }
	                } // handle ctrl +_A
	                else if( Keen_Events.key.keysym.sym == SDLK_a && SDL_GetModState() & KMOD_CTRL ){
	                	TypeFeildSelectStart = 0;
	                	TypeFeildLocator = TypeFeildString.length()-1;
	                }  // handle Home
	                else if( Keen_Events.key.keysym.sym == SDLK_HOME){
						TypeFeildLocator = -1;
	                }// handle Home
	                else if( Keen_Events.key.keysym.sym == SDLK_END){
						TypeFeildLocator = TypeFeildString.length()-1;
	                }
	                //Handle copy
	                else if( Keen_Events.key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL )
	                {
	                	std::string selectedStr = "";
	                	int x = TypeFeildSelectStart, y = TypeFeildLocator;
	                	if(y < x) { int t = y; y = x; x = t;} // swap
	                	if(x < 0) x = 0;
	                	for(int i = x; i < y; i++)
	                		selectedStr.push_back(TypeFeildString[i]);
	                    SDL_SetClipboardText( selectedStr.c_str() );
	                }
	                //Handle paste
	                else if( Keen_Events.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL )
	                {
	                	AppendString(std::string(SDL_GetClipboardText()));
	                	TypeFeildLocator += strlen(SDL_GetClipboardText());
	                }else if(Keen_Events.key.keysym.sym == SDLK_LEFT){
	                	if(SDL_GetModState() & KMOD_CTRL)
	                		TypeFeildLocator -= 4;
	                	else if(SDL_GetModState() & KMOD_SHIFT){
	                		if(TypeFeildSelectStart==-1)
	                			TypeFeildSelectStart = TypeFeildLocator;
	                		TypeFeildLocator -= 1;
	                	}
	                	else {
	                		TypeFeildLocator -= 1;
	                		TypeFeildSelectStart = -1;
	                	}
	                	if(TypeFeildString[TypeFeildLocator]=='\n'){
	                	//	TypeFeildLocator -= 1;
	                	}
	                }else if(Keen_Events.key.keysym.sym == SDLK_RIGHT){
	                	if(SDL_GetModState() & KMOD_CTRL)
	                		TypeFeildLocator += 4;
	                	else if(SDL_GetModState() & KMOD_SHIFT){
	                		if(TypeFeildSelectStart==-1)
	                			TypeFeildSelectStart = TypeFeildLocator;
	                		TypeFeildLocator += 1;
	                	}
	                	else {
	                		TypeFeildLocator += 1;
	                		TypeFeildSelectStart = -1;
	                	}
	                	if(TypeFeildString[TypeFeildLocator]=='\n'){
	                	//	TypeFeildLocator += 1;
	                	}
	                }
	            }
	            KeenHandleKey(Keen_Events.key.keysym.sym);
				//std::cout << "Key Down:" << (Keen_Events.key.keysym.sym) << std::endl;
				break;
			case SDL_KEYUP:
				if(TypeFeildEnabled){
					// Handle Enter
	                if( Keen_Events.key.keysym.sym == SDLK_RETURN){
						if(EditingText){
						}else{
		                	TypeFeildEnabled = false;
		                }
	                }
				}
				//std::cout << "Key Up:" << (Keen_Events.key.keysym.sym) << std::endl;
				break;
			case SDL_TEXTINPUT:
				if(TypeFeildEnabled){
					//Not copy or pasting
                    if( !( SDL_GetModState() & KMOD_CTRL && ( Keen_Events.text.text[ 0 ] == 'c' || Keen_Events.text.text[ 0 ] == 'C' || Keen_Events.text.text[ 0 ] == 'v' || Keen_Events.text.text[ 0 ] == 'V' ) ) ){
                        //Append character
                        AppendString(Keen_Events.text.text);
                        TypeFeildLocator += 1;
                    }
				}
				break;
			case SDL_MOUSEMOTION:
				SDL_GetMouseState(&MouseX,&MouseY);
				MouseX /= 2; MouseY /= 2;
			break;
			case SDL_MOUSEBUTTONUP:
				if (Keen_Events.button.button == SDL_BUTTON_LEFT) {
					MouseButton = true;
				}
				break;
		}
		if(TypeFeildLocator<-1) TypeFeildLocator = -1;
		if(TypeFeildLocator>=(int)TypeFeildString.length()) TypeFeildLocator = TypeFeildString.length()-1;
	}
};
