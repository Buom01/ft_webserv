CXX 		:=	clang++
NAME		:= 	webserv
RM			:=	rm -f

SRCS		?=	main.cpp
OBJS		:=	$(SRCS:.cpp=.o)
LIBRARY		?=	-I ./srcs/help -I ./srcs/includes -I ./srcs/includes/parse \
				-I ./srcs/log -I ./srcs/middlewares -I ./srcs/serve \
				-I ./srcs/static
CXXFLAGS	?=  -Wall -Werror -Wextra -std=c++98 -g $(LIBRARY)

$(NAME): $(OBJS) $(SUBDIRS)
	$(CXX) $(LIBRARY) -o $(NAME) $(OBJS)

all: help $(NAME)

help:
	xxd -i ./srcs/help/documentation > srcs/help/generateDocumentation.hpp

fclean:
	$(RM) $(OBJS)
	$(RM) $(NAME)

clean:
	$(RM) $(OBJS)

re: fclean
	$(MAKE) all

test: fclean
	$(MAKE) all
	./$(NAME) website_test/init.conf
