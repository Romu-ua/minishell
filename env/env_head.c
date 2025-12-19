/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_head.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 16:04:08 by hyamamot          #+#    #+#             */
/*   Updated: 2025/12/18 16:04:09 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

static t_env	**env_head(void)
{
	static t_env	*head = NULL;

	return (&head);
}

t_env	*env_list(void)
{
	return (*env_head());
}

t_env	**env_head_ref(void)
{
	return (env_head());
}
