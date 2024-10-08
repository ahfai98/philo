/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyap <jyap@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 20:32:16 by jyap              #+#    #+#             */
/*   Updated: 2024/09/07 07:48:14 by jyap             ###   ########.fr       */
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
			get_message(philo, philo->n, "died", 1);
			while (philo->table.n_philos-- > 0)
				sem_post(philo->end);
			break ;
		}
		ft_usleep(7);
	}
	pthread_join(philo->cleanup_thread, NULL);
	return (NULL);
}

static void	*cleanup(void *args)
{
	t_philo	*philo;

	philo = args;
	sem_wait(philo->end);
	sem_close(philo->read);
	sem_close(philo->full);
	sem_close(philo->write);
	sem_close(philo->end);
	sem_close(philo->fork);
	free(philo->pid);
	exit(0);
}

/* Resets philosopher's last ate time and reduce its max eat by one each eat */
static void	eat_sleep(t_philo *philo)
{
	sem_wait(philo->fork);
	get_message(philo, philo->n, "has taken a fork", 0);
	sem_wait(philo->fork);
	get_message(philo, philo->n, "has taken a fork", 0);
	sem_wait(philo->read);
	gettimeofday(&philo->last_ate, NULL);
	sem_post(philo->read);
	get_message(philo, philo->n, "is eating", 0);
	ft_usleep(philo->table.time_to_eat);
	if (philo->table.must_eat_count != -2)
	{
		philo->table.must_eat_count--;
		if (philo->table.must_eat_count <= 0)
			sem_post(philo->full);
	}
	sem_post(philo->fork);
	sem_post(philo->fork);
	get_message(philo, philo->n, "is sleeping", 0);
	ft_usleep(philo->table.time_to_sleep);
	get_message(philo, philo->n, "is thinking", 0);
}

static void	think_before_eat(t_philo *philo)
{
	int	time_to_think;

	sem_wait(philo->read);
	time_to_think = ((int)philo->table.time_to_die
			- (get_time(philo->last_ate)) - philo->table.time_to_eat) / 4;
	sem_post(philo->read);
	if (time_to_think > 200)
		time_to_think = 200;
	if (time_to_think < 0)
		time_to_think = 0;
	ft_usleep(time_to_think);
}

/* The routine each philosopher has to go through for the rest of their lives */
void	routine(t_philo *philo)
{
	pthread_create(&philo->death_thread, NULL, &check_death, philo);
	pthread_detach(philo->death_thread);
	pthread_create(&philo->cleanup_thread, NULL, &cleanup, philo);
	sim_start_delay(get_time_in_ms(&philo->start_time));
	if (philo->n % 2 == 0)
		ft_usleep(10);
	while (1)
	{
		eat_sleep(philo);
		think_before_eat(philo);
	}
	pthread_join(philo->cleanup_thread, NULL);
}
