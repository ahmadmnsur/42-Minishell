/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_special_case.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjamil <mjamil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 14:42:03 by mjamil            #+#    #+#             */
/*   Updated: 2025/02/25 14:42:03 by mjamil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	handle_qst_str(t_tools *tools, int **i, char ***result,
    size_t	size)
{
	char	*sub;

	sub = ft_itoa(tools->last_exit_status);
	ft_strlcat(**result, sub, size);
	free(sub);
	(**i)++;
}

static void	handle_special_cases(t_lexer *current, int **i, char ***result,
    size_t size)
{
	char	*sub;
	int		j;

	if (current->str[**i] && ft_isdigit(current->str[**i]))
	{
		(**i)++;
		j = 0;
		while (current->str[**i + j] && ft_isalnum(current->str[**i + j]))
			j++;
		sub = ft_substr(current->str, **i, j);
		if (sub)
			ft_strlcat(**result, sub, size);
		free(sub);
		**i += j;
		return ;
	}
	if (current->str[**i] && current->str[**i] == '$')
	{
		sub = ft_itoa((int)get_pid());
		ft_strlcat(**result, sub, size);
		free(sub);
		(**i)++;
		return ;
	}
}

static void	handle_empty_sub(char **sub, t_lexer *current, char ***result,
    size_t size)
{
	if (current->space || current->quote_type == DOUBLE_QUOTES)
	ft_strlcat(**result, "$", size);
	free(*sub);
}

void	handle_dollar_sign_str(t_lexer *current, t_tools *tools,
	int *i, char **result)
{
	int		j;
	char	*sub;
	t_env	*env_var;
	size_t	size;

	size = get_new_string_length(current, tools) + 1;
	(*i)++;
	j = 0;
	if (current->str[*i] && current->str[*i] == '?')
		return (handle_qst_str(tools, &i, &result, size), (void)0);
	if (is_a_special_case(current, i))
		return (handle_special_cases(current, &i, &result, size), (void)0);
	while (current->str[*i + j] && (ft_isalnum(current->str[*i + j])
		|| current->str[*i + j] == '_'))
		j++;
	sub = ft_substr(current->str, *i, j);
	if (!sub || !*sub)
		return (handle_empty_sub(&sub, current, &result, size), (void)0);
	env_var = get_env_var(tools->env, sub);
	if (env_var && env_var->value)
		ft_strlcat(*result, env_var->value, size);
	free(sub);
	*i += j;
}

void	handle_question_mark(t_tools *tools, char **sub, int **i, int **len)
{
	*sub = ft_itoa(tools->last_exit_status);
	**len += ft_strlen(*sub);
	free(*sub);
	(**i)++;
}
