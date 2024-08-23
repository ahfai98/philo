/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyap <jyap@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 19:31:14 by jyap              #+#    #+#             */
/*   Updated: 2024/08/23 19:31:15 by jyap             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "philo_bonus.h"

/* set_local_sem_name:
*	Creates a unique semaphore name to create a mutex that protects
*	a philosopher's own meal variables. The name must be unique, otherwise
*	all philosopher processes would share the same meal semaphore.
*	In order to create this semaphore name, simply concatenate the standard
*	semaphore meal name with the philosopher's ID.
*	Returns the unique semaphore name with the given ID. NULL if memory
*	allocation fails.
*/
static char	*set_local_sem_name(const char *str, int id)
{
	int	i;
	int	digit_count;
	char			*sem_name;
	char			*tmp;

	digit_count = 0;
	i = id;
	while (i)
	{
		digit_count++;
		i /= 10;
	}
	i = ft_strlen(str) + digit_count;
	sem_name = malloc (sizeof(char) * (i + 1));
	if (sem_name == NULL)
		return (NULL);
	sem_name[0] = '\0';
	sem_name = ft_strcat(sem_name, str);
	tmp = ft_utoa(id, digit_count);
	sem_name = ft_strcat(sem_name, tmp);
	free(tmp);
	return (sem_name);
}

/* set_philo_sem_names:
*	Creates unique semaphore names for semaphores that will only be shared
*	between a philo process and its personal grim reaper thread. No other
*	process will open the semaphores belonging to a different philo process.
*/
static int	set_philo_sem_names(t_philo *philo)
{
	philo->sem_meal_name = set_local_sem_name(SEM_NAME_MEAL, philo->id + 1);
	if (philo->sem_meal_name == NULL)
		return (0);
	return (1);
}

/* init_philosophers:
*	Allocates memory for each philosopher and initializes their values.
*	Also creates specific semaphore names to protect their own meal-related
*	variables. The semaphore names for sem_meal must be unique to each
*	philosopher because the semaphore shouldn't be accessed by a different
*	philosopher process.
*	Returns a pointer to the array of philosophers or NULL if
*	initialization failed.
*/
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
		philos[i] = malloc(sizeof(t_philo));
		if (!philos[i])
			return (error_null(STR_ERR_MALLOC, NULL, 0));
		philos[i]->table = t;
		philos[i]->id = i;
		if (!set_philo_sem_names(philos[i]))
			return (error_null(STR_ERR_MALLOC, NULL, t));
		philos[i]->times_ate = 0;
		philos[i]->nb_forks_held = 0;
		philos[i]->ate_enough = 0;
		i++;
	}
	return (philos);
}

/* init_global_semaphores:
*	Initializes semaphores for forks, writing, fullness and death detection.
*	These sempahores are first opened in the parent process, but each child
*	process will open the same named semaphores, which will allow interprocess
*	communication through these semaphores.
*
*	Returns 1 if the initalizations were successful, 0 if
*	initilization failed.
*/
static int	init_global_semaphores(t_table *t)
{
	unlink_global_sems();
	t->sem_forks = sem_open(SEM_NAME_FORKS, O_CREAT,
			S_IRUSR | S_IWUSR, t->n_philos);
	if (t->sem_forks == SEM_FAILED)
		return (sem_error_cleanup(t));
	t->sem_write = sem_open(SEM_NAME_WRITE, O_CREAT,
			S_IRUSR | S_IWUSR, 1);
	if (t->sem_write == SEM_FAILED)
		return (sem_error_cleanup(t));
	t->sem_philo_full = sem_open(SEM_NAME_FULL, O_CREAT,
			S_IRUSR | S_IWUSR, t->n_philos);
	if (t->sem_philo_full == SEM_FAILED)
		return (sem_error_cleanup(t));
	t->sem_philo_dead = sem_open(SEM_NAME_DEAD, O_CREAT,
			S_IRUSR | S_IWUSR, t->n_philos);
	if (t->sem_philo_dead == SEM_FAILED)
		return (sem_error_cleanup(t));
	t->sem_stop = sem_open(SEM_NAME_STOP, O_CREAT,
			S_IRUSR | S_IWUSR, 1);
	if (t->sem_stop == SEM_FAILED)
		return (sem_error_cleanup(t));
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
	t->philo_full_count = 0;
	t->stop_sim = 0;
	if (ac == 6)
		t->must_eat_count = integer_atoi(av[i]);
	if (!init_global_semaphores(t))
		return (NULL);
	t->philos = init_philosophers(t);
	if (!t->philos)
		return (NULL);
	t->pids = malloc(sizeof(*t->pids) * t->n_philos);
	if (!t->pids)
		return (error_null(STR_ERR_MALLOC, NULL, 0));
	return (t);
}
