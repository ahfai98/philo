/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyap <jyap@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 20:31:54 by jyap              #+#    #+#             */
/*   Updated: 2024/08/29 22:08:13 by jyap             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

/* Initializes the values into the struct based on the user table */
void	init_arg(t_table *table, int ac, char **av)
{
	table->n_philos = ft_atoi(av[1]);
	table->time_to_die = ft_atoi(av[2]);
	table->time_to_eat = ft_atoi(av[3]);
	table->time_to_sleep = ft_atoi(av[4]);
	table->must_eat_count = -2;
	if (ac == 6)
		table->must_eat_count = ft_atoi(av[5]);
}

/* Unlinks, closes and reopen a new semaphore */
int	create_sema(sem_t **sem, char *name, int count)
{
	sem_unlink(name);
	sem_close(*sem);
	*sem = NULL;
	*sem = sem_open(name, O_CREAT, S_IRWXU, count);
	return (*sem == SEM_FAILED);
}

/* Initializes the semaphore needed for the routine */
static int	init_sema(t_philo *philo, t_table table)
{
	int	ret;

	ret = 0;
	ret += create_sema(&philo->fork, "fork", table.n_philos);
	ret += create_sema(&philo->full, "full", 0);
	ret += create_sema(&philo->write, "write", 1);
	ret += create_sema(&philo->end, "end", 0);
	return (ret);
}

/* Initializes the philosopher's value and starts the routine */
int	init_philo(t_philo *philo, t_table table)
{
	if (init_sema(philo, table))
		return (msg(STR_ERR_SEM, NULL, 1));
	gettimeofday(&philo->start_time, NULL);
	philo->start_time.tv_usec += table.n_philos * 20;
	if (philo->start_time.tv_usec >= 1000000)
	{
		philo->start_time.tv_sec++;
		philo->start_time.tv_usec -= 1000000;
	}
	philo->last_ate = philo->start_time;
	philo->table = table;
	philo->n = 1;
	while (philo->n <= table.n_philos)
	{
		philo->pid[philo->n] = fork();
		if (philo->pid[philo->n] == -1)
			return (msg(STR_ERR_FORK, NULL, 1));
		if (philo->pid[philo->n] == 0)
			routine(philo);
		philo->n++;
	}
	if (philo->table.must_eat_count != -2)
	{
		check_stomach_and_death(philo, table);
		return (0);
	}
	waitpid(-1, NULL, 0);
	sem_cleanup(philo);
	free(philo->pid);
	kill(0, SIGINT);
	return (0);
}
