NAME = minishell

LIBFT = libft

LIBFT_LIB = libft/libft.a

SRCS =  minishell.c										\
		src/lexer/handle_lexer.c					\
		src/lexer/lexer_utils.c					\
		src/lexer/lexer_sublist.c					\
		srcs/lexer/parser_lexer.c						\
		srcs/lexer/parser_lexer.c						\
		srcs/parser/utils_1.c						\
		srcs/parser/free_parser.c						\
		srcs/parser/set_tokens.c						\
		srcs/parser/parser_checker.c					\
		srcs/parser/parser_checker2.c					\
		srcs/parser/set_builtin.c						\
		srcs/parser/hd_limiters.c					\
		srcs/parser/tokens.c				\
		srcs/parser/tokens2.c						\
		srcs/parser/redirection.c						\
		srcs/utils/utils.c								\
		srcs/tools/test.c 						\
		srcs/builtins/echo.c					\
		srcs/builtins/exit.c					\
		srcs/builtins/builtin_env.c						\
		srcs/builtins/export.c					\
		srcs/builtins/unset.c					\
		srcs/builtins/cd.c						\
		srcs/builtins/pwd.c						\
		srcs/builtins/utils.c					\
		srcs/builtins/utils2.c					\
		srcs/execute_command/execute1.c			\
		srcs/execute_command/execute2.c			\
		srcs/execute_command/execute3.c			\
		srcs/execute_command/execute4.c			\
		srcs/execute_command/utils.c			\
		srcs/signals/signals1.c					\
		srcs/signals/signals2.c							\
		srcs/heredoc/test.c							\
		
		
OBJDIR = objs
OBJCS = $(SRCS:%.c=$(OBJDIR)/%.o)

CC = cc

CFLAGS = -Wall -Wextra -Werror -g
READLINE_FLAG = -lreadline

all: $(NAME)

$(OBJDIR): 
	mkdir -p $(OBJDIR)

$(NAME): $(OBJDIR) $(OBJCS)
	make -C $(LIBFT)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJCS) $(READLINE_FLAG) $(LIBFT_LIB)
	@printf "Compiling the entire program...\n"
	@sleep 1
	@printf "\033[A\033[K"
	@printf "\033[32m### Minishell Compiled Successfully! ###\n\033[0m"

$(OBJDIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	make -C $(LIBFT) clean
	rm -f $(OBJCS)
	rm -rf $(OBJDIR)

fclean: clean
	rm -f $(LIBFT_LIB)
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re