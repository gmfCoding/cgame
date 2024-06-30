CC = cc
SRC = stb_image_impl.c

all: libstb.a

libstb.a: stb_image_impl.o
	@ar rcs libstb.a stb_image_impl.o

clean:
	rm stb_image_impl.o

fclean:
	rm libstb.a

.PHONY: all clean fclean 