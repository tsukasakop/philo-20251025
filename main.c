/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkondo <tkondo@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 16:31:48 by tkondo           #+#    #+#             */
/*   Updated: 2025/01/20 16:31:48 by tkondo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * main - Entry point for the philosophers simulation program
 * @argc: Number of command line arguments
 * @argv: Array of command line argument strings
 *
 * This function serves as the entry point for the philosophers simulation.
 * It parses command line arguments, initializes the simulation data,
 * starts the simulation, and performs cleanup. It also disables output
 * buffering to ensure real-time output.
 *
 * Return: 0 on success, 1 on error
 */
int	main(int argc, char **argv)
{
	t_data	data;

	// 引数チェック
	if (argc != 5 && argc != 6)
	{
		printf(FORMAT_USAGE, argv[0]);
		return (EXIT_FAILURE);
	}
	// 初期化
	if (init_data(&data, argc, argv) == ERROR)
	{
		printf(FORMAT_INIT_ERROR);
		return (EXIT_FAILURE);
	}
	// スレッド作成
	if (create_threads(&data) == ERROR)
	{
		printf(FORMAT_INIT_ERROR);
		cleanup_resources(&data);
		return (EXIT_FAILURE);
	}
	// 終了待機
	wait_for_threads(&data);
	// リソース解放
	cleanup_resources(&data);
	return (EXIT_SUCCESS);
}

int	create_threads(t_data *data)
{
	int	i;

	// 哲学者スレッド生成
	i = 0;
	while (i < data->num_philosophers)
	{
		if (pthread_create(&data->philosophers[i].thread, NULL,
				philosopher_routine, &data->philosophers[i]) != 0)
			return (ERROR);
		i++;
	}
	// 監視スレッド生成
	if (pthread_create(&data->monitor_thread, NULL, monitor_routine, data) != 0)
		return (ERROR);
	return (SUCCESS);
}

int	wait_for_threads(t_data *data)
{
	int	i;

	// スレッド終了待機
	i = 0;
	while (i < data->num_philosophers)
	{
		if (pthread_detach(data->philosophers[i].thread) != 0)
			return (ERROR);
		i++;
	}
	if (pthread_join(data->monitor_thread, NULL) != 0)
		return (ERROR);
	return (SUCCESS);
}
