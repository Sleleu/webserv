# VARIABLES

NAME		= http
CC			= c++
CFLAGS		= -Wall -Werror -Wextra -MMD -MP -std=c++98 -Wshadow

CLASS		= class/class.a
RM			= rm -rf
AR			= ar rcs
SAN			= -fsanitize=address -g

# SOURCES

SRC_FILES	=	handle_request \
				methods \
				_SERVERTEST \

SRCS		=	$(addsuffix .cpp, $(SRC_FILES))
OBJS 		=	$(SRCS:.cpp=.o)
DEPS		=	$(OBJS:.o=.d)


# 	MANDATORY
all: 		$(NAME)

$(NAME):	$(OBJS)
			$(CC) $^ -o $@ $(CFLAGS)
			@echo "$(GREEN)$(NAME) compiled !$(DEF_COLOR)"

%.o:		%.cpp
			$(CC) $(CFLAGS) -c -o $@ $<
			@echo "$(BLUE)Creating object file -> $(WHITE)$(notdir $@)... $(RED)[Done]$(NOC)"

$(CLASS):
			@make -C ./class

# 	RULES

clean:
			@$(RM) $(OBJS)
			@$(RM) $(DEPS)
			@echo "$(BLUE)$(NAME) object files cleaned!$(DEF_COLOR)"

fclean:
			@make clean
			@$(RM) $(NAME)

re:
			@make fclean
			@make all
			@echo "$(GREEN)Cleaned and rebuilt everything for $(NAME)!$(DEF_COLOR)"

.PHONY: all clean fclean re

-include $(DEPS)

# COLORS

NOC = \033[0m
BOLD = \033[1m
UNDERLINE = \033[4m
DEF_COLOR = \033[0;39m
GRAY = \033[0;90m
RED = \033[0;91m
GREEN = \033[0;92m
YELLOW = \033[0;93m
BLUE = \033[0;94m
MAGENTA = \033[0;95m
CYAN = \033[0;96m
WHITE = \033[0;97m
