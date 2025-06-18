/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: omaezzem <omaezzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 01:56:18 by omaezzem          #+#    #+#             */
/*   Updated: 2025/06/18 04:21:43 by omaezzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	to_many_arg(t_cmd *data)
{
	ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
	data->ex_status = 1;
}

void	handle_exit_error(t_cmd *data, char *arg, int st, int flg)
{
	ft_putstr_fd("exit\n", STDERR_FILENO);
	ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
	data->ex_status = st;
	if (flg == 0)
		exit(st);
}
