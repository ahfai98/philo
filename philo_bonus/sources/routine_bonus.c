/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyap <jyap@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 20:32:16 by jyap              #+#    #+#             */
/*   Updated: 2024/08/29 22:13:33 by jyap             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

/* Prints and exits the program if the philosopher is dead */
static void	*check_death(void *args)
{
	int		now;
	t_philo	*philo;

	philo = args;
	sim_start_delay(get_time_in_ms(&philo->start_time));
	while (1)
	{
		sem_wait(philo->read);
		now = get_time(philo->last_ate);
		sem_post(philo->read);
		if (now > philo->table.time_to_die)
		{
			get_message(philo, philo->n, "died");
			break ;
		}
		ft_usleep(100);
	}
	finish_and_exit(philo);
	return (NULL);
}

/* Resets philosopher's last ate time and reduce its max eat by one each eat */
static void	eat(t_philo *philo)
{
	sem_wait(philo->fork);
	get_message(philo, philo->n, "has taken a fork");
	sem_wait(philo->fork);
	get_message(philo, philo->n, "has taken a fork");
	sem_wait(philo->read);
	gettimeofday(&philo->last_ate, NULL);
	sem_post(philo->read);
	get_message(philo, philo->n, "is eating");
	if (philo->table.must_eat_count != -2)
	{
		philo->table.must_eat_count--;
		if (philo->table.must_eat_count <= 0)
			sem_post(philo->full);
	}
	ft_usleep(philo->table.time_to_eat);
}

/* The routine each philosopher has to go through for the rest of their lives */
void	routine(t_philo *philo)
{
	char	*sem_name;
	char	*philo_id;

	philo_id = ft_itoa(philo->n);
	sem_name = ft_strjoin("read", philo_id);
	create_sema(&philo->read, sem_name, 1);
	free(philo_id);
	free(sem_name);
	pthread_create(&philo->death_thread, NULL, &check_death, philo);
	pthread_detach(philo->death_thread);
	sim_start_delay(get_time_in_ms(&philo->start_time));
	if (philo->n % 2 == 0)
		ft_usleep(1);
	while (1)
	{
		eat(philo);
		sem_post(philo->fork);
		sem_post(philo->fork);
		get_message(philo, philo->n, "is sleeping");
		ft_usleep(philo->table.time_to_sleep);
		get_message(philo, philo->n, "is thinking");
	}
}

/* Waits when max_eat has reached <= 0, then finish and exit per philosopher */
void	check_stomach(t_philo *philo, t_table table)
{
	int		i;

	if (philo->table.must_eat_count != -2)
	{
		philo->stomach_process = fork();
		if (philo->stomach_process != 0)
			return ;
		i = -1;
		while (i++ < table.n_philos)
			sem_wait(philo->full);
		sem_wait(philo->write);
		finish_and_exit(philo);
	}
}

/* Finishes the simulation and exits the program cleanly */
void	finish_and_exit(t_philo *philo)
{
	waitpid(-1, NULL, 0);
	free(philo->pid);
	kill(0, SIGINT);
}
