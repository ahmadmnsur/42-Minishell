/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjamil <mjamil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 16:00:50 by mjamil            #+#    #+#             */
/*   Updated: 2025/01/31 23:22:11 by mjamil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"


int process_all_heredocs(t_parser *parser, t_tools *tools) {
    t_parser *curr = parser;
    while (curr) {
        t_lexer *redirect = curr->redirects;
        while (redirect) {
            if (redirect->token == TOKEN_HEREDOC) {
                if (!redirect->next || redirect->next->token != TOKEN_WORD) {
                    fprintf(stderr, "minishell: syntax error near unexpected token\n");
                    return -1;
                }
                // Process heredoc in parent
                char *delimiter = redirect->next->str;
                char *filename = handle_heredoc_case(&delimiter, tools, redirect->quote_type);
                if (!filename) {
                    return -1;
                }
                // Replace heredoc token with redirect_in and update filename
                redirect->token = TOKEN_REDIRECT_IN;
                free(redirect->next->str);
                redirect->next->str = filename;
            }
            redirect = redirect->next;  // Move to next node directly
        }
        curr = curr->next;
    }
    return 0;
}


static int process_redirections_child(t_tools *tools, t_lexer *redirects) {
    (void)tools;
    t_lexer *current = redirects;
    int fd;
    char *filename;

    while (current) {
        if (!current->next || current->next->token != TOKEN_WORD) {
            fprintf(stderr, "minishell: syntax error near unexpected token\n");
            return -1;
        }

        filename = current->next->str;
        if (!filename) {
            fprintf(stderr, "minishell: error processing redirection\n");
            return -1;
        }

        if (current->token == TOKEN_REDIRECT_IN)
            fd = open(filename, O_RDONLY);
        else if (current->token == TOKEN_REDIRECT_OUT)
            fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        else if (current->token == TOKEN_APPEND)
            fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
        else {
            fprintf(stderr, "minishell: syntax error near unexpected token\n");
            return -1;
        }

        if (fd == -1) {
            perror("minishell");
            return -1;
        }

        if (current->token == TOKEN_REDIRECT_IN) {
            if (dup2(fd, STDIN_FILENO) == -1) {
                perror("minishell");
                close(fd);
                return -1;
            }
        } else {
            if (dup2(fd, STDOUT_FILENO) == -1) {
                perror("minishell");
                close(fd);
                return -1;
            }
        }

        close(fd);
        current = current->next->next;
    }
    return 0;
}


static int if_mult_pipe(t_tools *tools, t_parser *parser, int num_pipes, char **envp) {
    // Process all heredocs in parent before forking
    if (process_all_heredocs(parser, tools) != 0) {
        return -1;
    }

    int num_cmd = num_pipes + 1;
    int status;
    int last_status = 0;
    pid_t *child_pids = malloc(num_cmd * sizeof(pid_t));
    if (!child_pids)
        return (-1);
    int nbr_of_pipe = num_cmd - 1;
    int **pipes = NULL;
    if (nbr_of_pipe > 0) {
        pipes = malloc(sizeof(int *) * nbr_of_pipe);
        if (!pipes) {
            free(child_pids);
            return (-1);
        }
        for (int i = 0; i < nbr_of_pipe; i++) {
            pipes[i] = malloc(sizeof(int) * 2);
            if (pipe(pipes[i]) < 0) {
                perror("minishell: pipe");
                for (int j = 0; j < i; j++) {
                    free(pipes[j]);
                }
                free(pipes);
                free(child_pids);
                return (-1);
            }
        }
    }

    int i = 0;
    t_parser *curr = parser;
    while (curr) {
        child_pids[i] = fork();
        if (child_pids[i] == -1) {
            perror("minishell: fork");
            return (-1);
        } else if (child_pids[i] == 0) {
            if (i > 0) {
                if (dup2(pipes[i - 1][0], STDIN_FILENO) == -1) {
                    perror("minishell: dup2");
                    exit(EXIT_FAILURE);
                }
            }
            if (i < num_cmd - 1) {
                if (dup2(pipes[i][1], STDOUT_FILENO) == -1) {
                    perror("minishell: dup2");
                    exit(EXIT_FAILURE);
                }
            }
            if (nbr_of_pipe > 0) {
                for (int x = 0; x < nbr_of_pipe; x++) {
                    close(pipes[x][0]);
                    close(pipes[x][1]);
                }
            }
            if (curr->redirects) {
                if (process_redirections_child(tools, curr->redirects) != 0)
                    exit(EXIT_FAILURE);
            }
            if (!curr->tokens || !curr->tokens->str)
                exit(0);
            char *path = get_command_path(curr->tokens->str, tools->env);
            if (!path) {
                fprintf(stderr, "minishell: %s: command not found\n", curr->tokens->str);
                exit(127);
            }
            char **args = build_args(curr->tokens);
            if (!args) {
                free(path);
                exit(EXIT_FAILURE);
            }
            execve(path, args, envp);
            perror("minishell");
            free(path);
            for (int j = 0; args[j]; j++)
                free(args[j]);
            free(args);
            exit(EXIT_FAILURE);
        } else {
            if (i > 0)
                close(pipes[i - 1][0]);
            if (i < num_cmd - 1)
                close(pipes[i][1]);
        }
        curr = curr->next;
        i++;
    }

    if (nbr_of_pipe > 0) {
        for (i = 0; i < nbr_of_pipe; i++)
            close(pipes[i][0]);
    }

    for (int i = 0; i < num_cmd; i++) {
        pid_t wpid = waitpid(child_pids[i], &status, 0);
        if (wpid == child_pids[num_cmd - 1]) {
            if (WIFEXITED(status))
                last_status = WEXITSTATUS(status);
            else if (WIFSIGNALED(status))
                last_status = 128 + WTERMSIG(status);
        }
    }
    if (nbr_of_pipe > 0) {
        for (i = 0; i < nbr_of_pipe; i++)
            free(pipes[i]);
        free(pipes);
    }
    free(child_pids);
    return last_status;
}
























// static int if_mult_pipe(t_tools *tools, t_parser *parser, int num_pipes, char **envp)
// {
	
//     int num_cmd = num_pipes + 1;
//     int status;
//     int last_status = 0;
//     pid_t *child_pids = malloc(num_cmd * sizeof(pid_t));
//     if (!child_pids)
//         return (-1);
//     int nbr_of_pipe = num_cmd - 1;
//     int **pipes = NULL;
//     if (nbr_of_pipe > 0) {
//         pipes = malloc(sizeof(int *) * nbr_of_pipe);
//         if (!pipes) {
//             free(child_pids);
//             return (-1);
//         }
//         for (int i = 0; i < nbr_of_pipe; i++) {
//             pipes[i] = malloc(sizeof(int) * 2);
//             if (pipe(pipes[i]) < 0) {
//                 perror("minishell: pipe");
//                 for (int j = 0; j < i; j++) {
//                     free(pipes[j]);
//                 }
//                 free(pipes);
//                 free(child_pids);
//                 return (-1);
//             }
//         }
//     }

//     int i = 0;
//     t_parser *curr = parser;
//     while (curr)
//     {
//         child_pids[i] = fork();
//         if (child_pids[i] == -1)
//         {
//             perror("minishell: fork");
//             return (-1);
//         }
//         else if (child_pids[i] == 0)
//         {
//             if (i > 0) {
//                 dup2(pipes[i - 1][0], STDIN_FILENO);
//             }
//             if (i < num_cmd - 1) {
//                 dup2(pipes[i][1], STDOUT_FILENO);
//             }
//             if (nbr_of_pipe > 0) {
//                 for (int x = 0; x < nbr_of_pipe; x++) {
//                     close(pipes[x][0]);
//                     close(pipes[x][1]);
//                 }
//             }
static int if_mult_pipe(t_tools *tools, t_parser *parser, int num_pipes, char **envp)
{
    int num_cmd = num_pipes + 1;
    int status;
    int last_status = 0;
    pid_t *child_pids = malloc(num_cmd * sizeof(pid_t));
    if (!child_pids)
        return (-1);
    int nbr_of_pipe = num_cmd - 1;
    int **pipes = NULL;
    if (nbr_of_pipe > 0) {
        pipes = malloc(sizeof(int *) * nbr_of_pipe);
        if (!pipes) {
            free(child_pids);
            return (-1);
        }
        for (int i = 0; i < nbr_of_pipe; i++) {
            pipes[i] = malloc(sizeof(int) * 2);
            if (pipe(pipes[i]) < 0) {
                perror("minishell: pipe");
                for (int j = 0; j < i; j++) {
                    free(pipes[j]);
                }
                free(pipes);
                free(child_pids);
                return (-1);
            }
        }
    }

    int i = 0;
    t_parser *curr = parser;
    while (curr)
    {
        child_pids[i] = fork();
        if (child_pids[i] == -1)
        {
            perror("minishell: fork");
            return (-1);
        }
        else if (child_pids[i] == 0)
        {
            if (i > 0) {
                if (dup2(pipes[i - 1][0], STDIN_FILENO) == -1) {
                    perror("minishell: dup2");
                    exit(EXIT_FAILURE);
                }
            }
            if (i < num_cmd - 1) {
                if (dup2(pipes[i][1], STDOUT_FILENO) == -1) {
                    perror("minishell: dup2");
                    exit(EXIT_FAILURE);
                }
            }
            if (nbr_of_pipe > 0) {
                for (int x = 0; x < nbr_of_pipe; x++) {
                    close(pipes[x][0]);
                    close(pipes[x][1]);
                }
            }
            if (curr->redirects)
                if (process_redirections_child(tools, curr->redirects) != 0)
                    exit(EXIT_FAILURE);

            if (!curr->tokens || !curr->tokens->str)
                exit(0);
            char *path = get_command_path(curr->tokens->str, tools->env);
            if (!path) {
                fprintf(stderr, "minishell: %s: command not found\n", curr->tokens->str);
                exit(127);
            }
            char **args = build_args(curr->tokens);
            if (!args) {
                free(path);
                exit(EXIT_FAILURE);
            }
            execve(path, args, envp);
            perror("minishell");
            free(path);
            for (int j = 0; args[j]; j++)
                free(args[j]);
            free(args);
            exit(EXIT_FAILURE);
        }
        else
        {
            if (i > 0)
                close(pipes[i - 1][0]);
            if (i < num_cmd - 1)
                close(pipes[i][1]);
        }
        curr = curr->next;
        i++;
    }

    if (nbr_of_pipe > 0)
    {
        for (i = 0; i < nbr_of_pipe; i++)
            close(pipes[i][0]);
    }

    for (int i = 0; i < num_cmd; i++)
    {
        pid_t wpid = waitpid(child_pids[i], &status, 0);
        if (wpid == child_pids[num_cmd - 1])
        {
            if (WIFEXITED(status))
                last_status = WEXITSTATUS(status);
            else if (WIFSIGNALED(status))
                last_status = 128 + WTERMSIG(status);
        }
    }
    if (nbr_of_pipe > 0)
    {
        for (i = 0; i < nbr_of_pipe; i++)
            free(pipes[i]);
        free(pipes);
    }
    free(child_pids);
    return last_status;
}

// 			if (parser->redirects)
//     			process_redirections_child(tools, parser->redirects);
//             if_no_pipe(tools, curr, envp);
//             exit(EXIT_FAILURE);
//         }
//         else
//         {
//             if (i > 0)
//                 close(pipes[i - 1][0]);
//             if (i < num_cmd - 1)
//                 close(pipes[i][1]);
//         }
//         curr = curr->next;
//         i++;
//     }

//     if (nbr_of_pipe > 0)
//     {
//         for (i = 0; i < nbr_of_pipe; i++)
//             close(pipes[i][0]);
//     }

//     for (int i = 0; i < num_cmd; i++)
// 	{
// 		pid_t wpid = waitpid(child_pids[i], &status, 0);
// 		if (wpid == child_pids[num_cmd - 1])
// 		{
// 			if (WIFEXITED(status))
// 				last_status = WEXITSTATUS(status) - 1;
// 			else if (WIFSIGNALED(status))
// 				last_status = 128 + WTERMSIG(status);
// 		}
// 	}
//     if (nbr_of_pipe > 0)
//     {
//         for (i = 0; i < nbr_of_pipe; i++)
//             free(pipes[i]);
//         free(pipes);
//     }
//     free(child_pids);
//     return last_status;
// }


// static int if_mult_pipe(t_tools *tools, t_parser *parser, int num_pipes, char **envp)
// {
//     int num_cmd = num_pipes + 1;
//     int status;
//     int last_status = 0;
//     pid_t *child_pids = malloc(num_cmd * sizeof(pid_t));
//     if (!child_pids)
//         return (-1);
//     int nbr_of_pipe = num_cmd - 1;
//     int **pipes = NULL;
//     if (nbr_of_pipe > 0) {
//         pipes = malloc(sizeof(int *) * nbr_of_pipe);
//         if (!pipes) {
//             free(child_pids);
//             return (-1);
//         }
//         for (int i = 0; i < nbr_of_pipe; i++) {
//             pipes[i] = malloc(sizeof(int) * 2);
//             if (pipe(pipes[i]) < 0) {
//                 perror("minishell: pipe");
//                 for (int j = 0; j < i; j++) {
//                     free(pipes[j]);
//                 }
//                 free(pipes);
//                 free(child_pids);
//                 return (-1);
//             }
//         }
//     }

//     int i = 0;
//     t_parser *curr = parser;
//     while (curr)
//     {
//         child_pids[i] = fork();
//         if (child_pids[i] == -1)
//         {
//             perror("minishell: fork");
//             return (-1);
//         }
//         else if (child_pids[i] == 0)
//         {
//             if (i > 0) {
//                 if (dup2(pipes[i - 1][0], STDIN_FILENO) == -1) {
//                     perror("minishell: dup2");
//                     exit(EXIT_FAILURE);
//                 }
//             }
//             if (i < num_cmd - 1) {
//                 if (dup2(pipes[i][1], STDOUT_FILENO) == -1) {
//                     perror("minishell: dup2");
//                     exit(EXIT_FAILURE);
//                 }
//             }
//             if (nbr_of_pipe > 0) {
//                 for (int x = 0; x < nbr_of_pipe; x++) {
//                     close(pipes[x][0]);
//                     close(pipes[x][1]);
//                 }
//             }
//             if (curr->redirects)
//                 if (process_redirections_child(tools, curr->redirects) != 0)
//                     exit(EXIT_FAILURE);

//             if (!curr->tokens || !curr->tokens->str)
//                 exit(0);
//             char *path = get_command_path(curr->tokens->str, tools->env);
//             if (!path) {
//                 fprintf(stderr, "minishell: %s: command not found\n", curr->tokens->str);
//                 exit(127);
//             }
//             char **args = build_args(curr->tokens);
//             if (!args) {
//                 free(path);
//                 exit(EXIT_FAILURE);
//             }
//             execve(path, args, envp);
//             perror("minishell");
//             free(path);
//             for (int j = 0; args[j]; j++)
//                 free(args[j]);
//             free(args);
//             exit(EXIT_FAILURE);
//         }
//         else
//         {
//             if (i > 0)
//                 close(pipes[i - 1][0]);
//             if (i < num_cmd - 1)
//                 close(pipes[i][1]);
//         }
//         curr = curr->next;
//         i++;
//     }

//     if (nbr_of_pipe > 0)
//     {
//         for (i = 0; i < nbr_of_pipe; i++)
//             close(pipes[i][0]);
//     }

//     for (int i = 0; i < num_cmd; i++)
//     {
//         pid_t wpid = waitpid(child_pids[i], &status, 0);
//         if (wpid == child_pids[num_cmd - 1])
//         {
//             if (WIFEXITED(status))
//                 last_status = WEXITSTATUS(status);
//             else if (WIFSIGNALED(status))
//                 last_status = 128 + WTERMSIG(status);
//         }
//     }
//     if (nbr_of_pipe > 0)
//     {
//         for (i = 0; i < nbr_of_pipe; i++)
//             free(pipes[i]);
//         free(pipes);
//     }
//     free(child_pids);
//     return last_status;
// }
