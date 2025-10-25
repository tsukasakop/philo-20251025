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
	if (philo->data->simulation_end)
		return (ERROR);
	if (philo->id % 2 == 1)
	{
		pthread_mutex_lock(&philo->left_fork->mutex);
		log_action(philo, TAKE_FORK);
		pthread_mutex_lock(&philo->right_fork->mutex);
		log_action(philo, TAKE_FORK);
	}
	else
	{
		pthread_mutex_lock(&philo->right_fork->mutex);
		log_action(philo, TAKE_FORK);
		pthread_mutex_lock(&philo->left_fork->mutex);
		log_action(philo, TAKE_FORK);
	}
	return (SUCCESS);
}

static void	eat(t_philosopher *philo)
{
	struct timeval	tmp;

	pthread_mutex_lock(&philo->data->meal_mutex);
	philo->eat_count++;
	log_action(philo, EAT);
	pthread_mutex_unlock(&philo->data->meal_mutex);
	if (cmp_timeval(&philo->left_fork->released_at,
			&philo->right_fork->released_at) & CMP_GREATER_THAN)
		philo->last_ate_at = philo->left_fork->released_at;
	else
		philo->last_ate_at = philo->right_fork->released_at;
	tmp = philo->last_ate_at;
	add_timeval(&tmp, &(philo->data->time_to_eat));
	sleep_until(&tmp);
}

static void	sleep_with_release_forks(t_philosopher *philo)
{
	struct timeval	tmp;

	log_action(philo, SLEEP);
	tmp = philo->last_ate_at;
	add_timeval(&tmp, &(philo->data->time_to_eat));
	philo->right_fork->released_at = tmp;
	philo->left_fork->released_at = tmp;
	if (philo->id % 2 == 1)
	{
		pthread_mutex_unlock(&philo->right_fork->mutex);
		pthread_mutex_unlock(&philo->left_fork->mutex);
	}
	else
	{
		pthread_mutex_unlock(&philo->left_fork->mutex);
		pthread_mutex_unlock(&philo->right_fork->mutex);
	}
	add_timeval(&tmp, &(philo->data->time_to_sleep));
	sleep_until(&tmp);
}

void	*solo_routine(void *arg)
{
	t_philosopher	*philo;

	philo = (t_philosopher *)arg;
	pthread_mutex_lock(&philo->left_fork->mutex);
	log_action(philo, TAKE_FORK);
	while (!philo->data->simulation_end)
		usleep(100);
	pthread_mutex_unlock(&philo->left_fork->mutex);
	return (NULL);
}

void	*philosopher_routine(void *arg)
{
	t_philosopher	*philo;

	philo = (t_philosopher *)arg;
	while (!philo->data->simulation_end)
	{
		log_action(philo, THINK);
		if (try_take_forks(philo) == 0)
		{
			eat(philo);
			sleep_with_release_forks(philo);
			usleep(US_ATE_DELAY);
		}
	}
	return (NULL);
}
