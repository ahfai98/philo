/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyap <jyap@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 19:31:53 by jyap              #+#    #+#             */
/*   Updated: 2024/08/23 19:44:57 by jyap             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "philo_bonus.h"

/* ft_strlen:
*	Measures the length of a string.
*/
size_t	ft_strlen(const char *str)
{
	size_t	i;

	i = 0;
	while (str[i] != '\0')
		i++;
	return (i);
}

/* ft_strcat:
*	Concatenates two strings.
*/
char	*ft_strcat(char	*dst, const char *src)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (dst[i])
		i++;
	j = 0;
	while (src[j])
	{
		dst[i + j] = src[j];
		j++;
	}
	dst[i + j] = '\0';
	return (dst);
}

/* ft_utoa:
*	Turns a integer into a string of characters. The length of
*	the string must be calculated in advance and specified to this function.
*/
char	*ft_utoa(int nb, size_t len)
{
	char	*ret;

	ret = malloc(sizeof * ret * (len + 1));
	if (!ret)
		return (NULL);
	ret[len] = '\0';
	len--;
	while (nb % 10)
	{
		ret[len--] = (nb % 10) + '0';
		nb /= 10;
	}
	return (ret);
}

void	unlink_global_sems(void)
{
	sem_unlink(SEM_NAME_FORKS);
	sem_unlink(SEM_NAME_WRITE);
	sem_unlink(SEM_NAME_FULL);
	sem_unlink(SEM_NAME_DEAD);
	sem_unlink(SEM_NAME_STOP);
}

int	start_grim_reaper_threads(t_table *t)
{
	if (pthread_create(&t->gluttony_reaper, NULL,
			&global_gluttony_reaper, t) != 0)
		return (error_failure(STR_ERR_THREAD, NULL, t));
	if (pthread_create(&t->famine_reaper, NULL,
			&global_famine_reaper, t) != 0)
		return (error_failure(STR_ERR_THREAD, NULL, t));
	return (1);
}
