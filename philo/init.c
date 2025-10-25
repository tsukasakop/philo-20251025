/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkondo <tkondo@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 16:31:48 by tkondo           #+#    #+#             */
/*   Updated: 2025/01/20 16:31:48 by tkondo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	init_args(t_data *data, int argc, char **argv)
{
	if (!is_valid_number(argv[1]) || !is_valid_number(argv[2])
		|| !is_valid_number(argv[3]) || !is_valid_number(argv[4]))
		return (ERROR);
	if (ft_atoi(argv[1]) <= 0 || ft_atoi(argv[2]) < 0 || ft_atoi(argv[3]) < 0
		|| ft_atoi(argv[4]) < 0)
		return (ERROR);
	if (argc == 6 && (!is_valid_number(argv[5]) || ft_atoi(argv[5]) < 0))
		return (ERROR);
	if (ft_atoi(argv[1]) > MAX_PHILOSOPHERS)
		return (ERROR);
	data->num_philosophers = ft_atoi(argv[1]);
	timeval_from_ms(&data->time_to_die, ft_atoi(argv[2]));
	timeval_from_ms(&data->time_to_eat, ft_atoi(argv[3]));
	timeval_from_ms(&data->time_to_sleep, ft_atoi(argv[4]));
	if (argc == 6)
		data->must_eat_count = ft_atoi(argv[5]);
	else
		data->must_eat_count = -1;
	return (SUCCESS);
}

static int	init_fork_mutexes(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philosophers)
	{
		if (pthread_mutex_init(&data->forks[i].mutex, NULL) != 0)
		{
			while (i > 0)
				pthread_mutex_destroy(&data->forks[--i].mutex);
			return (ERROR);
		}
		i++;
	}
	return (SUCCESS);
}

static int	init_mutexes(t_data *data)
{
	int	success_flag;

	success_flag = 0;
	success_flag |= (pthread_mutex_init(&data->meal_mutex, NULL) != 0) << 0;
	success_flag |= (pthread_mutex_init(&data->data_mutex, NULL) != 0) << 1;
	success_flag |= (pthread_mutex_init(&data->print_mutex, NULL) != 0) << 2;
	if (success_flag || init_fork_mutexes(data) != SUCCESS)
	{
		if (success_flag & (1 << 0))
			pthread_mutex_destroy(&data->meal_mutex);
		if (success_flag & (1 << 1))
			pthread_mutex_destroy(&data->data_mutex);
		if (success_flag & (1 << 2))
			pthread_mutex_destroy(&data->print_mutex);
		return (ERROR);
	}
	return (SUCCESS);
}

static int	init_philosophers(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philosophers)
	{
		data->philosophers[i].id = i + 1;
		data->philosophers[i].eat_count = 0;
		data->philosophers[i].last_ate_at = data->started_at;
		data->philosophers[i].is_eating = 0;
		data->philosophers[i].left_fork = &data->forks[i];
		data->philosophers[i].right_fork = &data->forks[(i + 1)
			% data->num_philosophers];
		data->philosophers[i].data = data;
		i++;
	}
	return (SUCCESS);
}

int	init_data(t_data *data, int argc, char **argv)
{
	int	i;

	memset(data, 0, sizeof(t_data));
	if (init_args(data, argc, argv) != SUCCESS)
		return (ERROR);
	data->simulation_end = 0;
	data->philosophers = malloc(sizeof(t_philosopher) * data->num_philosophers);
	data->forks = malloc(sizeof(t_fork) * data->num_philosophers);
	if (!data->philosophers || !data->forks || init_mutexes(data) != SUCCESS)
	{
		free(data->philosophers);
		free(data->forks);
		return (ERROR);
	}
	gettimeofday(&data->started_at, NULL);
	i = 0;
	while (i < data->num_philosophers)
		data->forks[i++].released_at = data->started_at;
	if (init_philosophers(data) != SUCCESS)
	{
		cleanup_resources(data);
		return (ERROR);
	}
	return (SUCCESS);
}
