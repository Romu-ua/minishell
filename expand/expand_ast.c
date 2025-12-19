/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_ast.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 16:06:27 by hyamamot          #+#    #+#             */
/*   Updated: 2025/12/18 16:06:27 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand_internal.h"

int	expand_ast(t_ast *node)
{
	if (!node)
		return (0);
	if (node->kind == N_COMMAND)
		return (expand_cmd(&node->cmd));
	if (node->lhs)
	{
		if (expand_ast(node->lhs) < 0)
			return (-1);
	}
	if (node->rhs)
	{
		if (expand_ast(node->rhs) < 0)
			return (-1);
	}
	return (0);
}
