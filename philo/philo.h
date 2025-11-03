/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkondo <tkondo@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 16:31:48 by tkondo           #+#    #+#             */
/*   Updated: 2025/01/20 16:31:48 by tkondo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

# define MAX_PHILOSOPHERS 3000

# define SUCCESS 0
# define ERROR 1

# define MSG_FORK "has taken a fork"
# define MSG_EAT "is eating"
# define MSG_SLEEP "is sleeping"
# define MSG_THINK "is thinking"
# define MSG_DIED "died"

# define FORMAT_USAGE "Usage: %s n_of_philo die_ms eat_ms sleep_ms [must_eat]\n"
# define FORMAT_INIT_ERROR "Error: Failed to initialize data\n"

# define US_MONITOR_INTERVAL 100
# define US_ATE_DELAY 300

# define CMP_EQUAL 1
# define CMP_LESS_THAN 2
# define CMP_GREATER_THAN 4

typedef struct s_data	t_data;

typedef struct s_philosopher
{
	int							id;
	int							eat_count;
	struct timeval				last_ate_at;
	pthread_mutex_t				*left_fork;
	pthread_mutex_t				*right_fork;
	pthread_t					thread;
	struct s_data				*data;
}						t_philosopher;

typedef struct s_data
{
	int							num_philosophers;
	struct timeval				time_to_die;
	struct timeval				time_to_eat;
	struct timeval				time_to_sleep;
	int							must_eat_count;
	struct timeval				started_at;
	int							simulation_end;
	pthread_mutex_t				data_mutex;
	pthread_mutex_t				meal_mutex;
	pthread_mutex_t				*forks;
	t_philosopher				*philosophers;
	pthread_t					monitor_thread;
}						t_data;

typedef enum e_action
{
	TAKE_FORK,
	EAT,
	SLEEP,
	THINK,
	DIED
}	t_action;

int						main(int argc, char **argv);
int						create_threads(t_data *data);
int						wait_for_threads(t_data *data);
int						init_data(t_data *data, int argc, char **argv);
void					*solo_routine(void *arg);
void					*philosopher_routine(void *arg);
void					*monitor_routine(void *arg);
size_t					ft_strlen(const char *s);
int						ft_atoi(const char *str);
struct timeval			*timeval_from_ms(struct timeval *dest, long ms);
struct timeval			*add_timeval(struct timeval *dest, struct timeval *add);
struct timeval			*sub_timeval(struct timeval *dest, struct timeval *sub);
int						cmp_timeval(struct timeval *t1, struct timeval *t2);
void					sleep_until(struct timeval *wait_for);
void					cleanup_resources(t_data *data);
int						is_valid_number(const char *str);
void					log_action(t_philosopher *philo, t_action action);

#endif
