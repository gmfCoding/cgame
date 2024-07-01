# SOURCE FILES
SRCS = 	cl_strfmt.c			\
		cl_strtol.c			\
		cl_itoa.c			\
		cl_ftoa.c			\
		cl_strlcpy.c		\
		cl_strlcat.c		\
		cl_sescanf.c		\
		cl_sescanf_handle.c	\
		cl_strjoin.c		\
		cl_strdup.c 		\
		cl_split.c			\
		cl_ftoa.c	

# BONUS SOURCE FILES
SRCSB =	

# TARGET NAME AND VARIBLES
NAME = libcml.a
OBJDIR = obj/
CC = clang
FLAGS = $(DFLAGS) -Wall -Wextra -Werror -g

# OBJECT FILES
OBJ := $(patsubst %.c, $(OBJDIR)%.o, $(SRCS))
BONUS_OBJ := $(patsubst %.c, $(OBJDIR)%.o, $(SRCSB))

# ANY
all: $(NAME)

# CLEAN
re: fclean all

fclean: clean
	rm -f $(NAME)

clean:
	rm -rf $(OBJDIR)

# BUILD LIBRARY
$(NAME): $(OBJ)
	@ar rcs $(NAME) $(OBJ)
	@echo "ar rcs $(NAME) ..."
	@echo "Libcml libary generated!"

# COMPILE
$(OBJDIR)%.o : %.c libcml.h
	@mkdir -p $(OBJDIR)
	$(CC) $(FLAGS) -c $< -o $@

bonus: $(BONUS_OBJ)
	@ar r $(NAME) $(BONUS_OBJ)
	@echo "Libcml bonus content generated!"