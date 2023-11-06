all: 
scenes:
	g++ main.cpp scripts/SDLHelper.cpp scripts/SDLHelper.hpp scripts/SDLHelper.cpp scripts/Scene.hpp scripts/Scene.cpp scripts/Button.hpp scripts/Button.cpp -I src/include -L src/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -o Splendor