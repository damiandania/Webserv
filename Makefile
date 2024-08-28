NAME		=	Webserv

#FLAGS__________________________________________________________________________

CC			=	c++
CFLAGS		=	-Werror -Wextra -Wall -g -std=c++98

OBJ_PATH	=	./objects/
INC_PATH	=	./includes/

#SOURCES________________________________________________________________________

SRC_PATH	=	./sources/
SRC			=	main.cpp					\
				Server.cpp					\
				Socket.cpp					\
				ConfigFile.cpp				\
				Configuration.cpp			\
				ServerData.cpp				\
				Response.cpp				\
				Request.cpp					\
				RouteData.cpp				\
				CGI.cpp						\
				HtmlMaker.cpp				


SRCS		=	$(addprefix $(SRC_PATH), $(SRC))

OBJ			=	$(SRC:.cpp=.o)
OBJS		=	$(addprefix $(OBJ_PATH), $(OBJ))
INC			=	-I $(INC_PATH)

#COLORS_________________________________________________________________________

GREEN		=	\033[0;32m
RED			=	\033[0;31m
WHITE		=	\033[0;97m
RESET		=	\033[0m
YELLOW		=	\033[0;93m

#MAIN_RULE______________________________________________________________________

all: $(OBJ_PATH) $(NAME)

$(NAME): $(OBJS)
	@echo -n "$(YELLOW)Compiling	${WHITE}→	$(YELLOW)$(NAME) with c++98$(RESET) "
	@$(CC) $(CFLAGS) $(OBJS) -o $@ $(INC)
	@echo "$(GREEN)[ok]✓$(RESET)"

#OBJECT_RULE____________________________________________________________________

$(OBJ_PATH):
	@mkdir -p $(OBJ_PATH)

$(OBJ_PATH)%.o: $(SRC_PATH)%.cpp
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@ $(INC)

#CLEAN_RULE_____________________________________________________________________

clean:
	@echo -n "$(RED)Deleting	${WHITE}→	$(RED)$(NAME) objects$(RESET) "
	@rm -rf $(OBJ_PATH)
	@echo "$(GREEN)[ok]$(RED)✗$(RESET)"

fclean: clean
	@echo -n "$(RED)Deleting	${WHITE}→	$(RED)$(NAME) executable$(RESET) "
	@rm -f $(NAME)
	@echo "$(GREEN)[ok]$(RED)✗$(RESET)"

re: fclean all

#SPEC___________________________________________________________________________

.PHONY: all re clean fclean
