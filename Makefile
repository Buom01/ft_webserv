CXX 		:=	clang++
NAME		:= 	webserv
RM			:=	rm -f

SRCS		:=	srcs/lib/split.cpp srcs/lib/gnl.cpp srcs/lib/file.cpp \
				srcs/lib/utils.cpp \
				\
				srcs/middlewares/body.cpp srcs/middlewares/eject.cpp \
				srcs/middlewares/error.cpp srcs/middlewares/forbidden.cpp \
				srcs/middlewares/graceful_shutdown.cpp \
				srcs/middlewares/mimetypes.cpp srcs/middlewares/read.cpp \
				srcs/middlewares/redirect.cpp srcs/middlewares/remover.cpp \
				srcs/middlewares/upload.cpp srcs/middlewares/write_body.cpp \
				srcs/middlewares/write_headers.cpp \
				\
				srcs/serve/Chain.cpp srcs/serve/Serve.cpp \
				\
				srcs/static/Static.cpp \
				\
				main.cpp

LIBRARY		?=	-I ./srcs/components \
				-I ./srcs/help \
				-I ./srcs/includes \
				-I ./srcs/lib/includes \
				-I ./srcs/middlewares/includes \
				-I ./srcs/serve/includes \
				-I ./srcs/static/includes

OBJS		:=	$(SRCS:.cpp=.o)
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
