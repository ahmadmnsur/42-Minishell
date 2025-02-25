/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils4.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjamil <mjamil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 18:21:20 by mjamil            #+#    #+#             */
/*   Updated: 2025/02/25 18:21:20 by mjamil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static t_lexer	*find_next_delimiter(t_lexer *lexer)
{
	while (lexer && lexer->token != TOKEN_PIPE && lexer
		->token != TOKEN_SEMICOLON)
		lexer = lexer->next;
	return (lexer);
}

static void	process_parser_node(t_parser *parser_node, t_assign *assign)
{
	t_lexer	*delim;

	delim = find_next_delimiter(assign->lexer);
	if (delim)
	{
		parser_node->tokens = ft_sublexer(assign->lexer_head,
				assign->start_node,
				delim->i - assign->start_node->i);
		assign->start_node = delim->next;
		assign->lexer = delim->next;
	}
	else
		parser_node->tokens = ft_sublexer(assign->lexer_head,
				assign->start_node,
				assign->nb_nodes - assign->start_node->i);
}

void	assign_tokens_to_parser(t_parser **parser, t_lexer *lexer,
				unsigned int nb_nodes)
{
	t_parser	*tmp_parser;
	t_assign	assign;

	tmp_parser = *parser;
	assign.lexer = lexer;
	assign.lexer_head = lexer;
	assign.start_node = lexer;
	assign.nb_nodes = nb_nodes;
	while (tmp_parser && assign.lexer)
	{
		if (assign.start_node && assign.start_node->token == TOKEN_EOF)
			break ;
		process_parser_node(tmp_parser, &assign);
		tmp_parser = tmp_parser->next;
	}
}
