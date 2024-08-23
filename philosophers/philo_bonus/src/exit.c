/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyap <jyap@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 19:30:57 by jyap              #+#    #+#             */
/*   Updated: 2024/08/23 19:31:00 by jyap             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "philo_bonus.h"

/* msg:
*	Writes a message to the console. Returns the provided exit number.
*	Used for error management.
*/
int	msg(char *str, char *detail, int exit_no)
{
	if (!detail)
		printf(str, STR_PROG_NAME);
	else
		printf(str, STR_PROG_NAME, detail);
	return (exit_no);
}

/* error_failure:
*	Frees any allocated memory, prints an error message and
*	returns 0 to indicate failure.
*	Used for error management during initialization.
*/
int	error_failure(char *str, char *details, t_table *t)
{
	if (t != NULL)
		free_table(t);
	return (msg(str, details, 0));
}

/* error_null:
*	Frees any allocated memory, prints an error message and returns a NULL pointer.
*	Used for error management during initialization.
*/
void	*error_null(char *str, char *details, t_table *t)
{
	if (t != NULL)
		free_table(t);
	msg(str, details, EXIT_FAILURE);
	return (NULL);
}

/* child_exit:
*	Exits a child philosopher process with the appropriate exit code.
*	Prints an error message if the child encountered an error.
*/
void	child_exit(t_table *t, int exit_code)
{
	sem_post(t->this_philo->sem_meal);
	pthread_join(t->this_philo->personal_grim_reaper, NULL);
	if (exit_code == CHILD_EXIT_ERR_SEM)
		msg(STR_ERR_SEM, NULL, 0);
	if (exit_code == CHILD_EXIT_ERR_PTHREAD)
		msg(STR_ERR_THREAD, NULL, 0);
	sem_close(t->this_philo->sem_forks);
	sem_close(t->this_philo->sem_philo_full);
	sem_close(t->this_philo->sem_philo_dead);
	sem_close(t->this_philo->sem_write);
	sem_close(t->this_philo->sem_meal);
	free_table(t);
	exit(exit_code);
}
