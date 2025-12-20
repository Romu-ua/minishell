/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_open.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 16:11:04 by hyamamot          #+#    #+#             */
/*   Updated: 2025/12/18 16:11:05 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "redirect_internal.h"

int	rd_open_in(const char *path)
{
	int	fd;

	fd = open(path, O_RDONLY);
	return (fd);
}

int	rd_open_out_trunc(const char *path)
{
	int	fd;

	fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	return (fd);
}

int	rd_open_out_app(const char *path)
{
	int	fd;

	fd = open(path, O_WRONLY | O_CREAT | O_APPEND, 0644);
	return (fd);
}
