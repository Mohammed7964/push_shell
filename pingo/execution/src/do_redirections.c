/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   do_redirections.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 20:07:00 by omaezzem          #+#    #+#             */
/*   Updated: 2025/06/18 09:01:22 by omaezzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	handle_output_redirection(char *file, char *redir, int *out_fd)
{
	if (*out_fd != -1)
		close(*out_fd);
	return (process_redirection_out(file, redir, out_fd));
}

static int	handle_input_redirection(char *file, char *redir,
		int heredoc, int *in_fd)
{
	if (*in_fd != -1)
		close(*in_fd);
	return (process_redirection_in(file, redir, heredoc, in_fd));
}

static int	process_single_redirection(t_redir_params *params, int i)
{
	if (ft_strcmp("/dev/stdout", params->files[i]) == 0
		|| ft_strcmp("/dev/stdin", params->files[i]) == 0)
		return (SUCCESS);
	if (ft_strcmp(params->redirections[i], ">") == 0
		|| ft_strcmp(params->redirections[i], ">>") == 0)
		return (handle_output_redirection(params->files[i],
				params->redirections[i], params->out_fd));
	else if (ft_strcmp(params->redirections[i], "<") == 0
		|| ft_strcmp(params->redirections[i], "<<") == 0)
		return (handle_input_redirection(params->files[i],
				params->redirections[i], params->heredoc, params->in_fd));
	return (SUCCESS);
}

static int	apply_file_descriptors(int out_fd, int in_fd, int heredoc)
{
	if (out_fd != -1 && dup2(out_fd, STDOUT_FILENO) == -1)
	{
		perror("minishell");
		return (close_fd_ou_in(out_fd, in_fd, heredoc), FAILURE);
	}
	if (in_fd != -1 && dup2(in_fd, STDIN_FILENO) == -1)
	{
		perror("minishell");
		return (close_fd_ou_in(out_fd, in_fd, heredoc), FAILURE);
	}
	return (SUCCESS);
}

int	ft_do_redirections(char **files, char **redirections,
		int heredoc, t_cmd *data)
{
	int				i;
	int				in_fd;
	int				out_fd;
	int				status;
	t_redir_params	params;

	in_fd = -1;
	out_fd = -1;
	status = SUCCESS;
	if (!files || !redirections)
		return (SUCCESS);
	if (ft_len_redirections(redirections) == 1)
		return (to_single_redirection(files, redirections, heredoc, data), 1);
	params = (t_redir_params){files, redirections, heredoc, &in_fd, &out_fd};
	i = 0;
	while (files[i] && redirections[i] && status == SUCCESS)
	{
		status = process_single_redirection(&params, i);
		i++;
	}
	if (status != SUCCESS)
		return (close_fd_ou_in(out_fd, in_fd, heredoc), FAILURE);
	if (apply_file_descriptors(out_fd, in_fd, heredoc) == FAILURE)
		return (FAILURE);
	return (close_fd_ou_in(out_fd, in_fd, heredoc), SUCCESS);
}
