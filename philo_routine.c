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

/**
 * philosopher_routine - Main routine executed by each philosopher thread
 * @arg: Pointer to the philosopher structure (cast to void*)
 *
 * This function implements the main loop for each philosopher thread.
 * Even-numbered philosophers start with a delay to reduce contention.
 * Each philosopher continuously tries to take forks, eat, drop forks,
 * sleep, and think until the simulation ends or they've eaten enough.
 *
 * Return: NULL (required for pthread compatibility)
 */
void	*philosopher_routine(void *arg)
{
	t_philosopher	*philo;

	philo = (t_philosopher *)arg;
	while (!philo->data->simulation_end)
	{
		think(philo);
		if (try_take_forks(philo) == 0)
		{
			eat(philo);
			sleep_with_release_forks(philo);
			usleep(US_ATE_DELAY);
		}
	}
	return (NULL);
}

/**
 * take_forks - Attempts to acquire both forks for a philosopher
 * @philo: Pointer to the philosopher structure
 *
 * This function handles fork acquisition with deadlock prevention.
 * For single philosopher case, it simulates the impossibility of eating.
 * For multiple philosophers, it uses different fork order based on
 * philosopher ID to prevent circular wait and deadlock. Odd philosophers
 * take left then right, even philosophers take right then left.
 *
 * Return: 0 on success (both forks acquired), 1 on failure or simulation end
 */
/**
 * try_take_forks - デッドロック回避してフォーク取得
 * @philo: 哲学者構造体へのポインタ
 *
 * デッドロック回避アルゴリズムを適用してフォークを取得する。
 * 奇数IDの哲学者: 左→右、偶数IDの哲学者: 右→左の順序で取得。
 *
 * Return: SUCCESS(成功), ERROR(失敗)
 */
int	try_take_forks(t_philosopher *philo)
{
	if (philo->data->simulation_end)
		return (ERROR);
	if (philo->id % 2 == 1)
	{
		pthread_mutex_lock(philo->left_fork);
		log_action(philo, MSG_FORK);
		pthread_mutex_lock(philo->right_fork);
		log_action(philo, MSG_FORK);
	}
	else
	{
		pthread_mutex_lock(philo->right_fork);
		log_action(philo, MSG_FORK);
		pthread_mutex_lock(philo->left_fork);
		log_action(philo, MSG_FORK);
	}
	return (SUCCESS);
}

/**
 * think - 思考フェーズ
 * @philo: 哲学者構造体へのポインタ
 *
 * 状態ログ出力、短時間のusleep（CPUを他に譲る）。
 *
 * Return: void
 */
void	think(t_philosopher *philo)
{
	log_action(philo, MSG_THINK);
	// usleep(100);
}

/**
 * eat - 食事処理
 * @philo: 哲学者構造体へのポインタ
 *
 * 食事開始時刻記録、食事中フラグセット、状態ログ出力、
 * 指定時間待機、食事回数インクリメント、食事中フラグクリア。
 *
 * Return: void
 */
void	eat(t_philosopher *philo)
{
	struct timeval	tmp;

	pthread_mutex_lock(&philo->data->meal_mutex);
	gettimeofday(&philo->last_ate_at, NULL);
	philo->eat_count++;
	log_action(philo, MSG_EAT);
	pthread_mutex_unlock(&philo->data->meal_mutex);
	memcpy(&tmp, &philo->last_ate_at, sizeof(struct timeval));
	add_timeval(&tmp, &(philo->data->time_to_eat));
	sleep_until(&tmp);
}

/**
 * sleep_with_release_forks - 食事後の睡眠フェーズ
 * @philo: 哲学者構造体へのポインタ
 *
 * 指定時間の睡眠、状態ログ出力。
 *
 * Return: void
 */
void	sleep_with_release_forks(t_philosopher *philo)
{
	struct timeval	tmp;

	log_action(philo, MSG_SLEEP);
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
	memcpy(&tmp, &philo->last_ate_at, sizeof(struct timeval));
	add_timeval(&tmp, &(philo->data->time_to_eat));
	add_timeval(&tmp, &(philo->data->time_to_sleep));
	sleep_until(&tmp);
}
