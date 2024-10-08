/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyap <jyap@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 13:35:57 by jyap              #+#    #+#             */
/*   Updated: 2024/09/07 07:47:49 by jyap             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <limits.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <stdbool.h>

# define MAX_PHILOS	200
# define STR_MAX_PHILOS "200"

# define STR_PROG_NAME	"philo:"
# define STR_USAGE	"%s usage: ./philo <number_of_philosophers> \
<time_to_die> <time_to_eat> <time_to_sleep> \
[number_of_times_each_philosopher_must_eat]\n"
# define STR_ERR_INPUT_DIGIT	"%s invalid input: %s: \
not a valid integer between 0 and 2147483647.\n"
# define STR_ERR_INPUT_POFLOW	"%s invalid input: \
there must be between 1 and %s philosophers.\n"
# define STR_ERR_THREAD	"%s error: Could not create thread.\n"
# define STR_ERR_MALLOC	"%s error: Could not allocate memory.\n"
# define STR_ERR_MUTEX	"%s error: Could not create mutex.\n"

typedef struct s_philo	t_philo;

typedef struct s_table
{
	unsigned long	start_time;
	int				n_philos;
	pthread_t		end_thread;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				must_eat_count;
	bool			sim_stop;
	pthread_mutex_t	sim_stop_lock;
	pthread_mutex_t	write_lock;
	pthread_mutex_t	*fork_locks;
	t_philo			**philos;
}	t_table;

typedef struct s_philo
{
	pthread_t		thread;
	int				id;
	int				times_ate;
	int				fork[2];
	pthread_mutex_t	meal_time_lock;
	unsigned long	last_meal;
	t_table			*table;
}	t_philo;

typedef enum e_status
{
	DIED = 0,
	EATING = 1,
	SLEEPING = 2,
	THINKING = 3,
	GOT_FORK = 4
}	t_status;

//	parsing.c
bool			is_valid_input(int ac, char **av);
int				ft_atoi(char *str);

//	init.c
t_table			*init_table(int ac, char **av, int i);

//	routines.c
void			*philosopher(void *data);

//	time.c
unsigned long	get_time_in_ms(void);
void			philo_sleep(t_table *table, int sleep_time);
void			sim_start_delay(unsigned long start_time);

//	output.c
void			write_status(t_philo *philo, bool reaper, t_status status);
void			write_outcome(t_table *table);
void			*error_null(char *str, char *details, t_table *table);
int				msg(char *str, char *detail, int exit_no);

//	check_end.c
void			*check_end(void *data);
bool			has_sim_stopped(t_table *table);

//	exit.c
int				error_failure(char *str, char *details, t_table *table);
void			free_table(t_table *table);
void			destroy_mutexes(t_table *table);

//	utils.c
void			assign_forks(t_philo *philo);
#endif