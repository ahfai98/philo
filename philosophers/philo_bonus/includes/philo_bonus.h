/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyap <jyap@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 18:35:17 by jyap              #+#    #+#             */
/*   Updated: 2024/08/23 18:35:18 by jyap             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <limits.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <stdint.h>
# include <semaphore.h>
# include <signal.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <pthread.h>

# define MAX_PHILOS	200
# define STR_MAX_PHILOS "200"

# define STR_PROG_NAME	"philo_bonus:"
# define STR_USAGE	"%s usage: ./philo <number_of_philosophers> \
<time_to_die> <time_to_eat> <time_to_sleep> \
[number_of_times_each_philosopher_must_eat]\n"
# define STR_ERR_INPUT_DIGIT	"%s invalid input: %s: \
not a valid integer between 0 and 2147483647.\n"
# define STR_ERR_INPUT_POFLOW	"%s invalid input: \
there must be between 1 and %s philosophers.\n"
# define STR_ERR_THREAD	"%s error: Could not create thread.\n"
# define STR_ERR_MALLOC	"%s error: Could not allocate memory.\n"
# define STR_ERR_SEM	"%s error: Could not create semaphore.\n"
# define STR_ERR_FORK	"%s error: Could not fork child.\n"

# define SEM_NAME_FORKS "/philo_global_forks"
# define SEM_NAME_WRITE "/philo_global_write"
# define SEM_NAME_FULL	"/philo_global_full"
# define SEM_NAME_DEAD	"/philo_global_dead"
# define SEM_NAME_STOP	"/philo_global_stop"
# define SEM_NAME_MEAL	"/philo_local_meal_"

# define CHILD_EXIT_ERR_PTHREAD	40
# define CHILD_EXIT_ERR_SEM		41
# define CHILD_EXIT_PHILO_FULL	42
# define CHILD_EXIT_PHILO_DEAD	43

typedef struct s_philo	t_philo;

typedef struct s_table
{
	int				start_time;
	int	n_philos;
	int			time_to_die;
	int			time_to_eat;
	int			time_to_sleep;
	int				must_eat_count;
	sem_t			*sem_forks;
	sem_t			*sem_write;
	sem_t			*sem_philo_full;
	int	philo_full_count;
	sem_t			*sem_philo_dead;
	sem_t			*sem_stop;
	int			stop_sim;
	t_philo			**philos;
	t_philo			*this_philo;
	pid_t			*pids;
	pthread_t		gluttony_reaper;
	pthread_t		famine_reaper;
}	t_table;

typedef struct s_philo
{
	pthread_t			personal_grim_reaper;
	sem_t				*sem_forks;
	sem_t				*sem_write;
	sem_t				*sem_philo_full;
	sem_t				*sem_philo_dead;
	sem_t				*sem_meal;
	char				*sem_meal_name;
	int		nb_forks_held;
	int		id;
	int		times_ate;
	int				ate_enough;
	int				last_meal;
	t_table				*table;
}	t_philo;

typedef enum e_status
{
	DIED = 0,
	EATING = 1,
	SLEEPING = 2,
	THINKING = 3,
	TAKEN_FORK = 4
}	t_status;

//	main.c
int			has_sim_stopped(t_table *table);

//	parsing.c
int			is_valid_input(int ac, char **av);
int				integer_atoi(char *str);

//	init.c
t_table			*init_table(int ac, char **av, int i);

// ipc.c
void			init_philo_ipc(t_table *table, t_philo *philo);

//	philosopher.c
void			philosopher(t_table *table);

// philosopher_utils.c
void			grab_fork(t_philo *philo);

//	time.c
int			get_time_in_ms(void);
void			philo_sleep(int sleep_time);
void			sim_start_delay(int start_time);

//	output.c
void			write_status(t_philo *philo, int reaper, t_status status);
void			print_status(t_philo *philo, char *str);

//	grim_reaper.c
void			*global_gluttony_reaper(void *data);
void			*global_famine_reaper(void *data);
void			*personal_grim_reaper(void *data);
int				kill_all_philos(t_table *table, int exit_code);

// utils.c
char			*ft_utoa(int nb, size_t len);
char			*ft_strcat(char	*dst, const char *src);
size_t			ft_strlen(const char *str);
void			unlink_global_sems(void);
int			start_grim_reaper_threads(t_table *table);

// cleanup.c
void			*free_table(t_table *table);
int				sem_error_cleanup(t_table *table);
int				table_cleanup(t_table *table, int exit_code);

//	exit.c
void			child_exit(t_table *table, int exit_code);
int				msg(char *str, char *detail, int exit_no);
int				error_failure(char *str, char *details, t_table *table);
void			*error_null(char *str, char *details, t_table *table);

#endif