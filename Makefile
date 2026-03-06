# Generated Makefile, Just a template. You can modify me

Var0 = clang++

.DEFAULT_GOAL := Link
run:
	
src/main.cpp.out: src/main.cpp
	@echo -e "[\e[1;32m Compiling file src/main.cpp 0.0%... \e[0m]"
	@ccache ${Var0} -c src/main.cpp -o build/main.cpp.out --std=gnu++23 -Iinclude
Link: src/main.cpp.out
	@echo -e "[\e[1;32m Linking 100%... \e[0m]"

	@clang++ build/main.cpp.out -o build/swww-setter -largumentsea
all:	run	src/main.cpp.out	Link
install:
	sudo mv build/swww-setter /usr/bin