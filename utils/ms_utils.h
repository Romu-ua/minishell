/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_utils.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hyamamot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 16:12:14 by hyamamot          #+#    #+#             */
/*   Updated: 2025/12/18 16:12:15 by hyamamot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MS_UTILS_H
# define MS_UTILS_H

# include <stddef.h>

typedef struct s_itoa
{
	char	tmp[32];
	long	n;
	size_t	i;
	size_t	j;
	int		neg;
}	t_itoa;

size_t	ms_strlen(const char *s);
int		ms_strcmp(const char *a, const char *b);
int		ms_strncmp(const char *a, const char *b, size_t n);
char	*ms_strchr(const char *s, int c);
void	*ms_memcpy(void *dst, const void *src, size_t n);

void	ms_bzero(void *p, size_t n);
void	*ms_calloc(size_t nmemb, size_t size);
void	*ms_realloc(void *ptr, size_t oldsz, size_t newsz);

char	*ms_strdup(const char *s);
char	*ms_strndup(const char *s, size_t n);

int		ms_isalpha(int c);
int		ms_isalnum(int c);
int		ms_isdigit(int c);
int		ms_isspace(int c);

int		ms_itoa_to_buf(int v, char *buf, size_t bufsz);

void	ms_putstr_fd(const char *s, int fd);

#endif
