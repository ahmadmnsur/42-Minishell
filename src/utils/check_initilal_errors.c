#include "../../minishell.h"

static int	check_quote_error(char *prompt, t_tools *tools)
{
	char	c;

	if (check_unclosed_quotes(prompt))
	{
		c = check_unclosed_quotes(prompt);
		if (c == '\'')
			print_syntax_error("'", &(tools->last_exit_status));
		else
			print_syntax_error("\"", &(tools->last_exit_status));
		free_tools(tools);
		return (0);
	}
	return (1);
}

static int	check_directory_error(char *prompt, t_tools *tools)
{
	if (is_directory_path(prompt))
	{
		ft_putstr_fd("bash: ", 2);
		ft_putstr_fd(prompt, 2);
		ft_putstr_fd(": Is a directory\n", 2);
		tools->last_exit_status = 126;
		free_tools(tools);
		return (0);
	}
	return (1);
}

static int	check_and_error(char *prompt, t_tools *tools)
{
	int	and_count;

	and_count = is_only_ands(prompt);
	if (and_count == 1)
	{
		ft_putstr_fd("bash: syntax error near unexpected token '&'\n", 2);
		tools->last_exit_status = 2;
		free_tools(tools);
		return (0);
	}
	if (and_count >= 2)
	{
		ft_putstr_fd("bash: syntax error near unexpected token '&&'\n", 2);
		tools->last_exit_status = 2;
		free_tools(tools);
		return (0);
	}
	return (1);
}

static int	check_redirection_error(t_tools *tools)
{
	if (!check_parser_redirections(tools->parser))
	{
		tools->last_exit_status = 2;
		free_tools(tools);
		return (0);
	}
	return (1);
}

int	check_initial_errors(char *prompt, t_tools *tools)
{
	if (!check_quote_error(prompt, tools))
		return (0);
	if (!check_directory_error(prompt, tools))
		return (0);
	if (!check_and_error(prompt, tools))
		return (0);
	if (!check_exclamation(prompt, tools))
		return (0);
	if (!check_colon(prompt, tools))
		return (0);
	if (!check_pipe_error(tools))
		return (0);
	if (!check_redirection_error(tools))
		return (0);
	return (1);
}
