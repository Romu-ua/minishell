/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_utils_str.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 16:12:06 by hyamamot          #+#    #+#             */
/*   Updated: 2025/12/18 16:12:07 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_utils.h"
#include <stdlib.h>

size_t	ms_strlen(const char *s)
{
	size_t	i;

	i = 0;
	if (!s)
		return (0);
	while (s[i])
		i++;
	return (i);
}

int	ms_strcmp(const char *a, const char *b)
{
	size_t			i;
	unsigned char	ca;
	unsigned char	cb;

	if (!a || !b)
		return (a != b);
	i = 0;
	while (a[i] || b[i])
	{
		ca = (unsigned char)a[i];
		cb = (unsigned char)b[i];
		if (ca != cb)
			return (ca - cb);
		i++;
	}
	return (0);
}

int	ms_strncmp(const char *a, const char *b, size_t n)
{
	size_t			i;
	unsigned char	ca;
	unsigned char	cb;

	if (n == 0)
		return (0);
	if (!a || !b)
		return (a != b);
	i = 0;
	while (i < n && (a[i] || b[i]))
	{
		ca = (unsigned char)a[i];
		cb = (unsigned char)b[i];
		if (ca != cb)
			return (ca - cb);
		i++;
	}
	return (0);
}

char	*ms_strchr(const char *s, int c)
{
	size_t	i;

	if (!s)
		return (NULL);
	i = 0;
	while (1)
	{
		if (s[i] == (char)c)
			return ((char *)(s + i));
		if (s[i] == '\0')
			break ;
		i++;
	}
	return (NULL);
}
