# IMAGEN

## Installation

``` sh
git submodule update —init  —recursive
```

### Mac OS 

- Install XCode
- Install CMake

For CMake, if you have homebrew setup, you can just run `brew install cmake`. If you don't have homebrew setup, you can install it from here: https://cmake.org/install/

### Windows

- Install Visual Studio (not Visual Studio Code) from here: https://visualstudio.microsoft.com
- Install CMake from here: https://cmake.org/install/

### Linux

- Install development tools like `make`, `g++`, etc... (often installable under the `build-essential` package). For example, on Ubuntu, run `sudo apt install build-essential`.
- Install CMake. It is under the package `cmake`. For example, on Ubuntu, run `sudo apt install cmake`.

## Setup/Compile/Run/Development

### Mac OS 

To setup the project, run the following commands (assuming that your current working directory starts from the root of the project: `~/This/That/Other/imagen/`):

``` sh
mkdir build
cd build 
cmake -DCMAKE_BUILD_TYPE=DEBUG ..
make
```

To build/compile the project after any changes, `cd` into the build folder and run `make`

To run the code for a particular lesson (for instance, lesson2), `cd` into the build folder and run `./lesson2`.

To edit the code for a lesson, just open the `.cpp` file contained in the lesson's directory with your favorite text editor and code away! You can then compile and run.

### Windows

To setup the project, run the following command (assuming that your current working directory starts from the root of the project: `~/This/That/Other/imagen/`):

``` sh
mkdir build
cd build 
cmake -DCMAKE_BUILD_TYPE=DEBUG ..
```

Inside the build folder, you will find `imagen.sln` file. Double click on it in your file explorer.

On the `Solution Explorer` which is a window on either the left or the right of the Visual Studio application. If you uncollapse the `imagen` solution (might already be uncollapsed), right click on the lesson you want to compile or run. Select `Set as Startup Project`. Then press `Control`, `Shift` and `B` at the same time to compile the lesson. To run the lesson, press `Control` and `F5` at the same time. You should see a window appear with whatever cool graphics you are drawing into it!

to edit the code for a lesson, uncollapse the lesson you want to modify, double click on the `.cpp` file for the lesson and code away! Then to compile it's once again `Control`, `Shift` and `B`. To run it's also once again `Control` and `F5`.

### Linux (Same as MacOS)

To setup the project, run the following commands (assuming that your current working directory starts from the root of the project: `~/This/That/Other/imagen/`):

``` sh
mkdir build
cd build 
cmake -DCMAKE_BUILD_TYPE=DEBUG ..
make
```

To build/compile the project after any changes, `cd` into the build folder and run `make`

To run the code for a particular lesson (for instance, lesson2), `cd` into the build folder and run `./lesson2`.

To edit the code for a lesson, just open the `.cpp` file contained in the lesson's directory with your favorite text editor and code away! You can then compile and run.
