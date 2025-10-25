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
 * cleanup_resources - リソースの完全解放
 * @data: t_data構造体へのポインタ
 *
 * スレッド終了待機（pthread_join）、ミューテックス破棄、メモリ解放、
 * 構造体初期化を適切な順序で行う。
 *
 * Return: void
 */
void	cleanup_resources(t_data *data)
{
	int	i;

	if (data->forks)
	{
		i = 0;
		while (i < data->num_philosophers)
		{
			pthread_mutex_destroy(&data->forks[i]);
			i++;
		}
		free(data->forks);
	}
	if (data->philosophers)
		free(data->philosophers);
	pthread_mutex_destroy(&data->print_mutex);
	pthread_mutex_destroy(&data->data_mutex);
	pthread_mutex_destroy(&data->meal_mutex);
}

/**
 * is_valid_number - Validates if a string represents a valid number
 * @str: String to validate
 *
 * This function checks if a string contains only valid numeric characters.
 * It allows an optional sign at the beginning but requires at least one
 * digit after the sign. Used for input validation.
 *
 * Return: 1 if the string is a valid number, 0 otherwise
 */
int	is_valid_number(const char *str)
{
	int	i;

	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (str[i] == '\0')
		return (0);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

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
void	log_action(t_philosopher *philo, t_action action)
{
	static const char	*msg[] = {
	[TAKE_FORK] = MSG_FORK,
	[EAT] = MSG_EAT,
	[SLEEP] = MSG_SLEEP,
	[THINK] = MSG_THINK,
	[DIED] = MSG_DIED
	};
	struct timeval		sub;

	pthread_mutex_lock(&philo->data->print_mutex);
	pthread_mutex_lock(&philo->data->data_mutex);
	if (!philo->data->simulation_end || action == DIED)
	{
		gettimeofday(&sub, NULL);
		sub_timeval(&sub, &(philo->data->started_at));
		if (sub.tv_sec == 0)
			printf("%d %d %s\n", sub.tv_usec, philo->id, msg[action]);
		else
			printf("%ld%03d %d %s\n", sub.tv_sec, sub.tv_usec, philo->id,
				msg[action]);
	}
	pthread_mutex_unlock(&philo->data->data_mutex);
	pthread_mutex_unlock(&philo->data->print_mutex);
}
