/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyap <jyap@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 20:32:34 by jyap              #+#    #+#             */
/*   Updated: 2024/08/29 22:11:29 by jyap             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <stdio.h>
# include <signal.h>
# include <unistd.h>
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>
# include <semaphore.h>
# include <sys/stat.h> 
# include <sys/wait.h> 
# include <fcntl.h> 
# include <stdbool.h>
# include <limits.h>

# define MAX_PHILOS	200
# define STR_MAX_PHILOS "200"

# define STR_PROG_NAME	"philo:"
# define STR_USAGE	"%s usage: ./philo <number_of_philosophers> \
<time_to_die> <time_to_eat> <time_to_sleep> \
[number_of_times_each_philosopher_must_eat]\n"
# define STR_ERR_INPUT_DIGIT	"%s invalid table: %s: \
not a valid integer between 0 and 2147483647.\n"
# define STR_ERR_INPUT_POFLOW	"%s invalid table: \
there must be between 1 and %s philosophers.\n"
# define STR_ERR_THREAD	"%s error: Could not create thread.\n"
# define STR_ERR_MALLOC	"%s error: Could not allocate memory.\n"
# define STR_ERR_SEM	"%s error: Could not create semaphore.\n"
# define STR_ERR_FORK	"%s error: Could not fork child.\n"

typedef struct s_philo	t_philo;

typedef struct s_table
{
	int	n_philos;
	int	must_eat_count;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
}	t_table;

typedef struct s_philo
{
	int				n;
	pid_t			*pid;
	sem_t			*fork;
	sem_t			*full;
	sem_t			*read;
	sem_t			*write;
	sem_t			*end;
	t_table			table;
	pthread_t		death_thread;
	pthread_t		cleanup_thread;
	pid_t			stomach_process;
	struct timeval	start_time;
	struct timeval	last_ate;
}	t_philo;

/* Libft */
int				ft_atoi(const char *str);
char			*ft_strjoin(char const *s1, char const *s2);
void			ft_usleep(int sec);
char			*ft_itoa(int n);

/* Initializing */
void			init_arg(t_table *table, int ac, char **av);
int				init_philo(t_philo *philo, t_table table);
int				create_sema(sem_t **sem, char *name, int count);

/* Getters */
struct timeval	get_starttime(void *timezone);
int				get_time(struct timeval old);
void			get_message(t_philo *philo, int i, char *str, int end);

/* Philosophizing */
void			routine(t_philo *philo);
void			finish_and_exit(t_philo *philo);
bool			is_valid_input(int ac, char **av);
int				msg(char *str, char *detail, int exit_no);
int				get_time_in_ms(struct timeval *time);
void			sim_start_delay(int start_time);
void			sem_cleanup(t_philo *philo);
void			check_stomach_and_death(t_philo *philo, t_table table);

#endif