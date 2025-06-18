/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_utilis.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-badd <mel-badd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 05:16:29 by mel-badd          #+#    #+#             */
/*   Updated: 2025/06/17 00:13:14 by mel-badd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	is_end_of_heredoc(char *line, char *delim)
{
	return (!line || ft_strcmp(line, delim) == 0);
}

int	should_expand_line(char *line, int expand_flag)
{
	return (expand_flag && ft_strchr(line, '$'));
}

int	count(const char *s, int *flag, int *b_flag)
{
	int	cnt;

	*flag = 0;
	cnt = 0;
	while (s[cnt] == '$')
		cnt++;
	if (cnt > 0 && (s[cnt] != '"' && s[cnt] != '\'') && *b_flag)
		*flag = 1;
	return (cnt);
}

void	clear_fd(void)
{
	int	i;

	i = 3;
	while (i < 1024)
	{
		close(i);
		i++;
	}
}
