sfml_path = /usr/local/lib/SFML-2.6.1

target_name = sfml-app

main = src/main.cpp
files = src/glad.c $(wildcard src/graphics/*.cpp)

compile:src/main.cpp
	g++ ${main} ${files} -o ${target_name} -I./include -I${sfml_path}/include -L${sfml_path}/lib -lsfml-graphics -lsfml-window -lsfml-system
run:
	export LD_LIBRARY_PATH=${sfml_path}//lib && ./${target_name}