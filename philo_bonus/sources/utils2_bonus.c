/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyap <jyap@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 20:32:20 by jyap              #+#    #+#             */
/*   Updated: 2024/09/07 07:47:13 by jyap             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	sem_cleanup(t_philo *philo)
{
	char	*philo_id;
	char	*sem_name;
	int		i;

	i = -1;
	while (++i < philo->table.n_philos)
	{
		philo_id = ft_itoa(i + 1);
		sem_name = ft_strjoin("read", philo_id);
		sem_unlink(sem_name);
		free(philo_id);
		free(sem_name);
	}
	sem_close(philo->end);
	sem_close(philo->full);
	sem_close(philo->write);
	sem_close(philo->fork);
	sem_unlink("/end");
	sem_unlink("/full");
	sem_unlink("/write");
	sem_unlink("/fork");
}

/* Finishes the simulation and exits the program cleanly */
void	finish_and_exit(t_philo *philo)
{
	waitpid(-1, NULL, 0);
	sem_cleanup(philo);
	free(philo->pid);
	kill(0, SIGINT);
}

/* Waits when max_eat has reached <= 0, then finish and exit per philosopher */

void	check_stomach_and_death(t_philo *philo, t_table table)
{
	int		i;

	i = -1;
	if (philo->table.must_eat_count != -2)
	{
		philo->stomach_process = fork();
		if (philo->stomach_process != 0)
			return ;
		while (++i < table.n_philos)
			sem_wait(philo->full);
		sem_wait(philo->write);
		while (i-- > 0)
			sem_post(philo->end);
		usleep(100);
		finish_and_exit(philo);
	}
}
