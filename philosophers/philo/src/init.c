/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyap <jyap@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/03 11:35:04 by mcombeau          #+#    #+#             */
/*   Updated: 2024/08/22 23:19:45 by jyap             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static pthread_mutex_t	*init_forks(t_table *t)
{
	pthread_mutex_t	*forks;
	int	i;

	forks = malloc(sizeof(pthread_mutex_t) * t->n_philos);
	if (!forks)
		return (error_null(STR_ERR_MALLOC, NULL, 0));
	i = 0;
	while (i < t->n_philos)
	{
		if (pthread_mutex_init(&forks[i], 0) != 0)
		{
			while (--i >= 0)
				pthread_mutex_destroy(&forks[i]);
			return (error_null(STR_ERR_MUTEX, NULL, 0));
		}
		i++;
	}
	return (forks);
}

static void	assign_forks(t_philo *philo)
{
	philo->fork[0] = philo->id;
	philo->fork[1] = (philo->id + 1) % philo->table->n_philos;
	if (philo->id % 2)
	{
		philo->fork[0] = (philo->id + 1) % philo->table->n_philos;
		philo->fork[1] = philo->id;
	}
}

static t_philo	**init_philosophers(t_table *t)
{
	t_philo			**philos;
	int	i;

	philos = malloc(sizeof(t_philo *) * t->n_philos);
	if (!philos)
		return (error_null(STR_ERR_MALLOC, NULL, 0));
	i = 0;
	while (i < t->n_philos)
	{
		philos[i] = malloc(sizeof(t_philo) * 1);
		if (!philos[i])
			return (error_null(STR_ERR_MALLOC, NULL, 0));
		if (pthread_mutex_init(&philos[i]->meal_time_lock, 0) != 0)
		{
			while (--i >= 0)
				pthread_mutex_destroy(&philos[i]->meal_time_lock);
			return (error_null(STR_ERR_MUTEX, NULL, 0));
		}
		philos[i]->table = t;
		philos[i]->id = i;
		philos[i]->times_ate = 0;
		assign_forks(philos[i]);
		i++;
	}
	return (philos);
}

static int	init_global_mutexes(t_table *t)
{
	int	i;

	i = -1;
	t->fork_locks = init_forks(t);
	if (!t->fork_locks)
		return (0);
	if (pthread_mutex_init(&t->sim_stop_lock, 0) != 0)
	{
		while (++i < t->n_philos)
		{
			pthread_mutex_destroy(&t->fork_locks[i]);
			pthread_mutex_destroy(&t->philos[i]->meal_time_lock);
		}
		return (error_failure(STR_ERR_MUTEX, NULL, t));
	}
	if (pthread_mutex_init(&t->write_lock, 0) != 0)
	{
		while (++i < t->n_philos)
		{
			pthread_mutex_destroy(&t->fork_locks[i]);
			pthread_mutex_destroy(&t->philos[i]->meal_time_lock);
		}
		pthread_mutex_destroy(&t->sim_stop_lock);
		return (error_failure(STR_ERR_MUTEX, NULL, t));
	}
	return (1);
}

/* init_table:
*	Initializes the "dining table", the data structure containing
*	all of the program's parameters.
*	Returns a pointer to the allocated table structure, or NULL if
*	an error occured during initialization.
*/
t_table	*init_table(int ac, char **av, int i)
{
	t_table	*t;

	t = malloc(sizeof(t_table));
	if (!t)
		return (error_null(STR_ERR_MALLOC, NULL, 0));
	t->n_philos = integer_atoi(av[i++]);
	t->time_to_die = integer_atoi(av[i++]);
	t->time_to_eat = integer_atoi(av[i++]);
	t->time_to_sleep = integer_atoi(av[i++]);
	t->must_eat_count = -1;
	if (ac == 6)
		t->must_eat_count = integer_atoi(av[i]);
	t->philos = init_philosophers(t);
	if (!t->philos)
		return (NULL);
	if (!init_global_mutexes(t))
		return (NULL);
	t->sim_stop = 0;
	return (t);
}
