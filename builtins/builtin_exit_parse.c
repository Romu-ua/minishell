/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit_parse.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 16:01:01 by hyamamot          #+#    #+#             */
/*   Updated: 2025/12/18 16:01:02 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_internal.h"

static size_t	skip_spaces(const char *s, size_t i)
{
	while (s[i] && ms_isspace((unsigned char)s[i]))
		i++;
	return (i);
}

static long	read_sign(const char *s, size_t *i)
{
	long	sign;

	sign = 1;
	if (s[*i] == '+' || s[*i] == '-')
	{
		if (s[*i] == '-')
			sign = -1;
		(*i)++;
	}
	return (sign);
}

long	bi_exit_atol(const char *s, int *ok)
{
	size_t	i;
	long	sign;
	long	val;

	*ok = 1;
	i = skip_spaces(s, 0);
	sign = read_sign(s, &i);
	if (!ms_isdigit((unsigned char)s[i]))
		return (*ok = 0, 0);
	val = 0;
	while (ms_isdigit((unsigned char)s[i]))
		val = val * 10 + (s[i++] - '0');
	i = skip_spaces(s, i);
	if (s[i] != '\0')
		*ok = 0;
	return (sign * val);
}
