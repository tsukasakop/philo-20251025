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

/**
 * monitor_routine - Monitor thread that supervises the philosopher simulation
 * @arg: Pointer to t_data structure containing simulation parameters
 *
 * This function runs in a separate thread and continuously monitors the
 * simulation state. It checks if all philosophers have eaten the required
 * number of times or if any philosopher has died. The function prioritizes
 * checking completion before death to avoid race conditions.
 *
 * Return: NULL (required for pthread compatibility)
 */
/**
 * monitor_routine - 監視スレッドのメインルーチン
 * @arg: t_data構造体へのポインタ
 *
 * 各哲学者の最後の食事時刻をチェック、全員食事完了をチェック。
 *
 * Return: NULL(ストリード互換性のため必要)
 */
void	*monitor_routine(void *arg)
{
	t_data	*data;

	data = (t_data *)arg;
	while (!has_dead(data) && !has_completed(data))
		usleep(US_MONITOR_INTERVAL);
	return (NULL);
}

/**
 * check_deaths - 死亡チェック
 * @data: t_data構造体へのポインタ
 *
 * 各哲学者の最後の食事時刻取得、現在時刻との差分計算、
 * time_to_dieと比較、超過していれば死亡処理。
 * 精度要求: 実際の死亡から10ms以内に検知。
 *
 * Return: 1(死亡者あり), 0(死亡者なし)
 */
int	has_dead(t_data *data)
{
	int				i;
	struct timeval	sub;

	i = 0;
	while (i < data->num_philosophers)
	{
		pthread_mutex_lock(&data->meal_mutex);
		gettimeofday(&sub, NULL);
		sub_timeval(&sub, &(data->philosophers[i].last_ate_at));
		if (cmp_timeval(&sub, &data->time_to_die) & CMP_GREATER_EQUAL_THAN)
		{
			pthread_mutex_lock(&data->data_mutex);
			data->simulation_end = 1;
			pthread_mutex_unlock(&data->data_mutex);
			log_action(&data->philosophers[i], MSG_DIED);
			// fprintf(stderr,
			//    "philosopher %d last meal: %s ms, time since meal: %s ms\n",
			//     timeval_to_string(&now),
			//     data->philosophers[i].id,
			//     timeval_to_string(&data->philosophers[i].last_ate_at),
			//     timeval_to_string(&sub)
			// );
			pthread_mutex_unlock(&data->meal_mutex);
			return (1);
		}
		pthread_mutex_unlock(&data->meal_mutex);
		i++;
	}
	return (0);
}

/**
 * check_completion - 全員食事完了チェック
 * @data: t_data構造体へのポインタ
 *
 * must_eat_countが設定されているかチェック、
 * 全哲学者の食事回数確認、全員が必要回数達成していれば完了フラグセット。
 *
 * Return: 1(完了), 0(継続)
 */
int	has_completed(t_data *data)
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
