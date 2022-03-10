CXX 		:=	clang++
NAME		:= 	webserv

SRCS		:=	main.cpp \
				srcs/middlewares/read.cpp srcs/middlewares/write.cpp

OBJS		:=	$(SRCS:.cpp=.o)
RM			:=	rm -f
LIBRARY		?=	-I ./srcs/cgi -I ./srcs/includes -I ./srcs/includes/parse -I ./srcs/log \
				-I ./srcs/middlewares -I ./srcs/serve  -I ./srcs/static
CXXFLAGS	?=  -Wall -Werror -Wextra -g -std=c++98 $(LIBRARY)

$(NAME): $(OBJS) $(SUBDIRS)
	$(CXX) $(LIBRARY) -o $(NAME) $(OBJS)

all: $(NAME)

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
