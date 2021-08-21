
SRC_FILES = src/main.cpp \
			src/SDL_Stuff.cpp \
			src/Keen_Text.cpp \
			src/Editor.cpp

CXX = g++

CXXFLAGS = -O2 -Wall -std=c++17 

SRCFLAGS = -lSDL2 
#-lSDL2_ttf

MINGFLAGS = -L/opt/local/i686-w64-mingw32/lib -lmingw32 -lSDL2main -lSDL2 -mwindows -Wl,--dynamicbase -Wl,--nxcompat -lm -ldinput8 -ldxguid -ldxerr8 -luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lshell32 -lsetupapi -lversion -luuid

all: debug release win_release

debug:
	$(CXX) $(SRC_FILES) $(CXXFLAGS) $(SRCFLAGS) -DLINUX=0 -g -o "bin/KeenTextEd_Debug"
release:
	$(CXX) $(SRC_FILES) $(CXXFLAGS) $(SRCFLAGS) -DLINUX=0 -o "bin/KeenTextEd_Release"
win_release:
	x86_64-w64-mingw32-g++ -DSDL_MAIN_HANDLED=0 $(SRC_FILES) -static -static-libgcc -static-libstdc++ \
	-I/home/jonathan/Programing/mingwSDL2/x64/include -L/home/jonathan/Programing/mingwSDL2/x64/lib $(CXXFLAGS) $(MINGFLAGS) -o "bin/KeenTextEd_Win64x.exe"
