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

char *my_strtok(char *str, const char *delim)
{
    static char *next;
    char *start;

    if (!str && (!next || *next == '\0'))
        return NULL;
    if (str)
        next = str;
    start = next;
    while (*next && !ft_strrchr(delim, *next))
        next++;
    if (*next)
        *next++ = '\0';
    return start;
}


int process_redirections_child_for_no_pipe(t_tools *tools, t_lexer *redirects)
{
    t_lexer *current = redirects;
    int fd;
    char *filename;

    while (current)
    {
        if (!current->next || current->next->token != TOKEN_WORD)
        {
            fprintf(stderr, "minishell: syntax error near unexpected token\n");
            return -1;
        }

        filename = current->next->str;
        if (!filename)
        {
            fprintf(stderr, "minishell: error processing redirection\n");
            return -1;
        }

        if (current->token == TOKEN_HEREDOC)
        {
            // Handle Heredoc and ensure filename is valid
            rl_clear_history();
            filename = handle_heredoc_case(tools->parser->hd_delimiters, tools, current->quote_type);
            
            if (!filename)
            {
                fprintf(stderr, "minishell: error processing heredoc\n");
                return -1;
            }
            fd = open(filename, O_RDONLY);
        }
        else if (current->token == TOKEN_REDIRECT_IN)
            fd = open(filename, O_RDONLY);
        else if (current->token == TOKEN_REDIRECT_OUT)
            fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        else if (current->token == TOKEN_APPEND)
            fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
        else
        {
            fprintf(stderr, "minishell: syntax error near unexpected token\n");
            return -1;
        }

        if (fd == -1)
        {
            perror("minishell");
            return -1;
        }

        // Redirect stdin/stdout based on the token type
        if (current->token == TOKEN_REDIRECT_IN || current->token == TOKEN_HEREDOC)
        {
            if (dup2(fd, STDIN_FILENO) == -1)
            {
                perror("minishell");
                close(fd);
                return -1;
            }
        }
        else // TOKEN_REDIRECT_OUT or TOKEN_APPEND
        {
            if (dup2(fd, STDOUT_FILENO) == -1)
            {
                perror("minishell");
                close(fd);
                return -1;
            }
        }

        close(fd);
        current = current->next->next; // Move to the next redirection
    }
    return 0;
}

char **build_args(t_lexer *tokens)
{
    int count = 0;
    t_lexer *tmp = tokens;
    while (tmp && tmp->token != TOKEN_PIPE && tmp->token != TOKEN_EOF)
    {
        if (tmp->token == TOKEN_WORD)
            count++;
        tmp = tmp->next;
    }
    char **args = malloc((count + 1) * sizeof(char *));
    if (!args)
        return NULL;
    int i = 0;
    tmp = tokens;
    while (tmp && i < count)
    {
        if (tmp->token == TOKEN_WORD)
        {
            args[i] = ft_strdup(tmp->str);
            if (!args[i]) {
                while (i > 0)
                    free(args[--i]);
                free(args);
                return NULL;
            }
            i++;
        }
        tmp = tmp->next;
    }
    args[i] = NULL;
    return args;
}

char *get_command_path(char *cmd, t_env *env)
{
    if (ft_strrchr(cmd, '/') != NULL)
    {
        if (access(cmd, F_OK) == 0)
            return ft_strdup(cmd);
        else
            return NULL;
    }
    char *path_env = NULL;
    while (env) {
        if (ft_cmp(env->key, "PATH") == 0)
        {
            path_env = env->value;
            break;
        }
        env = env->next;
    }
    if (!path_env)
        return NULL;
    char *path = ft_strdup(path_env);
    char *dir = my_strtok(path, ":");
    while (dir)
    {
        char *full_path = malloc(ft_strlen(dir) + ft_strlen(cmd) + 2);
        sprintf(full_path, "%s/%s", dir, cmd);
        if (access(full_path, F_OK) == 0)
        {
            free(path);
            return full_path;
        }
        free(full_path);
        dir = my_strtok(NULL, ":");
    }
    free(path);
    return NULL;
}


int if_no_pipe(t_tools *tools, t_parser *parser, char **envp)
{
    (void)envp;  // Mark envp as unused
    int exit_status = 0;
    pid_t pid;
    char **local_envp;
    
    if (process_all_heredocs(parser, tools) != 0) {
        return 130;
    }

    if (parser->builtin)
    {
        int save_stdin = dup(STDIN_FILENO);
        int save_stdout = dup(STDOUT_FILENO);
        int save_stderr = dup(STDERR_FILENO);

        if (parser->redirects && process_redirections_child_for_no_pipe(tools, parser->redirects) != 0)
            exit_status = 1;
        else
            exit_status = parser->builtin(parser, tools->env);

        dup2(save_stdin, STDIN_FILENO);
        close(save_stdin);
        dup2(save_stdout, STDOUT_FILENO);
        close(save_stdout);
        dup2(save_stderr, STDERR_FILENO);
        close(save_stderr);
        return exit_status;
    }
    else
    {
        pid = fork();
        if (pid == -1)
        {
            perror("minishell: fork");
            return 1;
        }
        else if (pid == 0)
        {
            if (parser->redirects && process_redirections_child_for_no_pipe(tools, parser->redirects) != 0)
                exit(EXIT_FAILURE);

            if (!parser->tokens || !parser->tokens->str)
            {
                t_lexer *tmp = parser->tokens;
                while (tmp && tmp->token != TOKEN_WORD)
                    tmp = tmp->next;
                if (!tmp || !tmp->str)
                    exit(0);
                parser->tokens = tmp;
            }

            char *path = get_command_path(parser->tokens->str, tools->env);
            if (!path) {
                fprintf(stderr, "minishell: %s: command not found\n", parser->tokens->str);
                exit(127);
            }
            char **args = build_args(parser->tokens);
            if (!args) {
                free(path);
                exit(EXIT_FAILURE);
            }
            local_envp = copy_envp_to_execve(tools->env);
            execve(path, args, local_envp);
            perror("minishell");
            free(path);
            for (int i = 0; args[i]; i++)
                free(args[i]);
            free(args);
            exit(EXIT_FAILURE);
        }
        else
        {
            waitpid(pid, &exit_status, 0);
            if (WIFEXITED(exit_status))
                exit_status = WEXITSTATUS(exit_status);
            else if (WIFSIGNALED(exit_status))
                exit_status = 128 + WTERMSIG(exit_status);
        }
    }
    return exit_status;
}