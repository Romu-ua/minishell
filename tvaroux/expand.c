/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomvaroux <tomvaroux@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 10:38:08 by tomvaroux         #+#    #+#             */
/*   Updated: 2025/12/10 17:26:06 by tomvaroux        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

/* ===== 伸びる文字バッファ ===== */

// typedef struct s_str_buf
// {
// 	char	*s;
// 	size_t	len;
// 	size_t	cap;
// }	t_str_buf;
#include "../lexer.h"


static void	tv_sb_init(t_str_buf *b);
static void	tv_sb_free(t_str_buf *b);
static int	tv_sb_reserve(t_str_buf *b, size_t add);
static int	tv_sb_putc(t_str_buf *b, char c);
static int	tv_sb_puts(t_str_buf *b, const char *s);
static int	tv_sb_put_status(t_str_buf *b, int status);

static int	is_var_start(int c);
static int	is_var_body(int c);

int	expand_word(const char *src, char **out);


static void	tv_sb_init(t_str_buf *b)
{
	b->s = NULL;
	b->len = 0;
	b->cap = 0;
}

static void	tv_sb_free(t_str_buf *b)
{
	free(b->s);
	b->s = NULL;
	b->len = 0;
	b->cap = 0;
}

static int	tv_sb_reserve(t_str_buf *b, size_t add)
{
	size_t	ncap;
	char	*ns;

	if (b->len + add + 1 <= b->cap)
		return (0);
	if (!b->cap)
		ncap = 32;
	else
		ncap = b->cap * 2;
	while (b->len + add + 1 > ncap)
		ncap *= 2;
	ns = (char *)realloc(b->s, ncap);
	if (!ns)
		return (-1);
	b->s = ns;
	b->cap = ncap;
	return (0);
}

static int	tv_sb_putc(t_str_buf *b, char c)
{
	if (tv_sb_reserve(b, 1) < 0)
		return (-1);
	b->s[b->len++] = c;
	b->s[b->len] = '\0';
	return (0);
}

static int	tv_sb_puts(t_str_buf *b, const char *s)
{
	size_t	len;

	if (!s)
		return (0);
	len = strlen(s);
	if (tv_sb_reserve(b, len) < 0)
		return (-1);
	memcpy(b->s + b->len, s, len);
	b->len += len;
	b->s[b->len] = '\0';
	return (0);
}

static int	tv_sb_put_status(t_str_buf *b, int status)
{
	char	buf[32];
	int		n;

	n = snprintf(buf, sizeof(buf), "%d", status);
	if (n < 0)
		return (-1);
	if (n >= (int)sizeof(buf))
		n = (int)sizeof(buf) - 1;
	buf[n] = '\0';
	return (tv_sb_puts(b, buf));
}

/* ===== $VAR / $? 展開用ヘルパ ===== */

static int	is_var_start(int c)
{
	return (isalpha((unsigned char)c) || c == '_');
}

static int	is_var_body(int c)
{
	return (isalnum((unsigned char)c) || c == '_');
}

/*
 * expand_word:
 *   - src: lexer の WORD (クォート込み)
 *   - out: 展開後の文字列（malloc 済み）
 * シングルクォート: メタを完全無効
 * ダブルクォート: $ だけ有効
 * それ以外: $VAR, $? を展開
 * 未対応: word splitting / $1 など
 */

int	expand_word(const char *src, char **out)
{
	t_str_buf	buf;
	int			in_single;
	int			in_double;
	size_t		i;

	if (!src)
	{
		*out = NULL;
		return (0);
	}
	tv_sb_init(&buf);
	in_single = 0;
	in_double = 0;
	i = 0;
	while (src[i])
	{
		if (src[i] == '\'' && !in_double)
		{
			in_single = !in_single;
			i++;
			continue;
		}
		if (src[i] == '"' && !in_single)
		{
			in_double = !in_double;
			i++;
			continue;
		}
		if (src[i] == '$' && !in_single)
		{
			char	next = src[i + 1];

			if (next == '\0')
			{
				if (tv_sb_putc(&buf, '$') < 0)
					goto error;
				i++;
				continue;
			}
			if (next == '?')
			{
				if (tv_sb_put_status(&buf, g_exit_status) < 0)
					goto error;
				i += 2;
				continue;
			}
			if (is_var_start(next))
			{
				size_t	start = i + 1;
				size_t	j = start;
				char	name[256];
				size_t	nlen;
				const char	*val;

				while (is_var_body(src[j]))
					j++;
				nlen = j - start;
				if (nlen >= sizeof(name))
					nlen = sizeof(name) - 1;
				memcpy(name, src + start, nlen);
				name[nlen] = '\0';
				val = env_get(name);
				if (val && tv_sb_puts(&buf, val) < 0)
					goto error;
				i = j;
				continue;
			}
			if (tv_sb_putc(&buf, '$') < 0)
				goto error;
			i++;
			continue;
		}
		if (tv_sb_putc(&buf, src[i]) < 0)
			goto error;
		i++;
	}
	if (!buf.s)
	{
		buf.s = (char *)malloc(1);
		if (!buf.s)
			return (-1);
		buf.s[0] = '\0';
		buf.len = 0;
		buf.cap = 1;
	}
	*out = buf.s;
	return (0);
error:
	tv_sb_free(&buf);
	*out = NULL;
	return (-1);
}


















