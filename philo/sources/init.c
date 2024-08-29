/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyap <jyap@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 13:36:20 by jyap              #+#    #+#             */
/*   Updated: 2024/08/29 17:51:58 by jyap             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	*free_philos(t_philo **philos, int i, int free_curr)
{
	if (free_curr == 1)
		free(philos[i]);
	else if (free_curr == 2)
	{
		pthread_mutex_destroy(&philos[i]->meal_time_lock);
		free(philos[i]);
	}
	while (--i >= 0)
	{
		pthread_mutex_destroy(&philos[i]->meal_time_lock);
		free(philos[i]);
	}
	free(philos);
	if (free_curr == 1)
		return (error_null(STR_ERR_MUTEX, NULL, 0));
	return (error_null(STR_ERR_MALLOC, NULL, 0));
}

static pthread_mutex_t	*init_forks(t_table *table)
{
	pthread_mutex_t	*forks;
	int				i;

	forks = malloc(sizeof(pthread_mutex_t) * table->n_philos);
	if (!forks)
	{
		free_philos(table->philos, table->n_philos - 1, 2);
		return (error_null(STR_ERR_MALLOC, NULL, 0));
	}
	i = 0;
	while (i < table->n_philos)
	{
		if (pthread_mutex_init(&forks[i], 0) != 0)
		{
			while (--i >= 0)
				pthread_mutex_destroy(&forks[i]);
			free(forks);
			free_philos(table->philos, table->n_philos - 1, 2);
			return (error_null(STR_ERR_MUTEX, NULL, 0));
		}
		i++;
	}
	return (forks);
}

static t_philo	**init_philosophers(t_table *table)
{
	t_philo		**philos;
	int			i;

	philos = malloc(sizeof(t_philo *) * table->n_philos);
	if (!philos)
		return (error_null(STR_ERR_MALLOC, NULL, 0));
	i = -1;
	while (++i < table->n_philos)
	{
		philos[i] = malloc(sizeof(t_philo));
		if (!philos[i])
			return (free_philos(philos, i, 0));
		if (pthread_mutex_init(&philos[i]->meal_time_lock, 0) != 0)
			return (free_philos(philos, i, 1));
		philos[i]->table = table;
		philos[i]->id = i;
		philos[i]->times_ate = 0;
		assign_forks(philos[i]);
	}
	return (philos);
}

static bool	init_global_mutexes(t_table *table)
{
	int	i;

	i = table->n_philos;
	table->fork_locks = init_forks(table);
	if (!table->fork_locks)
		return (false);
	if (pthread_mutex_init(&table->sim_stop_lock, 0) != 0)
	{
		while (--i >= 0)
			pthread_mutex_destroy(&table->fork_locks[i]);
		free_philos(table->philos, table->n_philos, 2);
		free(table->fork_locks);
		return (error_failure(STR_ERR_MUTEX, NULL, NULL));
	}
	if (pthread_mutex_init(&table->write_lock, 0) != 0)
	{
		while (--i >= 0)
			pthread_mutex_destroy(&table->fork_locks[i]);
		free_philos(table->philos, table->n_philos, 2);
		free(table->fork_locks);
		pthread_mutex_destroy(&table->sim_stop_lock);
		return (error_failure(STR_ERR_MUTEX, NULL, NULL));
	}
	return (true);
}

t_table	*init_table(int ac, char **av, int i)
{
	t_table	*table;

	table = malloc(sizeof(t_table));
	if (!table)
		return (error_null(STR_ERR_MALLOC, NULL, 0));
	table->n_philos = ft_atoi(av[i++]);
	table->time_to_die = ft_atoi(av[i++]);
	table->time_to_eat = ft_atoi(av[i++]);
	table->time_to_sleep = ft_atoi(av[i++]);
	table->must_eat_count = -1;
	if (ac == 6)
		table->must_eat_count = ft_atoi(av[i]);
	table->philos = init_philosophers(table);
	if (!table->philos)
	{
		free(table);
		return (NULL);
	}
	if (!init_global_mutexes(table))
	{
		free(table);
		return (NULL);
	}
	table->sim_stop = false;
	return (table);
}
