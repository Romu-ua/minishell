/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fatal_state.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 16:06:11 by hyamamot          #+#    #+#             */
/*   Updated: 2025/12/18 16:06:11 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

static t_ast	**ast_slot(void)
{
	static t_ast	*root = NULL;

	return (&root);
}

void	ms_register_ast(t_ast *root)
{
	*ast_slot() = root;
}

t_ast	**ast_slot_ref(void)
{
	return (ast_slot());
}
