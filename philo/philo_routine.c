/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkondo <tkondo@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 16:31:48 by tkondo           #+#    #+#             */
/*   Updated: 2025/01/20 16:31:48 by tkondo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	try_take_forks(t_philosopher *philo)
{
	if (philo->id % 2 == 1)
	{
		pthread_mutex_lock(philo->left_fork);
		log_action(philo, TAKE_FORK);
		pthread_mutex_lock(philo->right_fork);
		log_action(philo, TAKE_FORK);
	}
	else
	{
		pthread_mutex_lock(philo->right_fork);
		log_action(philo, TAKE_FORK);
		pthread_mutex_lock(philo->left_fork);
		log_action(philo, TAKE_FORK);
	}
	return (SUCCESS);
}

static void	eat(t_philosopher *philo)
{
	struct timeval	tmp;

	pthread_mutex_lock(&philo->data->meal_mutex);
	gettimeofday(&philo->last_ate_at, NULL);
	philo->eat_count++;
	log_action(philo, EAT);
	tmp = philo->last_ate_at;
	add_timeval(&tmp, &(philo->data->time_to_eat));
	pthread_mutex_unlock(&philo->data->meal_mutex);
	sleep_until(&tmp);
}

static void	sleep_with_release_forks(t_philosopher *philo)
{
	struct timeval	tmp;

	log_action(philo, SLEEP);
	if (philo->id % 2 == 1)
	{
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(philo->left_fork);
	}
	else
	{
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
	}
	tmp = philo->last_ate_at;
	add_timeval(&tmp, &(philo->data->time_to_eat));
	add_timeval(&tmp, &(philo->data->time_to_sleep));
	sleep_until(&tmp);
}

void	*solo_routine(void *arg)
{
	t_philosopher	*philo;

	philo = (t_philosopher *)arg;
	pthread_mutex_lock(philo->left_fork);
	log_action(philo, TAKE_FORK);
	while (1)
	{
		pthread_mutex_lock(&philo->data->data_mutex);
		if (philo->data->simulation_end)
		{
			pthread_mutex_unlock(&philo->data->data_mutex);
			break ;
		}
		pthread_mutex_unlock(&philo->data->data_mutex);
		usleep(US_DELAY);
	}
	pthread_mutex_unlock(philo->left_fork);
	return (NULL);
}

void	*philosopher_routine(void *arg)
{
	t_philosopher	*philo;

	philo = (t_philosopher *)arg;
	while (1)
	{
		pthread_mutex_lock(&philo->data->data_mutex);
		if (philo->data->simulation_end)
		{
			pthread_mutex_unlock(&philo->data->data_mutex);
			break ;
		}
		pthread_mutex_unlock(&philo->data->data_mutex);
		log_action(philo, THINK);
		if (try_take_forks(philo) == 0)
		{
			eat(philo);
			sleep_with_release_forks(philo);
			usleep(US_DELAY);
		}
	}
	return (NULL);
}
