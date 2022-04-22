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
				srcs/middlewares/body.cpp srcs/middlewares/eject.cpp \
				srcs/middlewares/error.cpp srcs/middlewares/forbidden.cpp \
				srcs/middlewares/keep_alive.cpp \
				srcs/middlewares/mimetypes.cpp srcs/middlewares/read.cpp \
				srcs/middlewares/redirect.cpp srcs/middlewares/remover.cpp \
				srcs/middlewares/upload.cpp srcs/middlewares/write_body.cpp \
				srcs/middlewares/write_headers.cpp \
				\
				srcs/serve/Definition.cpp srcs/serve/Chain.cpp srcs/serve/Serve.cpp \
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
CXXFLAGS	?=  -g -Wall -Werror -Wextra -std=c++98 $(LIBRARY)

# Colours
RED			:= \e[0;91m
GREEN		:= \e[0;92m
MAGENTA		:= \e[0;95m
RESET		:= \e[0;0m
PREFIX		:= $(MAGENTA)$(NAME)$(RESET) => 

$(NAME): help $(OBJS)
	@echo "$(PREFIX)$(GREEN)Bundling $(RESET)$(NAME)$(GREEN) executable$(RESET)"
	@$(CXX) $(CXXFLAGS) $(OBJS) $(LIBRARY) -o $@

%.o: %.cpp
	@echo "$(PREFIX)$(GREEN)Compiling file $(RESET)$< $(GREEN)to $(RESET)$@"
	@$(CXX) $(CXXFLAGS) $(LIBRARY) -c $< -o $@

all: $(NAME)

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

test: fclean
	$(MAKE) all
	./$(NAME) website_test/init.conf
