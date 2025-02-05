#include "../../minishell.h"

void	add_lexer_and_increment_i(t_lexer **lexer, int token,
		int *i, int increment)
{
	t_lexer	*new;

	new = create_new_lexer_node(NULL, token, -1, 1);
	*i += increment;
	lexer_add_back(lexer, new);
}
