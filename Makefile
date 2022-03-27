NAME = 				webserv

SRCS = 				main.cpp Program.cpp Loc.cpp Helper.cpp Server.cpp Config.cpp Cgi.cpp Client.cpp Req.cpp Res.cpp 
PATH_SRCS = 		cpp/

PATH_HEAD =			hpp/

PATH_OBJS = 		obj/
OBJS =				$(SRCS:%.cpp=$(PATH_OBJS)%.o)
OBJS_D =			$(SRCS:%.cpp=$(PATH_OBJS)%.d)

CPP =				clang++
FLAGS =				-Wall -Wextra -Werror -std=c++98


all:				$(NAME)

$(NAME):			$(OBJS)
					@$(CPP) $(OBJS) -o $(NAME)
					@printf "Webserver is ready! 📬\n"

$(PATH_OBJS)%.o:	$(PATH_SRCS)%.cpp
					@$(CPP) -I $(PATH_HEAD) -c $< -o $@ -MD


$(OBJS):			| $(PATH_OBJS)

$(PATH_OBJS):
					@mkdir $(PATH_OBJS)

include $(wildcard $(OBJS_D))

clean:
					@rm -rf $(PATH_OBJS)

fclean:				clean
					@rm -rf $(NAME)

re:					fclean all

.PHONY:				all clean fclean re

