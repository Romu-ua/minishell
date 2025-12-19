/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_utils_alloc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 16:11:15 by hyamamot          #+#    #+#             */
/*   Updated: 2025/12/18 16:11:16 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_utils.h"
#include <stdlib.h>

void	*ms_calloc(size_t nmemb, size_t size)
{
	void	*p;
	size_t	total;

	if (nmemb == 0 || size == 0)
		return (malloc(1));
	total = nmemb * size;
	p = malloc(total);
	if (!p)
		return (NULL);
	ms_bzero(p, total);
	return (p);
}

void	*ms_realloc(void *ptr, size_t oldsz, size_t newsz)
{
	void	*np;
	size_t	cp;

	if (newsz == 0)
	{
		free(ptr);
		return (NULL);
	}
	np = malloc(newsz);
	if (!np)
		return (NULL);
	cp = oldsz;
	if (cp > newsz)
		cp = newsz;
	if (ptr && cp)
		ms_memcpy(np, ptr, cp);
	free(ptr);
	return (np);
}

char	*ms_strdup(const char *s)
{
	size_t	len;
	char	*r;

	if (!s)
		s = "";
	len = ms_strlen(s);
	r = (char *)malloc(len + 1);
	if (!r)
		return (NULL);
	ms_memcpy(r, s, len);
	r[len] = '\0';
	return (r);
}

char	*ms_strndup(const char *s, size_t n)
{
	size_t	i;
	char	*r;

	if (!s)
		s = "";
	r = (char *)malloc(n + 1);
	if (!r)
		return (NULL);
	i = 0;
	while (i < n && s[i])
	{
		r[i] = s[i];
		i++;
	}
	r[i] = '\0';
	return (r);
}
