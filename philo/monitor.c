/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkondo <tkondo@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 16:31:48 by tkondo           #+#    #+#             */
/*   Updated: 2025/01/20 16:31:48 by tkondo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	has_dead(t_data *data)
{
	int				i;
	struct timeval	now;
	struct timeval	dead_at;

	i = 0;
	while (i < data->num_philosophers)
	{
		pthread_mutex_lock(&data->meal_mutex);
		gettimeofday(&now, NULL);
		dead_at = data->philosophers[i].last_ate_at;
		add_timeval(&dead_at, &(data->time_to_die));
		if (cmp_timeval(&now, &dead_at) & (CMP_EQUAL | CMP_GREATER_THAN))
		{
			pthread_mutex_lock(&data->data_mutex);
			data->simulation_end = 1;
			pthread_mutex_unlock(&data->data_mutex);
			log_action(&data->philosophers[i], DIED);
			pthread_mutex_unlock(&data->meal_mutex);
			return (1);
		}
		pthread_mutex_unlock(&data->meal_mutex);
		i++;
	}
	return (0);
}

static int	has_completed(t_data *data)
{
	int	i;
	int	all_eaten;

	if (data->must_eat_count == -1)
		return (0);
	pthread_mutex_lock(&data->meal_mutex);
	all_eaten = 1;
	i = 0;
	while (i < data->num_philosophers)
	{
		if (data->philosophers[i].eat_count < data->must_eat_count)
		{
			all_eaten = 0;
			break ;
		}
		i++;
	}
	pthread_mutex_lock(&data->data_mutex);
	if (all_eaten)
		data->simulation_end = 1;
	pthread_mutex_unlock(&data->data_mutex);
	pthread_mutex_unlock(&data->meal_mutex);
	return (all_eaten);
}

void	*monitor_routine(void *arg)
{
	t_data	*data;

	data = (t_data *)arg;
	while (!has_dead(data) && !has_completed(data))
		usleep(US_MONITOR_INTERVAL);
	return (NULL);
}
