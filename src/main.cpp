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

#include "heads.h"

bool KeenTextExtended = false;
std::string KeenTextUseFile = "";
std::string KeenTextBmpPref = "CKD_pic_";
extern std::string FloppyLoadStr;

bool EditorIsRunning = true;

int main(int argc,char *argv[]){
	std::vector<std::string> args;
	for(int i = 0; i < argc; i ++){
		args.push_back(argv[i]);
	}

	// Parse the args

	for(int argCount = 0; argCount < (int)args.size(); argCount ++){
		std::string &arg = args[argCount];
		if(arg[0]=='-'){
			bool BadArgument = true;
			if(arg.compare("-bmp")==0){
				// Get the next arg
				if(++argCount < (int)args.size())
					KeenTextBmpPref = args[argCount];
				BadArgument = false;
			}
			if(arg.compare("-file")==0){
				// Get the next arg
				if(++argCount < (int)args.size())
					KeenTextUseFile = args[argCount];
				BadArgument = false;
				FloppyLoadStr = KeenTextUseFile;
			}
			if(arg.compare("-extended")==0){
				// Set the bool
				KeenTextExtended = true;
				BadArgument = false;
			}
			if(arg.compare("-h")==0 ||
				arg.compare("-help")==0||
				arg.compare("--h")==0||
				arg.compare("--help")==0||
				BadArgument){
				// Print the help
				std::cout << "Keen Text Generator v1.0" << std::endl;
				std::cout << "Written by John314" << std::endl << std::endl;
				std::cout << "KeenTxtEd [OPTIONS]" << std::endl << std::endl;
				std::cout << "Options : " << std::endl;
				std::cout << "      -h -help     : Print this help" << std::endl;
				std::cout << "      -file [name] : Use a specific file to edit" << std::endl;
				std::cout << "      -bmp [name]  : The filename to use for BMPs [ex. CKD_pic_ ]" << std::endl;
				std::cout << "      -extended    : Allow for Atroxian Realms text additions" << std::endl;
				return 0;
			}
		}
	}

	if(KeenTextUseFile.length())
		LoadKeenText(KeenTextUseFile);

//	CoutKeenText();

	// Set up the window
	if(SetupSDLVideo()!=0){
		ShutdownSDLVideo();
		return 0;
	}
	std::cout << "Loading Graphics" << std::endl;
	KeenTextEditor_LoadGfx();

	while(EditorIsRunning){
		UpdateSDL();
		
		if(KeenTextEditorLoop()!=0){
			EditorIsRunning = false;
		}

		RenderSDL();
	}

	KeenTextEditor_UnloadGfx();

	ShutdownSDLVideo();

	return 0;
};
