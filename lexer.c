#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"

// この構造体はlexer.c内のみで使用する。
typedef struct s_str_buf
{
	char	*s;
	size_t	len;
	size_t	cap;
}	t_str_buf;

// t_token_vec ユーティリティ
static void	tv_init(t_token_vec *v)
{
	v->data = NULL;
	v->cap = 0;
	v->size = 0;
}

static void	tv_push(t_token_vec *v, t_token t)
{
	size_t	ncap;
	t_token	*nd;

	if (v->size == v->cap)
	{
		if (v->cap)
			ncap = v->cap * 2;
		else
			ncap = 8;
		nd = (t_token *)realloc(v->data, ncap * sizeof(t_token));
		if (!nd)
		{
			perror("realloc");
			exit(1);
		}
		v->data = nd;
		v->cap = ncap;
	}
	v->data[v->size++] = t;
}

void	tv_free(t_token_vec *v)
{
	size_t	i;

	i = 0;
	while (i < v->size)
	{
		if (v->data[i].kind == T_WORD || v->data[i].kind == T_ERROR)
			free(v->data[i].lexeme);
		i++;
	}
	free(v->data);
}

// t_str_buf ユーティリティ
static void	sb_init(t_str_buf *b)
{
	b->s = NULL;
	b->cap = 0;
	b->len = 0;
}

static void	sb_putc(t_str_buf *b, int c)
{
	size_t	ncap;
	char	*ns;

	if (b->len + 1 >= b->cap)
	{
		if (b->cap)
			ncap = b->cap * 2;
		else
			ncap = 32;
		ns = (char *)realloc(b->s, ncap * sizeof(char));
		if (!ns)
		{
			perror("realloc");
			exit(1);
		}
		b->cap = ncap;
		b->s = ns;
	}
	b->s[b->len++] = (char)c;
	b->s[b->len] = '\0';
}

static char	*sb_take(t_str_buf *sb)
{
	char	*r;

	r = sb->s;
	sb->s = NULL;
	sb->cap = 0;
	sb->len = 0;
	return (r);
}

static void	sb_free(t_str_buf *b)
{
	free(b->s);
}

static void	skip_space(const char **p)
{
	while (isspace((unsigned char)**p))
		(*p)++;
}

int	is_opchar(int c)
{
	if (c == '|' || c == '&' || c == '>' || c == '<')
		return (1);
	return (0);
}

int	read_quote(const char **p, t_str_buf *b, int quote)
{
	while (**p && **p != quote)
	{
		sb_putc(b, **p);
		(*p)++;
	}
	if (**p != quote)
		return (-1);
	(*p)++; // 終端のquoteを消費。
	return (0);
}

int	read_word(const char **p, t_str_buf *b)
{
	int	q;
	int	consumed;

	while (1)
	{
		if (**p == '\0' || isspace((unsigned char)**p) || is_opchar((unsigned char)**p))
			break ;

		if (**p == '\'' || **p == '"')
		{
			q = **p;
			(*p)++; //開始quoteを消費
			if (read_quote(p, b, q) != 0)
				return (-1);
			consumed = 1;
		}
		else
		{
			// quoteで囲まれていないとき
			while (**p && !isspace((unsigned char)**p) &&
					!is_opchar((unsigned char)**p) &&
					**p != '\'' && **p != '"')
			{
				sb_putc(b, **p);
				(*p)++;
				consumed = 1;
			}
		}
	}
	if (consumed)
		return (0);
	else
		return (1);
}

