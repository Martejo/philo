/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gemartel <gemartel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 15:19:19 by gemartel          #+#    #+#             */
/*   Updated: 2024/03/20 13:37:39 by gemartel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/philosopher.h"

void	print_message(char *str, t_philo *philo, int id)
{
	long	time;
	t_table *table;

	table = philo->table;

	pthread_mutex_lock(philo->write_lock);
	time = get_time() - table->start_time;
	if (!dead_loop(philo))
		printf("%ld %d %s\n", time, id, str);
	pthread_mutex_unlock(philo->write_lock);
}

int	philosopher_dead(t_philo *philo, long time_to_die)
{
	pthread_mutex_lock(&philo->meal_lock);
	if (get_time() - philo->last_meal >= time_to_die)
	{
		pthread_mutex_unlock(&philo->meal_lock);
		return (1);
	}
	pthread_mutex_unlock(&philo->meal_lock);
	return (0);
}

int	check_if_dead(t_philo *philos)
{
	int	i;

	i = 0;
	while (i < philos[0].num_of_philos)
	{
		if (philosopher_dead(&philos[i], philos[i].time_to_die))
		{
			print_message("died", &philos[i], philos[i].id);
			pthread_mutex_lock(philos[0].dead_lock);
			*philos->dead = 1;
			pthread_mutex_unlock(philos[0].dead_lock);
			return (1);
		}
		i++;
	}
	return (0);
}

int	check_if_all_ate(t_philo *philos)
{
	int	i;
	int	finished_eating;

	i = 0;
	finished_eating = 0;
	if (philos[0].num_times_to_eat == -1)
		return (0);
	while (i < philos[0].num_of_philos)
	{
		pthread_mutex_lock(&philos[i].meal_lock);
		if (philos[i].is_full == 1)
			finished_eating++;
		pthread_mutex_unlock(&philos[i].meal_lock);
		i++;
	}
	if (finished_eating == philos[0].num_of_philos)
	{
		pthread_mutex_lock(philos[0].dead_lock);
		*philos->dead = 1;
		pthread_mutex_unlock(philos[0].dead_lock);
		return (1);
	}
	return (0);
}

void	*monitor(void *pointer)
{
	t_philo	*philos;

	philos = (t_philo *)pointer;
	while (1)
	{
		usleep(5000);
		if (check_if_dead(philos) == 1)
			break ;
		if (philos->num_times_to_eat > 0)
		{
			if (check_if_all_ate(philos) == 1)
			break ;
		}
	}
	return (pointer);
}