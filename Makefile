CXX 		:=	clang++
NAME		:= 	webserv
RM			:=	rm -f

SRCS		:=	srcs/main.cpp \
				\
				srcs/components/components.cpp \
				\
				srcs/lib/split.cpp srcs/lib/gnl.cpp srcs/lib/file.cpp \
				srcs/lib/utils.cpp srcs/lib/argv.cpp \
				\
				srcs/middlewares/body.cpp srcs/middlewares/Cgi.cpp \
				srcs/middlewares/eject.cpp srcs/middlewares/error.cpp \
				srcs/middlewares/forbidden.cpp srcs/middlewares/keep_alive.cpp \
				srcs/middlewares/log_request.cpp \
				srcs/middlewares/mimetypes.cpp srcs/middlewares/read.cpp \
				srcs/middlewares/redirect.cpp srcs/middlewares/remover.cpp \
				srcs/middlewares/upload.cpp srcs/middlewares/write_body.cpp \
				srcs/middlewares/write_headers.cpp \
				\
				srcs/serve/Definition.cpp srcs/serve/Chain.cpp \
				srcs/serve/Serve.cpp \
				\
				srcs/static/Static.cpp

LIBRARY		?=	-I ./srcs/components/includes \
				-I ./srcs/help \
				-I ./srcs/includes \
				-I ./srcs/lib/includes \
				-I ./srcs/middlewares/includes \
				-I ./srcs/serve/includes \
				-I ./srcs/static/includes \
				-I ./srcs

OBJS		:=	$(SRCS:.cpp=.o)
CXXFLAGS	?=  -Wall -Werror -Wextra -std=c++98 $(LIBRARY)

# Colors
RED			:= \e[0;91m
GREEN		:= \e[0;92m
BLUE		:= \e[0;94m
MAGENTA		:= \e[0;95m
RESET		:= \e[0;0m
PREFIX		:= $(MAGENTA)$(NAME)$(RESET) => 

$(NAME): $(OBJS)
	@echo "$(PREFIX)$(GREEN)Bundling $(RESET)$(NAME)$(GREEN) executable$(RESET)"
	@$(CXX) $(CXXFLAGS) $(OBJS) $(LIBRARY) -o $@

%.o: %.cpp
	@echo "$(PREFIX)$(GREEN)Compiling file $(RESET)$< $(BLUE)to $(RESET)$@"
	@$(CXX) $(CXXFLAGS) $(LIBRARY) -c $< -o $@

all: help $(NAME)

help:
	@if command -v xxd > /dev/null; then \
		echo "$(PREFIX)$(GREEN)xxd generate documentation$(RESET)"; \
		xxd -i ./srcs/help/documentation > srcs/help/generateDocumentation.hpp; \
	else \
		echo "$(PREFIX)$(RED)xxd command is not present, skip this part$(RESET)"; \
	fi

fclean:
	$(RM) $(OBJS)
	$(RM) $(NAME)

clean:
	$(RM) $(OBJS)

re: fclean
	$(MAKE) all

