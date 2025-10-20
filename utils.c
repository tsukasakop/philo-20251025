/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkondo <tkondo@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 16:31:48 by tkondo           #+#    #+#             */
/*   Updated: 2025/01/20 16:31:48 by tkondo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * log_action - 状態変化のログ出力
 * @philo: 哲学者構造体へのポインタ
 * @action: アクション文字列
 *
 * スレッドセーフなログ出力を提供する。print_mutexで保護され、
 * シミュレーション開始時刻からの相対タイムスタンプを計算し、
 * "timestamp_ms philosopher_id action"形式で出力する。
 *
 * Return: void
 */
void	log_action(t_philosopher *philo, const char *action)
{
	struct timeval	sub;

	pthread_mutex_lock(&philo->data->print_mutex);
	pthread_mutex_lock(&philo->data->data_mutex);
	if (!philo->data->simulation_end || strcmp(action, MSG_DIED) == 0)
	{
		gettimeofday(&sub, NULL);
		sub_timeval(&sub, &(philo->data->started_at));
		if (sub.tv_sec == 0)
			printf("%d %d %s\n", sub.tv_usec / 1000, philo->id, action);
		else
			printf("%ld%03d %d %s\n", sub.tv_sec, sub.tv_usec / 1000, philo->id,
				action);
	}
	pthread_mutex_unlock(&philo->data->data_mutex);
	pthread_mutex_unlock(&philo->data->print_mutex);
}
