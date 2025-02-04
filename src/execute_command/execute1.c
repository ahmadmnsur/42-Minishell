/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjamil <mjamil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 16:00:50 by mjamil            #+#    #+#             */
/*   Updated: 2025/02/04 16:13:45 by mjamil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int get_pipe_len(t_parser *parser)
{
    int i = 0;
    while (parser)  
    {
        i++;
        parser = parser->next;
    }
    return i;
}

int	execute_command(t_tools *tools, char **envp)
{
	t_parser	*parser;
	int			nbr_of_pipe;

	parser = tools->parser;
    nbr_of_pipe = get_pipe_len(parser);
    nbr_of_pipe = nbr_of_pipe - 1;
    printf("Number of pipes: %d\n", nbr_of_pipe);
    return 0;
}

int main()
{
    t_tools tools;
    t_parser *cmd1 = malloc(sizeof(t_parser));
    t_parser *cmd2 = malloc(sizeof(t_parser));
    t_parser *cmd3 = malloc(sizeof(t_parser));

    if (!cmd1 || !cmd2 || !cmd3)
        return 1;  // Exit if memory allocation fails

    // Initialize the commands
    cmd1->tokens = malloc(sizeof(t_lexer));
    cmd2->tokens = malloc(sizeof(t_lexer));
    cmd3->tokens = malloc(sizeof(t_lexer));

    if (!cmd1->tokens || !cmd2->tokens || !cmd3->tokens)
        return 1;  // Exit if memory allocation fails

    cmd1->tokens->str = "ls";
    cmd2->tokens->str = "grep main";
    cmd3->tokens->str = "wc -l";

    cmd1->next = cmd2;
    cmd2->next = cmd3;
    cmd3->next = NULL;

    cmd1->prev = NULL;
    cmd2->prev = cmd1;
    cmd3->prev = cmd2;

    tools.parser = cmd1;  // Assign the command list to tools

    // Execute command
    execute_command(&tools, NULL);

    // Free allocated memory
    free(cmd1->tokens);
    free(cmd2->tokens);
    free(cmd3->tokens);
    free(cmd1);
    free(cmd2);
    free(cmd3);

    return 0;
}