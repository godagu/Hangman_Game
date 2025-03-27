
# Hangman Game

## Description
This is a classic Hangman game implemented using SDL2 done in my 1st year of Computer Science studies for the Fundamentals of Programming class. The player must guess the hidden word by typing letters. Each incorrect guess results in the hangman image being drawn step by step. The game keeps track of the highest score, allows saving and loading progress, and includes a leaderboard of the best-played games.

## Features
- Interactive word guessing gameplay
- Hangman image rendered progressively with incorrect guesses
- High score tracking (guessed words in a row)
- Save and load game functionality
- Leaderboard of top scores

## Compilation Instructions

### Windows
To compile on Windows, use the following command:

```sh
gcc -o hangman.exe -Iinclude -Llib -Isdl2_ttf\font\include -Lsdl2_ttf\font\includeIsdl2_ttf\font\lib *.c -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf
```


Ensure you have SDL2 and SDL2_ttf libraries installed and correctly referenced.

### macOS
Before compiling on macOS, change the include paths in the source code to macOS-compatible formats. Then, use:

```sh
gcc -o hangman *.c -lSDL2main -lSDL2 -lSDL2_ttf 
```

Download and install SDL2 and SDL2_ttf for macOS.

### Ubuntu (Tested Platform)
To install dependencies:

```sh
sudo apt-get install libsdl2-ttf-dev libsdl2-dev libsdl2-ttf-2.0-0
```

To compile and clean:

```sh
make     # Compiles the game
make clean   # Cleans build files
```

## Installation Notes
- For macOS and Windows, download the SDL2 and SDL2_ttf libraries and adjust the include paths in the source code accordingly.
- On Linux, using the package manager is recommended for easier installation of dependencies.

## Running the Game
After compilation, execute the generated binary:

```sh
./hangman   # Linux/macOS
hangman.exe   # Windows
```

## Author 
Goda Gutparakytė

Enjoy playing Hangman!
