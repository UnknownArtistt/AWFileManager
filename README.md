# AWFileManager

<img src="Banner/AsciArt.PNG">

AWFileManager is a terminal based file manager developed in C++. Is available in spanish and english. 

It can handle basic operations like:
- List files and directories
- Copy files
- Create and write files
- Add content to a file
- Delete a file

## Instalation

To use AWFileManager you only need to download the version you want and compile it. For that you only need to have a c/c++ compiler. 

In Windows you can get the GCC (MinGW) compiler and make sure you install the c++ compiler.

In Linux you should have already installed the gcc compiler. If you don´t have it you can do this way:

sudo apt update && sudo apt install build-essential

Once the compiler is fixed you can use this commands to get the program:
- Create/Go to the directory where you want to clone the repository
- Clone the repository -> git clone https://github.com/UnknownArtistt/AWFileManager or download the source code and extract it there
- Compile it -> g++ -o AWFileManager AWFileManager(en).cpp
- Run it -> Windows AWFileManager.exe / Linux ./AWFileManager

## How To Use

Once you have compiled and run the program select the option you want. You will always need to pass the absolute path of the file you want to manipulate. In the cases of writing content the program will let you write multiples lines, once you are done writing the content insert a "#" in a new line to finish the input.

IMPORTANT -> 
Pass always the absolute paths of the directories or files.
If you are running it on Linux take into account that Linux manages the paths in a different way: 
- "/" for Linux
- "\\" for Windows
