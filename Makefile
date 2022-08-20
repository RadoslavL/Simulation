a.out: main.c obj.h
	gcc main.c -lglfw -lGL -lGLEW -lm
