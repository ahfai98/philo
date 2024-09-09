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
		sem_name = ft_strjoin("/read", philo_id);
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
	int	i;

	i = -1;
	waitpid(-1, NULL, 0);
	if (philo->table.must_eat_count != -2)
		kill(philo->stomach_process, SIGINT);
	usleep(500);
	while (++i < philo->table.n_philos)
		kill(philo->pid[i], SIGINT);
	sem_cleanup(philo);
	free(philo->pid);
	exit(0);
}

/* Waits when max_eat has reached <= 0, then finish and exit per philosopher */

int	check_stomach(t_philo *philo, t_table table)
{
	int		i;

	i = -1;
	if (philo->table.must_eat_count != -2)
	{
		philo->stomach_process = fork();
		if (philo->stomach_process == -1)
			return (msg(STR_ERR_FORK, NULL, 1));
		if (philo->stomach_process != 0)
			return (0);
		while (++i < table.n_philos)
			sem_wait(philo->full);
		sem_wait(philo->write);
		while (i-- > 0)
			sem_post(philo->end);
		sem_close(philo->end);
		sem_close(philo->full);
		sem_close(philo->write);
		sem_close(philo->fork);
		usleep(500);
		exit(0);
	}
	return (0);
}

void	sim_starttime(t_philo *philo, t_table table)
{
	gettimeofday(&philo->start_time, NULL);
	philo->start_time.tv_usec += table.n_philos * 200;
	if (philo->start_time.tv_usec >= 1000000)
	{
		philo->start_time.tv_sec++;
		philo->start_time.tv_usec -= 1000000;
	}
}
