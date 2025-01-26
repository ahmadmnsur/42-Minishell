/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjamil <mjamil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 14:02:45 by mjamil            #+#    #+#             */
/*   Updated: 2025/01/23 14:33:12 by mjamil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdlib.h>
# include <stdio.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/stat.h>
# include <errno.h>
# include <stdbool.h>
# include <signal.h>
# include "libft/libft.h"

typedef enum e_tokens
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIRECT_IN,
	TOKEN_REDIRECT_OUT,
	TOKEN_HEREDOC,
	TOKEN_APPEND,
	TOKEN_EOF
}	t_tokens;

typedef enum e_quote_type
{
	NO_QUOTE,
	SINGLE_QUOTES,
	DOUBLE_QUOTES,
	QUOTE_ERROR
}	t_quote_type;

typedef struct s_lexer
{
	char			*str;
	int				i;
	t_tokens		token;
	t_quote_type	quote_type;
	int				space;
	struct s_lexer	*next;
}	t_lexer;

typedef struct s_env
{
	char			*key;
	char			*value;
	int				hidden;
	struct s_env	*next;
    int 			two_point;
}	t_env;

typedef struct s_parser
{
	t_lexer				*tokens;
	int					(*builtin)(struct s_parser *, t_env *);
	t_lexer				*redirects;
	char				**hd_delimiters;
	struct s_parser		*next;
	struct s_parser		*prev;
}	t_parser;

typedef struct s_tools
{
	int			last_exit_status;
	t_lexer		*lexer;
	t_parser	*parser;
	t_env		*env;
}	t_tools;

extern int	g_signum;


int     builtin_env(t_parser *parser, t_env *env);
int	    builtin_pwd(t_parser *parser, t_env *env);
int		builtin_exit(t_parser *parser, t_env *env);
int     builtin_unset(t_parser *parser, t_env *env);
int		builtin_export(t_parser *parser, t_env *env);
int		builtin_cd(t_parser *parser, t_env *env);
char	*get_env(t_env *head, const char *key);
int     ft_cmp(const char *s1, const char *s2);
int		get_cmd_len(t_lexer *lexer);
char	**get_cmd(t_lexer *lexer);
int		is_numeric(const char *str);
void 	add_env_var(t_env **env, const char *key, const char *value);
void 	update_env_var(t_env **env, const char *key, const char *value);
void 	print_full_export_list(t_env *env);
char 	**export_split(const char *str, const char *delimiter);
void 	free_split_array(char **array);
t_env	*create_env_node(char *key, char *value, int hidden);
int		set_env(t_env **head, const char *key, const char *value, int hidden);

#endif