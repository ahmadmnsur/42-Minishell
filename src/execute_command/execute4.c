/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute4.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjamil <mjamil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 03:50:59 by mjamil            #+#    #+#             */
/*   Updated: 2025/02/14 03:50:59 by mjamil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char *get_env_value(const char *key, t_env *env)
{
    while (env)
    {
        if (strcmp(env->key, key) == 0)
            return strdup(env->value);
        env = env->next;
    }
    return NULL;
}


char *expand_heredoc_line(char *line, t_env *env, int last_ret, int expand_variables)
{
    char *expanded = malloc(1000);
    if (!expanded)
        return NULL;

    size_t i = 0, j = 0;
    while (line[j])
    {
        if (expand_variables && line[j] == '$' &&
            (line[j + 1] == '?' || isalpha((unsigned char)line[j + 1]) || line[j + 1] == '_'))
        {
            j++;  // Skip the '$'
            if (line[j] == '?')  
            {
                char ret_str[12];
                snprintf(ret_str, sizeof(ret_str), "%d", last_ret);
                size_t k = 0;
                while (ret_str[k])
                    expanded[i++] = ret_str[k++];
                j++;
            }
            else  
            {
                size_t key_start = j;
                while (isalnum((unsigned char)line[j]) || line[j] == '_')
                    j++;

                char *key = strndup(line + key_start, j - key_start);
                if (!key)
                    continue;
                char *value = get_env_value(key, env);
                free(key);
                if (value)
                {
                    size_t k = 0;
                    while (value[k])
                        expanded[i++] = value[k++];
                    free(value);
                }
            }
        }
        else  // Regular character; simply copy it.
        {
            expanded[i++] = line[j++];
        }
    }
    expanded[i] = '\0';
    return expanded;
}

char *handle_heredoc_case(char **delimiters, t_tools *tools, t_quote_type quote_type) {
    char *delimiter;
    char template[] = "/tmp/minishell_heredocXXXXXX";
    int tmp_fd;
    FILE *tmp_file;
    char *line;
    char *expanded_line;
    int expand_variables;
    int stdin_copy;

    // Save a copy of stdin
    stdin_copy = dup(STDIN_FILENO);
    if (stdin_copy == -1) {
        perror("minishell");
        return NULL;
    }

    // Set specific signal handler for heredoc
    signal(SIGINT, handle_heredoc_signal);
    signal(SIGQUIT, SIG_IGN);
    g_signum = 0;
    
    if (!delimiters || !delimiters[0]) {
        close(stdin_copy);
        reset_signals();
        return NULL;
    }
    
    delimiter = ft_strtrim(delimiters[0], " \t\n");
    if (!delimiter) {
        perror("minishell");
        close(stdin_copy);
        reset_signals();
        return NULL;
    }

    tmp_fd = mkstemp(template);
    if (tmp_fd == -1) {
        perror("minishell");
        free(delimiter);
        close(stdin_copy);
        reset_signals();
        return NULL;
    }
    
    tmp_file = fdopen(tmp_fd, "w+");
    if (!tmp_file) {
        perror("minishell");
        close(tmp_fd);
        free(delimiter);
        close(stdin_copy);
        reset_signals();
        return NULL;
    }

    expand_variables = (quote_type == NO_QUOTE);

    while (1) {
        line = readline("> ");
        
        if (g_signum == SIGINT) {
            free(line);
            fclose(tmp_file);
            unlink(template);
            free(delimiter);
            
            // Restore stdin
            dup2(stdin_copy, STDIN_FILENO);
            close(stdin_copy);
            
            reset_signals();
            return NULL;
        }

        // Handle EOF
        if (!line) {
            fprintf(stderr, 
                "minishell: warning: here-document delimited by end-of-file (wanted `%s')\n",
                delimiter);
            break;
        }

        if (strcmp(line, delimiter) == 0) {
            free(line);
            break;
        }

        if (expand_variables) {
            expanded_line = expand_heredoc_line(line, tools->env,
                                              tools->last_exit_status, 1);
            fprintf(tmp_file, "%s\n", expanded_line);
            free(expanded_line);
        } else {
            fprintf(tmp_file, "%s\n", line);
        }
        free(line);
    }

    // Restore stdin before returning
    dup2(stdin_copy, STDIN_FILENO);
    close(stdin_copy);

    free(delimiter);
    fflush(tmp_file);
    fclose(tmp_file);
    reset_signals();
    
    if (g_signum == SIGINT) {
        unlink(template);
        return NULL;
    }

    return ft_strdup(template);
}








// char *expand_heredoc_line(char *line, t_env *env, int last_ret, int expand_variables)
// {
//     char *expanded = malloc(1000);
//     if (!expanded)
//         return NULL;

//     size_t i = 0, j = 0;
//     while (line[j])
//     {
//         if (expand_variables && line[j] == '$' &&
//             (line[j + 1] == '?' || isalpha((unsigned char)line[j + 1]) || line[j + 1] == '_'))
//         {
//             j++;  // Skip the '$'
//             if (line[j] == '?')  
//             {
//                 char ret_str[12];
//                 snprintf(ret_str, sizeof(ret_str), "%d", last_ret);
//                 size_t k = 0;
//                 while (ret_str[k])
//                     expanded[i++] = ret_str[k++];
//                 j++;
//             }
//             else  
//             {
//                 size_t key_start = j;
//                 while (isalnum((unsigned char)line[j]) || line[j] == '_')
//                     j++;

//                 char *key = strndup(line + key_start, j - key_start);
//                 if (!key)
//                     continue;
//                 char *value = get_env_value(key, env);
//                 free(key);
//                 if (value)
//                 {
//                     size_t k = 0;
//                     while (value[k])
//                         expanded[i++] = value[k++];
//                     free(value);
//                 }
//             }
//         }
//         else  // Regular character; simply copy it.
//         {
//             expanded[i++] = line[j++];
//         }
//     }
//     expanded[i] = '\0';
//     return expanded;
// }

// char	*handle_heredoc_case(char **delimiters, t_tools *tools, t_quote_type quote_type)
// {
//     char *delimiter;
//     char template[] = "/tmp/minishell_heredocXXXXXX";
//     int tmp_fd;
//     FILE *tmp_file;
//     char *line;
//     char *expanded_line;
//     int expand_variables;
//     int delimiter_found = 0;

//     reset_signals();

//     if (!delimiters || !delimiters[0])
//     {
//         return NULL;
//     }
//     delimiter = ft_strtrim(delimiters[0], " \t\n");
//     if (!delimiter)
//     {
//         perror("minishell");
//         return NULL;
//     }

//     tmp_fd = mkstemp(template);
//     if (tmp_fd == -1)
//     {
//         perror("minishell");
//         free(delimiter);
//         return NULL;
//     }
//     tmp_file = fdopen(tmp_fd, "w+");
//     if (!tmp_file)
//     {
//         perror("minishell");
//         close(tmp_fd);
//         free(delimiter);
//         return NULL;
//     }

//     expand_variables = (quote_type == NO_QUOTE);

//     while ((line = readline("> ")) != NULL)
//     {
//         if (g_signum == SIGINT)
//         {
//             free(line);
//             fclose(tmp_file);
//             free(delimiter);
//             return NULL;
//         }
//         if (strcmp(line, delimiter) == 0)
//         {
//             free(line);
//             delimiter_found = 1;
//             break;
//         }
//         if (expand_variables)
//         {
//             expanded_line = expand_heredoc_line(line, tools->env, tools->last_exit_status, 1);
//             fprintf(tmp_file, "%s\n", expanded_line);
//             free(expanded_line);
//         }
//         else
//         {
//             fprintf(tmp_file, "%s\n", line);
//         }
//         free(line);
//     }

//     if (!delimiter_found)
//     {
//         fclose(tmp_file);
//         free(delimiter);
//         return NULL;
//     }

//     reset_signals();
//     free(delimiter);
//     fflush(tmp_file);
//     fclose(tmp_file);

//     return ft_strdup(template);
// }
