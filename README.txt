To compile on Windows use this command:

gcc -o hangman.exe -Iinclude -Llib -Isdl2_ttf\font\include -Lsdl2_ttf\font\includeIsdl2_ttf\font\lib *.c -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf

To compile on macOS use this command:

Change paths in the source code to macOS type and run this command

gcc -o hangman *.c -lSDL2main -lSDL2 -lSDL2_ttf 


// INSTALIUOT ANT UBUNTU (KUR IR TESTUOTA) libsdl2-ttf-dev libsdl2-dev libsdl2-ttf-2.0-0 (sudo apt-get) to compile and clean make or make clean 
// for mac and windows (good luck) download sdl2 lib for mac/windows, change the include path in the make file (for mac specifically change paths in the header files)