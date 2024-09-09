/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyap <jyap@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 20:32:06 by jyap              #+#    #+#             */
/*   Updated: 2024/08/29 20:33:31 by jyap             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

/* Start of the main function */
int	main(int ac, char **av)
{
	t_table	table;
	t_philo	philo;

	memset(&table, 0, sizeof(table));
    memset(&philo, 0, sizeof(philo));
	if (ac < 5 || ac > 6)
		return (msg(STR_USAGE, NULL, EXIT_FAILURE));
	if (!is_valid_input(ac, av))
		return (EXIT_FAILURE);
	init_arg(&table, ac, av);
	philo.pid = malloc(sizeof(pid_t) * table.n_philos);
	if (philo.pid == NULL)
		return (1);
	if (init_philo(&philo, table) == 1)
	{
		free(philo.pid);
		return (1);
	}
	return (0);
}
