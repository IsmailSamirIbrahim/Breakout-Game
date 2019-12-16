# Breakout-Game
This is my first 2D-Game using OpenGl/C++ 

## Build Guide

### Windows
- make sure you have a C++17 compiler (recommended vs2017)
- make sure you have CMake 3.9+ (recommended 3.14)
- clone repo recursively `git clone https://github.com/IsmailSamir/Breakout-Game.git --recursive`
- at this point you may have 3 options
	1. configure cmake (`cmake . -G "Visual Studio 15 Win64" -B build`) then open the solution
	2. open the cmake file using Visual Studio Directly
	3. open the cmake file using QtCreator which will give you all the Qt Goodies right out of the box

### Linux
- Install some dev dependencies (ubuntu): `sudo apt-get install xorg-dev libglu1-mesa-dev libglew-dev`
- clone the repo: `git clone https://github.com/IsmailSamir/Breakout-Game.git --recursive`
- `cd Breakout-Game`
- `mkdir build`
- `cd build`
- `cmake ..`
- `make`
