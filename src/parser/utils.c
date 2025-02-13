#include "../../minishell.h"

t_parser	*create_new_parser_node(void)
{
	t_parser	*parser;

	parser = malloc(sizeof(t_parser));
	if (!parser)
		return (NULL);
	parser->tokens = NULL;
	parser->builtin = NULL;
	parser->redirects = NULL;
	parser->hd_delimiters = NULL;
	parser->next = NULL;
	parser->prev = NULL;
	return (parser);
}

void	parser_add_back(t_parser **parser)
{
	t_parser	*new_node;
	t_parser	*last_node;

	new_node = create_new_parser_node();
	if (!new_node)
		return ;
	if (*parser == NULL)
	{
		*parser = new_node;
		return ;
	}
	last_node = *parser;
	while (last_node->next != NULL)
		last_node = last_node->next;
	last_node->next = new_node;
	new_node->prev = last_node;
}

void	init_parser_nodes(t_parser **parser, t_lexer *lexer, const char *str)
{
	int	pipe_count;
	int	i;
	int	j;

	if (!str || !str[0])
		return ;
	j = 0;
	while (str[j])
	{
		if (str[j] != ' ' && (str[j] < 9 || str[j] > 13))
			break ;
		j++;
	}
	if (str[j] == '\0')
		return ; // If the string is only spaces, return early

	pipe_count = get_nb_of_pipes(lexer);
	i = 0;
	while (i <= pipe_count)
	{
		parser_add_back(parser);
		i++;
	}
}
