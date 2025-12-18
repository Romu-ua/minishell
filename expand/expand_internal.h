/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_internal.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 16:07:04 by hyamamot          #+#    #+#             */
/*   Updated: 2025/12/18 16:07:04 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPAND_INTERNAL_H
# define EXPAND_INTERNAL_H

# include <stddef.h>
# include "exec.h"
# include "lexer.h"
# include "parser.h"

typedef struct s_exp
{
	const char	*src;
	size_t		i;
	int			in_s;
	int			in_d;
	t_str_buf	*b;
}	t_exp;

void	tv_sb_init(t_str_buf *b);
void	tv_sb_free(t_str_buf *b);
int		tv_sb_reserve(t_str_buf *b, size_t add);
int		tv_sb_putc(t_str_buf *b, char c);
int		tv_sb_puts(t_str_buf *b, const char *s);
int		tv_sb_put_status(t_str_buf *b, int status);
void	tv_sv_init(t_str_vec *v);
int		tv_sv_push(t_str_vec *v, char *s);
char	**tv_sv_take_null_terminated(t_str_vec *v);
void	tv_sv_free_all(t_str_vec *v);
void	tv_free_argv(char **argv);
int		exp_dollar(t_exp *e);
int		expand_word(const char *src, char **out);
char	*expand_heredoc_line(const char *src);
int		expand_cmd(t_cmd *cmd);
int		expand_ast(t_ast *node);

#endif
