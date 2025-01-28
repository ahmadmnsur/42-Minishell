#include "../../minishell.h"

void	copy_lexer_node(t_lexer **sub, t_lexer **tmp)
{
	t_lexer	*new;

	new = create_new_lexer_node((*tmp)->str, (*tmp)->token,
			(*tmp)->quote_type, (*tmp)->space);
	lexer_add_back(sub, new);
	(*tmp) = (*tmp)->next;
}

void	fill_sub_lexer(t_lexer **sub, t_lexer **tmp, unsigned int sub_size)
{
	unsigned int	i;

	i = 0;
	while (i < sub_size)
	{
		copy_lexer_node(sub, tmp);
		add_index_token(sub);
		i++;
	}
}

unsigned int	get_sub_size(t_lexer *start, unsigned int nb_nodes)
{
	unsigned int	sub_size;

	sub_size = 0;
	while (start && sub_size < nb_nodes)
	{
		start = start->next;
		sub_size++;
	}
	return (sub_size);
}

t_lexer	*find_start_node(t_lexer *lexer, unsigned int start_index)
{
	while (lexer && (unsigned int) lexer->i < start_index)
		lexer = lexer->next;
	return (lexer);
}

t_lexer	*ft_sublexer(t_lexer *lexer, t_lexer *start_node, unsigned int nb_nodes)
{
	t_lexer			*sub;
	t_lexer			*tmp;
	unsigned int	sub_size;

	if (!lexer || nb_nodes == 0)
		return (NULL);
	tmp = find_start_node(lexer, start_node->i);
	if (!tmp)
		return (NULL);
	sub_size = get_sub_size(tmp, nb_nodes);
	if (sub_size == 0)
		return (NULL);
	sub = NULL;
	fill_sub_lexer(&sub, &tmp, sub_size);
	return (sub);
}