/* ===== expand_cmd / expand_ast ===== */

// typedef struct s_str_vec
// {
// 	char	**data;
// 	size_t	size;
// 	size_t	cap;
// }	t_str_vec;
#include "../parser.h"


static void	tv_sv_init(t_str_vec *v);
static void	tv_sv_push(t_str_vec *v, char *s);
static char	**tv_sv_take_null_terminated(t_str_vec *v);
static void	free_argv_local(char **argv);

int			expand_cmd(t_cmd *cmd);
int			expand_ast(t_ast *node);
char		*expand_heredoc_line(const char *src);


static void	tv_sv_init(t_str_vec *v)
{
	v->data = NULL;
	v->size = 0;
	v->cap = 0;
}

static void	tv_sv_push(t_str_vec *v, char *s)
{
	char	**nd;
	size_t	ncap;

	if (v->size == v->cap)
	{
		if (v->cap)
			ncap = v->cap * 2;
		else
			ncap = 8;
		nd = (char **)realloc(v->data, ncap * sizeof(char *));
		if (!nd)
			fatal("realloc");
		v->data = nd;
		v->cap = ncap;
	}
	v->data[v->size++] = s;
}

static char	**tv_sv_take_null_terminated(t_str_vec *v)
{
	char	**r;

	tv_sv_push(v, NULL);
	r = v->data;
	v->data = NULL;
	v->size = 0;
	v->cap = 0;
	return (r);
}

static void	free_argv_local(char **argv)
{
	size_t	i;

	if (!argv)
		return ;
	i = 0;
	while (argv[i])
	{
		free(argv[i]);
		i++;
	}
	free(argv);
}

/* 1 コマンド分の argv + redir の展開 */

int	expand_cmd(t_cmd *cmd)
{
	t_str_vec	argv;
	size_t		i;
	char		*expanded;
	t_redir		*r;

	tv_sv_init(&argv);
	if (cmd->argv)
	{
		i = 0;
		while (cmd->argv[i])
		{
			if (expand_word(cmd->argv[i], &expanded) < 0)
				fatal("expand_word");
			tv_sv_push(&argv, expanded);
			i++;
		}
	}
	free_argv_local(cmd->argv);
	cmd->argv = tv_sv_take_null_terminated(&argv);
	r = cmd->redirs;
	while (r)
	{
		if (r->type != R_HERDOC)
		{
			if (expand_word(r->target, &expanded) < 0)
				fatal("expand_word");
			free(r->target);
			r->target = expanded;
		}
		/* R_HERDOC の target は delimiter。
		   クォート付きかどうかを apply_redirs() 側で見たいので、
		   ここではいじらない。 */
		r = r->next;
	}
	return (0);
}

int	expand_ast(t_ast *node)
{
	if (!node)
		return (0);
	if (node->kind == N_COMMAND)
		return (expand_cmd(&node->cmd));
	if (node->lhs && expand_ast(node->lhs) != 0)
		return (-1);
	if (node->rhs && expand_ast(node->rhs) != 0)
		return (-1);
	return (0);
}

/* ===== heredoc 用: 行単位で $ 展開だけする ===== */
/* クォートは特別扱いしないで、そのまま文字として扱う */

char	*expand_heredoc_line(const char *src)
{
	t_str_buf	buf;
	size_t		i;

	if (!src)
		return (NULL);
	tv_sb_init(&buf);
	i = 0;
	while (src[i])
	{
		if (src[i] == '$')
		{
			char	next = src[i + 1];

			if (next == '\0')
			{
				if (tv_sb_putc(&buf, '$') < 0)
					goto error;
				i++;
				continue;
			}
			if (next == '?')
			{
				if (tv_sb_put_status(&buf, g_exit_status) < 0)
					goto error;
				i += 2;
				continue;
			}
			if (is_var_start(next))
			{
				size_t	start = i + 1;
				size_t	j = start;
				char	name[256];
				size_t	nlen;
				const char	*val;

				while (is_var_body(src[j]))
					j++;
				nlen = j - start;
				if (nlen >= sizeof(name))
					nlen = sizeof(name) - 1;
				memcpy(name, src + start, nlen);
				name[nlen] = '\0';
				val = env_get(name);
				if (val && tv_sb_puts(&buf, val) < 0)
					goto error;
				i = j;
				continue;
			}
			if (tv_sb_putc(&buf, '$') < 0)
				goto error;
			i++;
			continue;
		}
		if (tv_sb_putc(&buf, src[i]) < 0)
			goto error;
		i++;
	}
	if (!buf.s)
	{
		buf.s = (char *)malloc(1);
		if (!buf.s)
			return (NULL);
		buf.s[0] = '\0';
	}
	return (buf.s);
error:
	tv_sb_free(&buf);
	return (NULL);
}
