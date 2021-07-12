CXX 		:=	clang++
NAME		:= 	webserv

SRCS		:=	main.cpp \
				srcs/middlewares/addHeaders.cpp srcs/middlewares/parseStartLine.cpp srcs/middlewares/sendResponse.cpp

OBJS		:=	$(SRCS:.cpp=.o)
RM			:=	rm -f
LIBRARY		?=	-I ./srcs/includes -I ./srcs/includes/parse -I ./srcs/log -I ./srcs/middlewares -I ./srcs/serve
CXXFLAGS	?=  -Wall -Werror -Wextra -std=c++98 $(LIBRARY)

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
	./$(NAME)
