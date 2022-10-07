a.out: main.c obj.h pointers.h
	gcc main.c -lglfw -lGL -lGLEW -lm
debug: main.c obj.h pointers.h
	gcc -g main.c -lglfw -lGL -lGLEW -lm
