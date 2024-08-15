# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sbelomet <sbelomet@42lausanne.ch>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/04 10:58:20 by sbelomet          #+#    #+#              #
#    Updated: 2024/08/14 10:27:36 by sbelomet         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# -fsanitize=address -g3

# Variables
NAME		=	webserv
CC			=	c++
INCLUDES	=	-I includes
CFLAGS		=	-Wall -Wextra -Werror -std=c++98
OBJ_DIR		=	objs/

# Colors
RED 		=	\033[1;91m
YELLOW		=	\033[1;93m
GREEN		=	\033[1;92m
BLUE		=	\033[1;94m
PURPLE		=	\033[1;95m
DEF_COLOR	=	\033[0;39m
GRAY		=	\033[0;90m
MAGENTA		=	\033[0;95m
CYAN		=	\033[1;96m
WHITE		=	\033[0;97m
RAINBOW		=	$(RED)-$(YELLOW)-$(GREEN)-$(CYAN)-$(BLUE)-$(PURPLE)-

# Directories
SRCS_DIR	=	srcs/

# Files
SRC			=	$(SRCS_DIR)main.cpp
OBJ			=	$(OBJ_DIR)main.o

# Commands
all:			$(OBJ_DIR) $(NAME)

$(NAME):		$(OBJ)
					@echo "$(RAINBOW)$(RAINBOW)$(RAINBOW)$(DEF_COLOR)"
					$(CC) $(CFLAGS) $(OBJ) -o $(NAME)
					@echo ""
					@echo "$(GREEN)$(NAME) ready!$(DEF_COLOR)🥶🥶🥶"
					@echo "$(RAINBOW)$(RAINBOW)$(RAINBOW)$(DEF_COLOR)"

$(OBJ_DIR)%.o:	$(SRCS_DIR)%.cpp
					@echo "$(YELLOW)$< added to the oven!$(DEF_COLOR)🥵🥵🥵"
					@$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

$(OBJ_DIR):
					@mkdir -p $(OBJ_DIR)

clean:
					@rm -rf $(OBJ_DIR)
					@echo "$(RAINBOW)$(RAINBOW)$(RAINBOW)$(DEF_COLOR)"
					@echo "$(BLUE)à la poubelle les .o$(DEF_COLOR)🤓🤓🤓"

fclean:			clean
					@rm -rf $(NAME)
					@echo "$(CYAN)à la poubelle l'exec$(DEF_COLOR)🤤🤤🤤"

re:				fclean all

run:			re
					valgrind ./$(NAME)

.PHONY:			all clean fclean re run

/////////////////////////////////////////

# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lgosselk <lgosselk@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/08/14 13:22:59 by lgosselk          #+#    #+#              #
#    Updated: 2024/08/15 09:40:30 by lgosselk         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	webserv

DEFAULT		=	\033[0m
RED			=	\033[1;31m
GREEN		=	\033[1;32m
YELLOW		=	\033[1;33m

CC			=	c++
RM			=	rm -rf
CFLAGS		=	-Wall -Werror -Wextra -std=c++98
HEADERS		=	-I ./includes

SRCS_DIR	=	srcs/
OBJS_DIR	=	objs/

FILES		=	main Config Section

SRCS		=	$(addprefix $(SRCS_DIR), $(addsuffix .cpp, $(FILES)))
OBJS		=	$(addprefix $(OBJS_DIR), $(addsuffix .o, $(FILES)))

all:	$(OBJS_DIR) $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo "$(GREEN)$(NAME) created!$(DEFAULT)"

$(OBJS_DIR)%.o: $(SRCS_DIR)%.cpp 
	@echo "$(YELLOW)$< en compilation!$(DEFAULT)"
	@$(CC) $(CFLAGS) $(HEADERS) -o $@ -c $<

$(OBJS_DIR):
		@mkdir -p $(OBJS_DIR)

clean:
	@$(RM) $(OBJS_DIR)
	@echo "$(YELLOW)object files deleted!$(DEFAULT)"

fclean:	clean
	@$(RM) $(NAME)
	@echo "$(RED)all deleted!$(DEFAULT)"

re:		fclean all

.PHONY:		all clean fclean re
