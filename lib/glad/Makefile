CC = cc
SRC = glad.c

all: libstb.a

libstb.a: glad.o
	@ar rcs libglad.a glad.o

clean:
	rm glad.o

fclean:
	rm libglad.a

.PHONY: all clean fclean 