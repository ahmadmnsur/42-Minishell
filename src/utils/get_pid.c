/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_pid.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjamil <mjamil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 14:48:50 by mjamil            #+#    #+#             */
/*   Updated: 2025/02/25 14:48:50 by mjamil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

pid_t	get_pid(void)
{
	int		fd;
	char	buffer[256];
	pid_t	pid;
	ssize_t	bytes_read;
	char	*ptr;

	pid = -1;
	fd = open("/proc/self/stat", O_RDONLY);
	if (fd == -1)
	{
		perror("open");
		return (pid);
	}
	bytes_read = read(fd, buffer, sizeof(buffer) - 1);
	if (bytes_read == -1)
	{
		perror("read");
		close(fd);
		return (pid);
	}
	buffer[bytes_read] = '\0';
	fill_ptr_substr(&ptr, buffer, &pid);
	close(fd);
	return (pid);
}

void	fill_ptr_substr(char **ptr, char *buffer, pid_t *pid)
{
	*ptr = buffer;
	while (**ptr != ' ' && **ptr != '\0')
		(*ptr)++;
	**ptr = '\0';
	*pid = ft_atoi(buffer);
}
