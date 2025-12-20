/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 16:08:47 by hyamamot          #+#    #+#             */
/*   Updated: 2025/12/18 16:08:48 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "ms_utils.h"
#include <ctype.h>

void	skip_space(const char **p)
{
	while (ms_isspace((unsigned char)**p))
		(*p)++;
}

int	is_opchar(int c)
{
	if (c == '|' || c == '&' || c == '>' || c == '<')
		return (1);
	return (0);
}
