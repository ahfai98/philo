/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyap <jyap@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 20:32:20 by jyap              #+#    #+#             */
/*   Updated: 2024/08/29 22:12:25 by jyap             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

/* Converts numbers into a string */

static int	number_len(int number)
{
	unsigned int	len;

	len = 0;
	if (number == 0)
		return (1);
	if (number < 0)
		len++;
	while (number != 0)
	{
		number /= 10;
		len++;
	}
	return (len);
}

char	*ft_itoa(int i)
{
	char			*str;
	unsigned int	number;
	int				len;

	len = number_len(i);
	str = malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	if (i < 0)
	{
		str[0] = '-';
		number = -i;
	}
	else
		number = i;
	if (i == 0)
		str[0] = '0';
	str[len] = '\0';
	while (number != 0)
	{
		str[len - 1] = (number % 10) + '0';
		number /= 10;
		len--;
	}
	return (str);
}

static int	ft_strlen(const char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
		i++;
	return (i);
}

/* Joins two strings into one */
char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*output;
	int		i;
	int		j;

	if (!s1 || !s2)
		return (NULL);
	output = malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (output == NULL)
		return (NULL);
	i = 0;
	j = 0;
	while (s1[i] != '\0')
		output[j++] = s1[i++];
	i = 0;
	while (s2[i] != '\0')
		output[j++] = s2[i++];
	output[j] = '\0';
	return (output);
}

/* Finishes the simulation and exits the program cleanly */
void	finish_and_exit(t_philo *philo)
{
	waitpid(-1, NULL, 0);
	free(philo->pid);
	kill(0, SIGINT);
}
