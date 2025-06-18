/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_redir.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 02:28:13 by omaezzem          #+#    #+#             */
/*   Updated: 2025/06/18 09:01:04 by omaezzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	close_fd_ou_in(int out_fd, int in_fd, int heredoc)
{
	if (out_fd != -1)
		close(out_fd);
	if (in_fd != -1 && in_fd != heredoc)
		close(in_fd);
}

int	process_redirection_out(char *file, char *redir, int *out_fd)
{
	int	fd;

	fd = 0;
	if (ft_strcmp(redir, ">") == 0)
	{
		fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd == -1)
			return (perror("minishell"), FAILURE);
		*out_fd = fd;
	}
	else if (ft_strcmp(redir, ">>") == 0)
	{
		fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd == -1)
			return (perror("minishell"), FAILURE);
		*out_fd = fd;
	}
	return (SUCCESS);
}

int	process_redirection_in(char *file, char *redir, int heredoc_fd, int *in_fd)
{
	if (ft_strcmp(redir, "<") == 0)
	{
		*in_fd = open(file, O_RDONLY);
		if (*in_fd == -1)
			return (perror("minishell"), FAILURE);
	}
	else if (ft_strcmp(redir, "<<") == 0)
		*in_fd = heredoc_fd;
	else
		return (FAILURE);
	return (SUCCESS);
}
