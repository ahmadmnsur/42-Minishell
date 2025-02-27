/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjamil <mjamil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 21:25:48 by mjamil            #+#    #+#             */
/*   Updated: 2025/02/25 21:25:48 by mjamil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	**allocate_and_initialize_result(void)
{
	char	**result;

	result = malloc(5 * sizeof(char *));
	if (!result)
		return (NULL);
	result[0] = NULL;
	result[1] = NULL;
	result[2] = NULL;
	result[3] = NULL;
	result[4] = NULL;
	return (result);
}

void	fill_result_with_values(char **result,
	const char *str, const char *delimiter)
{
	size_t	delim_len;
	char	*delim_pos;
	char	*trimmed_key;
	char	*trimmed_value;

	delim_len = ft_strlen(delimiter);
	delim_pos = ft_strstr(str, delimiter);
	if (!delim_pos)
	{
		result[1] = ft_strdup(str);
		return ;
	}
	result[1] = ft_strndupp(str, delim_pos - str);
	result[2] = ft_strndupp(delimiter, delim_len);
	result[3] = ft_strdup(delim_pos + delim_len);
	trimmed_key = ft_strtrim(result[1], " \t\n\r");
	free(result[1]);
	result[1] = trimmed_key;
	trimmed_value = ft_strtrim(result[3], " \t\n\r");
	free(result[3]);
	result[3] = trimmed_value;
}

char	**export_split(const char *str, const char *delimiter)
{
	char	**result;

	result = allocate_and_initialize_result();
	if (!result)
		return (NULL);
	fill_result_with_values(result, str, delimiter);
	return (result);
}

int	is_valid_identifier(char *str)
{
	int	i;

	if (!str || (!ft_isalpha(str[0]) && str[0] != '_'))
		return (0);
	i = 0;
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

int	handle_invalid_export(char **argcomment)
{
	printf("export: `%s`: not a valid identifier\n", argcomment[1]);
	free_split_array(argcomment);
	return (1);
}
