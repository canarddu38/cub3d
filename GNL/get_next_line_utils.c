/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julcleme <julcleme@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 00:53:49 by julcleme          #+#    #+#             */
/*   Updated: 2025/11/16 00:19:53 by julcleme         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

size_t	ft_strlen(char *str)
{
	size_t	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
		i++;
	return (i);
}

void	clear_buffer(char buffer[BUFFER_SIZE + 1])
{
	size_t	i;

	i = 0;
	while (buffer && i < BUFFER_SIZE + 1)
	{
		buffer[i] = '\0';
		i++;
	}
}

char	*ft_strcat_realloc(char *s1, char *s2, size_t n)
{
	size_t	s1_len;
	size_t	i;
	char	*new;

	s1_len = ft_strlen(s1);
	new = malloc(s1_len + n + 1);
	if (!new)
		return (0);
	i = 0;
	while (s1 && s1[i])
	{
		new[i] = s1[i];
		i++;
	}
	if (s1)
		free(s1);
	s1 = 0;
	while (n-- > 0 && s2 && *s2)
		new[i++] = *(s2++);
	new[i] = '\0';
	return (new);
}

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	long	i;
	int		copy_direction;

	i = 0;
	if (!dest && !src)
		return (0);
	copy_direction = 1;
	if (src < dest)
	{
		i = (long)n - 1;
		copy_direction = -1;
	}
	while (i < (long)n && i >= 0)
	{
		((char *)dest)[i] = ((char *)src)[i];
		i += copy_direction;
	}
	return (dest);
}
