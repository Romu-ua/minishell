/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_sb_status.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 16:07:10 by hyamamot          #+#    #+#             */
/*   Updated: 2025/12/18 16:07:11 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand_internal.h"

int	tv_sb_put_status(t_str_buf *b, int status)
{
	char	buf[32];

	if (ms_itoa_to_buf(status, buf, sizeof(buf)) < 0)
		return (-1);
	return (tv_sb_puts(b, buf));
}
