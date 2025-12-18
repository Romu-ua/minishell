/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_utils_itoa.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 16:11:45 by hyamamot          #+#    #+#             */
/*   Updated: 2025/12/18 16:11:45 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_utils.h"
#include <stddef.h>

static int	ms_write_zero(char *buf, size_t bufsz)
{
	if (!buf || bufsz < 2)
		return (-1);
	buf[0] = '0';
	buf[1] = '\0';
	return (0);
}

static long	ms_abs_long(int v, int *neg)
{
	long	n;

	n = (long)v;
	*neg = 0;
	if (n < 0)
	{
		*neg = 1;
		n = -n;
	}
	return (n);
}

static size_t	ms_build_rev(long n, char *tmp)
{
	size_t	i;

	i = 0;
	while (n > 0 && i < 31)
	{
		tmp[i] = (char)('0' + (n % 10));
		i++;
		n = n / 10;
	}
	return (i);
}

static int	ms_write_rev(char *buf, size_t bufsz, char *tmp, size_t len)
{
	size_t	j;

	if (!buf || len + 1 > bufsz)
		return (-1);
	j = 0;
	while (len > 0)
	{
		buf[j] = tmp[len - 1];
		j++;
		len--;
	}
	buf[j] = '\0';
	return (0);
}

int	ms_itoa_to_buf(int v, char *buf, size_t bufsz)
{
	char	tmp[32];
	long	n;
	size_t	len;
	int		neg;

	if (!buf || bufsz == 0)
		return (-1);
	if (v == 0)
		return (ms_write_zero(buf, bufsz));
	n = ms_abs_long(v, &neg);
	len = ms_build_rev(n, tmp);
	if (neg && len < 31)
	{
		tmp[len] = '-';
		len++;
	}
	return (ms_write_rev(buf, bufsz, tmp, len));
}
