FLG := -O3 -Wall -finput-charset=UTF-8 -fexec-charset=UTF-8 -std=c++17 -mwindows

LIB := -lsfml-window -lsfml-graphics -lsfml-system

all:
	g++ $(FLG) -o bezier.exe main.cpp $(LIB)