int	lex_line(const char *line, t_token_vec *out)
{
	const char	*p = line;
	t_str_buf	sb;
	int			r;

	tv_init(out);
	while (1)
	{
		skip_space(&p);
		// 行末
		if (*p == '\0' || *p == '\n')
		{
			tv_push(out, (t_token){ .kind = T_EOF, .lexeme = NULL});
			return (0);
		}
		// 二項演算子
		if (*p == '&' && p[1] == '&')
		{
			tv_push(out, (t_token){ .kind = T_AND_IF, .lexeme = NULL});
			p += 2;
			continue;
		}
		if (*p == '|' && p[1] == '|')
		{
			tv_push(out, (t_token){ .kind = T_OR_IF, .lexeme = NULL});
			p += 2;
			continue;
		}
		if (*p == '>' && p[1] == '>')
		{
			tv_push(out, (t_token){ .kind = T_DGT, .lexeme = NULL});
			p += 2;
			continue;
		}
		if (*p == '<' && p[1] == '<')
		{
			tv_push(out, (t_token){ .kind = T_DLT, .lexeme = NULL});
			p += 2;
			continue;
		}

		// 単項演算子
		// 単項の&はbashとしてはジョブ制御に関する演算子で、バックグラウンドで実行するために使われる
		// 単項の&はエラーとする
		if (*p == '&')
		{
			tv_push(out, (t_token){ .kind = T_ERROR, .lexeme = strdup("unexpected single '&'")});
			tv_push(out, (t_token){ .kind = T_EOF, .lexeme = NULL});
			return (-1);
		}
		if (*p == '|')
		{
			tv_push(out, (t_token){ .kind = T_PIPE, .lexeme = NULL});
			p++;
			continue;
		}
		if (*p == '>')
		{
			tv_push(out, (t_token){ .kind = T_GT, .lexeme = NULL});
			p++;
			continue;
		}
		if (*p == '<')
		{
			tv_push(out, (t_token){ .kind = T_LT, .lexeme = NULL});
			p++;
			continue;
		}

		// WORD
		sb_init(&sb);
		r = read_word(&p, &sb);
		// read_word の返り値が-1の時はquoteが閉じていないこととする
		if (r == -1)
		{
			sb_free(&sb);
			tv_push(out, (t_token){ .kind = T_ERROR, .lexeme = strdup("unclosed quote")});
			tv_push(out, (t_token){ .kind = T_EOF, .lexeme = NULL});
			return (-1);
		}
		// 1が帰るのは末尾、スペース、上記演算子であるが、これは事前に処理している。
		// ここには到達しないはずだが、一応残しておく。
		if (r == 1)
		{
			sb_free(&sb);
			tv_push(out, (t_token){ .kind = T_ERROR, .lexeme = strdup("unexpected error")});
			tv_push(out, (t_token){ .kind = T_EOF, .lexeme = NULL});
			return (-1);
		}
		tv_push(out, (t_token){ .kind = T_WORD, .lexeme = sb_take(&sb)});
	}
}


//// DEBUG用
//static void	print_tokens(t_token_vec *v)
//{
//	for (size_t i = 0; i < v->size; i++ )
//	{
//		const t_token	*t = &v->data[i];
//		switch (t->kind)
//		{
//			case T_WORD: printf("T_WORD(\"%s\")\n", t->lexeme); break;
//			case T_PIPE:   printf("T_PIPE(|)\n"); break;
//			case T_OR_IF:  printf("T_OR_IF(||)\n"); break;
//			case T_AND_IF: printf("T_AND_IF(&&)\n"); break;
//			case T_LT:     printf("T_LT(<)\n"); break;
//			case T_GT:     printf("T_GT(>)\n"); break;
//			case T_DLT:    printf("T_DLT(<<)\n"); break;
//			case T_DGT:    printf("T_DGT(>>)\n"); break;
//			case T_EOF:    printf("T_EOF\n"); break;
//			case T_ERROR:  printf("T_ERROR(%s)\n", t->lexeme ? t->lexeme:""); break;
//		}
//	}
//}

//int	main(void)
//{
//	char		buf[4096];
//	t_token_vec	tv;
//	int			ok;

//	printf("lexer> ");
//	while (fgets(buf, sizeof(buf), stdin))
//	{
//		if (lex_line(buf, &tv) == 0)
//			ok = 1;
//		else
//			ok = 0;
//		if (!ok)
//			puts("[lexer error]");
//		print_tokens(&tv);
//		tv_free(&tv);
//		printf("lexer> ");
//	}
//	return (0);
//}
