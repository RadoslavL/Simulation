a.out: main.c obj.h
	gcc main.c -lglfw -lGL -lGLEW -lm
debug: main.c obj.h
	gcc -g main.c -lglfw -lGL -lGLEW -lm
