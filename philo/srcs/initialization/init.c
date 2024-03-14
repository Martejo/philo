#include "../../include/philosopher.h"

void	init_input(t_philo *philo, char **argv)
{
	philo->time_to_die = ft_atol(argv[2]);
	philo->time_to_eat = ft_atol(argv[3]);
	philo->time_to_sleep = ft_atol(argv[4]);
	philo->num_of_philos = (int)ft_atol(argv[1]);
	if (argv[5])
		philo->num_times_to_eat = (int)ft_atol(argv[5]);
	else
		philo->num_times_to_eat = -1;
}

void	init_philos(t_philo *philos, t_table *table, pthread_mutex_t *forks, char **argv, int philo_nbr)
{
	int	i;

	i = 0;
	while (i < philo_nbr)
	{
		init_input(&philos[i], argv);
		philos[i].id = i + 1;
		philos[i].eating = 0;
		philos[i].meals_eaten = 0;
		philos[i].start_time = get_time(MILLISECOND);
		philos[i].last_meal = get_time(MILLISECOND);
		philos[i].write_lock = &table->write_lock;
		philos[i].dead_lock = &table->dead_lock;
		philos[i].meal_lock = &table->meal_lock;
		philos[i].dead = &table->dead_flag;
		philos[i].thread_error = &table->thread_error;
		philos[i].l_fork = &forks[i];
		if (i == 0)
			philos[i].r_fork = &forks[philos[i].num_of_philos - 1];
		else
			philos[i].r_fork = &forks[i - 1];
		i++;
	}
}

void	init_forks(pthread_mutex_t *forks, int philo_num)
{
	int	i;

	i = 0;
	while (i < philo_num)
	{
		handle_mutex_error(pthread_mutex_init(&forks[i], NULL), INIT);
		i++;
	}
}

void	init_launcher(t_table *table, t_philo *philos)
{
	table->dead_flag = 0;
	table->philos	= philos;
	table->thread_error.error	= 0;
	handle_mutex_error(pthread_mutex_init(&table->write_lock, NULL), INIT);
	handle_mutex_error(pthread_mutex_init(&table->dead_lock, NULL), INIT);
	handle_mutex_error(pthread_mutex_init(&table->meal_lock, NULL), INIT);
}