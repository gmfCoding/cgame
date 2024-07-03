SRCSF = $(TEST) \
		texture/texture.c \
		texture/pixel.c \
		texture/texture_util.c \
		texture/circle.c \
		vector/vector2_math_extra.c \
		vector/vector2_math_div.c \
		vector/vector2_math.c \
		vector/vector2_angle.c \
		vector/vector2_dist.c \
		vector/vector2_lerp.c \
		vector/vector2.c \
		vector/vector2i_math_extra.c \
		vector/vector2i_math.c \
		vector/vector2i.c \
		vector/vector3_math_extra.c \
		vector/vector3_math.c \
		vector/vector3_math_div.c \
		vector/vector3.c \
		vector/vector4_math.c \
		vector/vtoa.c \
		vector/vector2_conv.c \
		vector/vector3_conv.c \
		vector/rect.c \
		util/time.c \
		util/math.c \
		util/bounds.c \
		util/random.c \
		input/input.c \
		input/input_hooks.c \
		input/keys.c \
		input/mouse.c \
		gpu/shader.c \
		gpu/gpu_mesh.c \
		core/material.c \
		core/entity.c \
		core/renderer.c \
		core/material_system.c \
		model.c \
		containers.c \
		system/io.c \
		asset.c

ifndef $(TEST)
TEST=main.c
endif

ifndef $(OS)
OS := $(shell uname)
endif

$(info Compiling for OS:$(OS))

NAME = app

DIRSRC = src
DIROBJ = obj/$(CONF)
DIRINC = inc
DIRLIB = lib

CMLLIB = libcml/libcml.a
STBLIB = stb_image/libstb.a
GLFWLIB = glfw/libglfw3.a
GLFWDIR = $(DIRLIB)/$(dir $(GLFWLIB))

GLADLIB = glad/libglad.a

STCINC = -I$(DIRLIB)/STC/include -I$(DIRLIB)/cglm/include

LIBSF = $(CMLLIB) $(STBLIB) $(GLADLIB) $(GLFWLIB)

# All relative to Makefile's folder
SRCS = $(patsubst %.c,$(DIRSRC)/%.c, $(SRCSF))
OBJS = $(SRCS:$(DIRSRC)/%.c=$(DIROBJ)/%.o)
LIBS = $(patsubst %.a,$(DIRLIB)/%.a, $(LIBSF)) 
INCS = $(patsubst %.h,$(DIRINC)/%.h, $(INCSF))
DEPS = $(OBJS:.o=.d)

LIB-I = $(patsubst %,-I%,$(dir $(LIBS))) -I$(DIRLIB) $(STCINC)
LIB-l = $(subst lib,-l,$(basename $(notdir $(LIBSF))))
LIB-L = $(patsubst %,-L$(DIRLIB)/%, $(dir $(LIBSF)))

CC = cc

WFLAGS =-Wall -Werror -Wextra
CPPFLAGS =-I$(DIRINC) $(LIB-I) -MMD -MP
CFLAGS = $(OPFLAG) $(DFLAGS) $(XCFLAGS) $(WFLAGS)
LDFLAGS = $(OPFLAG) $(DFLAGS) $(XLDFLAGS) $(LIB-L) $(LIB-l) -lz -lm -lpthread -ldl
OPFLAG = #-O3 -flto -march=native -mtune=native -msse4.2 

OPTS = $(OPT)
SAN = address 

CONF = release
CONF_TARGET = .target

ifneq (,$(findstring debug,$(CONF)))
OPTS = fsan,debug
endif

ifneq (,$(findstring dsym,$(CONF)))
OPTS = debug
endif

ifneq (,$(findstring none,$(OPTS)))
OPFLAG = -O0
endif
ifneq (,$(findstring debug,$(OPTS)))
	OPFLAG = -O0
	DFLAGS += -gdwarf-4
endif
ifneq (,$(findstring fdeb,$(OPTS)))
	OPFLAG = -O1 -march=native
	DFLAGS += -gdwarf-4
endif
ifneq (,$(findstring fsan,$(OPTS)))
# -fno-sanitize-ignorelist -fsanitize-ignorelist=ignorelist.txt
# Compile with selected sanitizer:
# And when using other sanitizers such as memory or undefined, it may be useful to not prematurely stop,
# Use UBSAN_OPTIONS=halt_on_error=0 (need -fs..-recover=..) or equivelent
# Also might be nice to redirect stderr to a file
# USE 
	DFLAGS += -fsanitize=$(SAN) -fsanitize-recover=$(SAN) 
endif
ifneq (,$(findstring gmon,$(OPTS)))
	PGFLAGS += -pg
endif

ifeq ($(EXTRA),1)
CPPFLAGS += -D EXTRA
endif

ifneq ($(OS),Linux) 
LDFLAGS += -framework OpenGL -framework AppKit 
else 
LDFLAGS += -lX11 -lXext -lGL
endif

# RULES
all: $(NAME)

# marker for the last built architecture
BUILT_MARKER := $(CONF).built

$(BUILT_MARKER):
	@-rm -f *.built
	@touch $(BUILT_MARKER)

# OBJ TO PROJECT
$(NAME): $(LIBS) $(OBJS) $(BUILT_MARKER)
	-@printf "${BLUE}"
	$(CC) $(PGFLAGS) $(OBJS) $(LDFLAGS) -o $@
	-@printf "${NC}"

# SOURCE TO OBJ
$(OBJS): $(DIROBJ)%.o : $(DIRSRC)%.c $(INCS) | $(DIROBJ)
	-@mkdir -p $(dir $@)
	-@printf "${GREEN}"
	-$(CC) $(PGFLAGS) $(CPPFLAGS) $(CFLAGS) -o $@ -c $<
	-@printf "${NC}"

# TODO: Merge with fclean before submitting
lclean: clean
	-make -C $(dir $(DIRLIB)/$(CMLLIB)) fclean

# CLEANING
fclean: clean
	-@printf "${BRED}Cleaning executable!\n${RED}"
	-rm -f $(NAME)
	-@printf "${NC}"

clean:
	-@printf "${BYELLOW}Cleaning objects!\n${RED}"
	-rm -rf $(DIROBJ)
	-@printf "${NC}"

re: fclean lclean all

# Dependencies
-include $(DEPS)

$(DIRLIB)/$(CMLLIB):
	make -s -C $(dir $@) all DFLAGS="$(DFLAGS)"

$(DIRLIB)/$(STBLIB):
	make -s -C $(dir $@) all DFLAGS="$(DFLAGS)"

$(DIRLIB)/$(GLFWLIB):
	mkdir -p $(GLFWDIR)build
	cmake -S $(GLFWDIR) -B $(GLFWDIR)build
	make -C $(GLFWDIR)build 
	cp $(GLFWDIR)build/src/libglfw3.a $(GLFWDIR)libglfw3.a

$(DIRLIB)/$(GLADLIB):
	make -s -C $(dir $@) all DFLAGS="$(DFLAGS)"

# Folders
$(DIROBJ):
	-@printf "${CYAN}"
	-mkdir -p $(DIROBJ)
	-@printf "${NC}"

.PHONY: all re fclean clean lclean

# COLORS
export BGREEN = \033[1;32m
export GREEN = \033[0;32m
export BRED = \033[1;31m
export RED = \033[0;31m
export BYELLOW = \033[1;33m
export YELLOW = \033[0;33m
export BBLUE = \033[1;34m
export BLUE = \033[0;34m
export BCYAN = \033[1;36m
export CYAN = \033[0;36m
export NC = \033[0m
