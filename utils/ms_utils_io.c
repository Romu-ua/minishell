/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_utils_io.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 16:11:39 by hyamamot          #+#    #+#             */
/*   Updated: 2025/12/18 16:11:40 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_utils.h"
#include <unistd.h>

void	ms_putstr_fd(const char *s, int fd)
{
	if (!s)
		return ;
	write(fd, s, ms_strlen(s));
}
