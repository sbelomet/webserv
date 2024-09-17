# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sbelomet <sbelomet@42lausanne.ch>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/04 10:58:20 by sbelomet          #+#    #+#              #
#    Updated: 2024/09/17 14:26:48 by sbelomet         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# -fsanitize=address -g3

# Variables
NAME		=	webserv
CC			=	c++
INCLUDES	=	-I includes
CFLAGS		=	-Wall -Wextra -Werror -std=c++98 -g3
OBJS_DIR	=	objs/
SRCS_DIR	=	srcs/

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

# Files
FILES		=	main Config MapConfig utils Server Location Webserv Manager \
				httpRequest httpResponse httpHeader CGI Mime

SRCS		=	$(addprefix $(SRCS_DIR), $(addsuffix .cpp, $(FILES)))
OBJS		=	$(addprefix $(OBJS_DIR), $(addsuffix .o, $(FILES)))

# Commands
all:			$(OBJS_DIR) $(NAME)

$(NAME):		$(OBJS)
					@echo "$(RAINBOW)$(RAINBOW)$(RAINBOW)$(DEF_COLOR)"
					$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
					@echo ""
					@echo "$(GREEN)$(NAME) ready!$(DEF_COLOR)🥶🥶🥶"
					@echo "$(RAINBOW)$(RAINBOW)$(RAINBOW)$(DEF_COLOR)"

$(OBJS_DIR)%.o:	$(SRCS_DIR)%.cpp
					@echo "$(YELLOW)$< added to the oven!$(DEF_COLOR)🥵🥵🥵"
					@$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

$(OBJS_DIR):
					@mkdir -p $(OBJS_DIR)

clean:
					@rm -rf $(OBJS_DIR)
					@echo "$(RAINBOW)$(RAINBOW)$(RAINBOW)$(DEF_COLOR)"
					@echo "$(BLUE)à la poubelle les .o$(DEF_COLOR)🤓🤓🤓"

fclean:			clean
					@rm -rf $(NAME)
					@echo "$(CYAN)à la poubelle l'exec$(DEF_COLOR)🤤🤤🤤"

re:				fclean all

run:			re
					valgrind ./$(NAME)

.PHONY:			all clean fclean re run
