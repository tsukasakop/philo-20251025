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

int	main(int argc, char **argv)
{
	t_data	data;

	if (argc != 5 && argc != 6)
	{
		printf(FORMAT_USAGE, argv[0]);
		return (EXIT_FAILURE);
	}
	if (init_data(&data, argc, argv) == ERROR)
	{
		printf(FORMAT_INIT_ERROR);
		return (EXIT_FAILURE);
	}
	if (create_threads(&data) == ERROR)
	{
		printf(FORMAT_INIT_ERROR);
		cleanup_resources(&data);
		return (EXIT_FAILURE);
	}
	wait_for_threads(&data);
	cleanup_resources(&data);
	return (EXIT_SUCCESS);
}

int	create_threads(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philosophers)
	{
		if (pthread_create(&data->philosophers[i].thread, NULL,
				philosopher_routine, &data->philosophers[i]) != 0)
			return (ERROR);
		i++;
	}
	if (pthread_create(&data->monitor_thread, NULL, monitor_routine, data) != 0)
		return (ERROR);
	return (SUCCESS);
}

int	wait_for_threads(t_data *data)
{
	int	i;

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
