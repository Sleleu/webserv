# VARIABLES

NAME		= webserv
NAME_BONUS	= 

SRC_DIR		= src/
OBJ_DIR		= obj/
SUBDIR		= obj/parser \
			  obj/server \
			  obj/response
BONUS_DIR	= src_bonus/

ADDFLAGS	=

# SOURCES

SRC_FILES 		= server/Server \
				  server/Server_utils \
				  server/display \
				  server/handler \
				  parser/main \
				  parser/parser \
				  response/handle_response \
				  response/parsing_request \
				  response/methods \
				  response/HttpResponse \
				  response/HttpRequest \
				  response/CgiHandler \
				  response/directory_index \

SRC_FILES_BONUS	=

# SOURCES SET UP

C_FILES		=	$(addsuffix .cpp, $(SRC_FILES))
SRCS		=	$(addprefix $(SRC_DIR), $(C_FILES))
OBJS		=	$(addprefix $(OBJ_DIR), $(C_FILES:.cpp=.o))
DEPS		=	$(OBJS:.o=.d)

C_BONUS			=	$(addsuffix _bonus.cpp, $(SRC_FILES_BONUS))
SRCS_BONUS		=	$(addprefix $(BONUS_DIR), $(C_BONUS))
OBJS_BONUS		=	$(addprefix $(OBJ_DIR), $(C_BONUS:.cpp=.o))
DEPS_BONUS		=	$(OBJS_BONUS:.o=.d)

# FLAGS & UTILS
CC			= c++
CFLAGS		= -Wall -Werror -Wextra -std=c++98 -D_GLIBCXX_DEBUG -g -I./srcs/includes -MMD -MP
RM			= rm -rf
AR			= ar rcs
SAN			= -fsanitize=address

#	LOAD BAR

COUNT	:= $(words $(SRC_FILES))
LOAD	= 0
DIVIDE	= 0
SPACE = $(COUNT)

#	 MANDATORY
all:		 obj $(NAME)


$(NAME):	$(OBJS)
			@$(CC) $(CFLAGS) $^ -o $@ $(ADDFLAGS)
			@echo "\e[1A\e[K$(FONT_BOLD)FILES LOADED ! $(FONT_RESET)    $(COUNT)/($(COUNT))"
			@echo " "
			@echo "$(RED)$(NAME) compiled !$(DEF_COLOR)"

$(OBJ_DIR)%.o:	 $(SRC_DIR)%.cpp
			@$(CC) $(CFLAGS) $(ADDFLAGS) -c -o $@ $<
			@$(eval LOAD=$(shell echo $$(($(LOAD)+1))))
			@$(eval SPACE=$(shell echo $$(($(SPACE)-1))))
			@echo "\e[2A\e[K$(BLUE)Creating object file -> $(WHITE)$(notdir $@)... $(RED)[Done]$(NOC)"
			@/bin/echo -n "$(FONT_BOLD)Load Files |$(FONT_RESET)"
			@load=$(LOAD) ; while [ $${load} -gt 0 ] ; do\
				/bin/echo -n "=" ;\
				load=`expr $$load - 1`; \
			done; \
			true
			@space=$(SPACE) ; while [ $${space} -gt 0 ] ; do\
				/bin/echo -n " " ;\
				space=`expr $$space - 1`; \
			done; \
			true
			@/bin/echo -n "$(FONT_BOLD)|$(FONT_RESET)"
			@echo "    $(RED)$(LOAD)/($(COUNT))"

#	BONUS
bonus:		$(LIB) $(NAME_BONUS)

$(NAME_BONUS): $(OBJS_BONUS)
			@$(CC) $(CFLAGS) $(ADDFLAGS) $^ -o $(NAME_BONUS) $(LIB)
			@echo "$(RED)$(NAME_BONUS) BONUS compiled !$(CYAN)"

$(OBJ_DIR)%.o:	 $(BONUS_DIR)%.cpp
			@mkdir -p $(OBJ_DIR)
			@$(CC) $(CFLAGS) -c -o $@ $<
			@echo "$(BLUE)Creating object file -> $(WHITE)$(notdir $@)... $(RED)[Done]$(NOC)"

#	 RULES
obj:
			@mkdir -p $(OBJ_DIR)
			@mkdir -p $(SUBDIR)

clean:
			@$(RM) $(OBJ_DIR) $(DEPS_DIR)
			@echo "$(BLUE)$(NAME) object files cleaned!$(DEF_COLOR)"

fclean:		clean
			@$(RM) -f $(NAME)
			@$(RM) -f $(NAME_BONUS)

re:			fclean
			@make --no-print-directory all
			@echo "$(GREEN)Cleaned and rebuilt everything for $(NAME)!$(DEF_COLOR)"

.PHONY: all clean fclean re

.SILENT: lib

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
FONT_BOLD := $(shell tput bold)
FONT_RED := $(shell tput setaf 1)
FONT_RESET := $(shell tput sgr0)
FONT_CYAN := $(shell tput setaf 6)
