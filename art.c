/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   art.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjamil <mjamil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 12:58:50 by mjamil            #+#    #+#             */
/*   Updated: 2025/01/23 16:30:31 by mjamil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	art(void)
{
	printf("\033[1;37m%s\033[0m\n", "");
	printf("\033[1;37m%s\033[0m\n", "");
	printf("\033[1;37m%s\033[0m\n", "");
	printf("\033[38;5;208m%s\033[0m", "███╗   ███╗██╗███╗   ██╗██╗");
	printf("\033[1;37m%s\033[0m", "███████╗");
	printf("\033[38;5;208m%s\n", "██╗  ██╗███████╗██╗     ██╗");
	printf("\033[38;5;208m%s\033[0m", "████╗ ████║██║████╗  ██║██║");
	printf("\033[1;37m%s\033[0m", "██╔════╝");
	printf("\033[38;5;208m%s\n", "██║  ██║██╔════╝██║     ██║");
	printf("\033[1;37m%s\033[0m", "██╔████╔██║██║██╔██╗ ██║██║");
	printf("\033[1;37m%s\033[0m", "███████╗");
	printf("\033[1;37m%s\n", "███████║█████╗  ██║     ██║");
	printf("\033[38;5;208m%s\033[0m", "██║╚██╔╝██║██║██║╚██╗██║██║");
	printf("\033[1;37m%s\033[0m", "╚════██║");
	printf("\033[38;5;208m%s\n", "██╔══██║██╔══╝  ██║     ██║");
	printf("\033[38;5;208m%s\033[0m", "██║ ╚═╝ ██║██║██║ ╚████║██║");
	printf("\033[1;37m%s\033[0m", "███████║");
	printf("\033[38;5;208m%s\n", "██║  ██║███████╗███████╗███████╗");
	printf("\033[38;5;208m%s\033[0m", "╚═╝     ╚═╝╚═╝╚═╝  ╚═══╝╚═╝");
	printf("\033[1;37m%s\033[0m", "╚══════╝");
	printf("\033[38;5;208m%s\n", "╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝");
	printf("\033[38;5;208m%s\033[0m\n", "      		by Ahmad & Jamil     ");
	printf("\033[1;37m%s\033[0m\n", "");
	printf("\033[1;32m%s", "\"No leaks allowed ");
	printf("our shell is watertight!\033[0m\n");
}
