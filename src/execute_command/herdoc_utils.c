#include "../../minishell.h"

int	handle_sigint(t_heredoc_ctx *ctx, char *line)
{
	free(line);
	close(ctx->tmp_fd);
	unlink(ctx->template);
	free(ctx->delimiter);
	dup2(ctx->stdin_copy, STDIN_FILENO);
	close(ctx->stdin_copy);
	reset_signals();
	return (-1);
}

int	process_heredoc_line(t_heredoc_ctx *ctx, t_tools *tools, char *line)
{
	char	*expanded_line;

	if (ft_cmp(line, ctx->delimiter) == 0)
	{
		free(line);
		return (1);
	}
	if (!ctx->expand_variables)
	{
		expanded_line = expand_heredoc_line(line, tools->env,
				tools->last_exit_status, 1);
		ft_putendl_fd(expanded_line, ctx->tmp_fd);
		free(expanded_line);
	}
	else
		ft_putendl_fd(line, ctx->tmp_fd);
	free(line);
	return (0);
}

int	heredoc_loop(t_heredoc_ctx *ctx, t_tools *tools)
{
	char	*line;
	int		ret;

	while (1)
	{
		line = readline("> ");
		if (g_signum == SIGINT)
			return (handle_sigint(ctx, line));
		if (!line)
		{
			print_heredoc_error(ctx->delimiter);
			break ;
		}
		ret = process_heredoc_line(ctx, tools, line);
		if (ret)
			break ;
	}
	return (0);
}
