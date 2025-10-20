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

// 戻り値定義
# define SUCCESS 0
# define ERROR 1
# define MALLOC_ERROR -1
# define MUTEX_ERROR -2
# define THREAD_ERROR -3

// 状態メッセージ
# define MSG_FORK "has taken a fork"
# define MSG_EAT "is eating"
# define MSG_SLEEP "is sleeping"
# define MSG_THINK "is thinking"
# define MSG_DIED "died"

// タイミング調整
# define US_MONITOR_INTERVAL 100 // 監視間隔(μs)
# define US_ATE_DELAY 300        // 食後の待機時間(μs)

// 比較結果定義
# define CMP_EQUAL 1
# define CMP_LESS_THAN 2
# define CMP_GREATER_THAN 4
# define CMP_NOT_EQUAL 6
# define CMP_LESS_EQUAL_THAN 3
# define CMP_GREATER_EQUAL_THAN 5

typedef struct s_data	t_data;

typedef struct s_philosopher
{
	int							id;                      // 哲学者ID (1-based)
	int							eat_count;               // 現在の食事回数
	struct timeval				last_ate_at;            // 最後の食事開始時刻（timeval形式）
	int							is_eating;               // 食事中フラグ
	pthread_mutex_t				*left_fork;              // 左フォークへのポインタ
	pthread_mutex_t				*right_fork;             // 右フォークへのポインタ
	pthread_t					thread;                   // スレッドハンドル
	struct s_data				*data;                    // 共有データへの参照
}						t_philosopher;

typedef struct s_data
{
	// プログラム引数
	int							num_philosophers;         // 哲学者数
	struct timeval				time_to_die;              // 死亡時間(ms)
	struct timeval				time_to_eat;              // 食事時間(ms)
	struct timeval				time_to_sleep;            // 睡眠時間(ms)
	int							must_eat_count;           // 必要食事回数(-1=無制限)
	// 実行時情報
	struct timeval				started_at;               // シミュレーション開始時刻（timeval形式）
	int							simulation_end;           // 終了フラグ
	// スレッド同期
	pthread_mutex_t				print_mutex;              // 出力用ミューテックス
	pthread_mutex_t				data_mutex;               // データアクセス用ミューテックス
	pthread_mutex_t				meal_mutex;               // 食事情報アクセス用
	// リソース
	pthread_mutex_t				*forks;      // フォーク配列
	t_philosopher				*philosophers; // 哲学者配列
	pthread_t					monitor_thread;    // 監視スレッド
}						t_data;

/* Main functions */
int						main(int argc, char **argv);
int						init_data(t_data *data, int argc, char **argv);
void					cleanup_resources(t_data *data);
int						create_threads(t_data *data);
int						wait_for_threads(t_data *data);

/* Init functions */
int						init_args(t_data *data, int argc, char **argv);
int						init_philosophers(t_data *data);
int						init_mutexes(t_data *data);

/* Philosopher functions */
void					*philosopher_routine(void *arg);
void					eat(t_philosopher *philo);
void					think(t_philosopher *philo);
void					sleep_with_release_forks(t_philosopher *philo);
int						try_take_forks(t_philosopher *philo);
void					release_forks(t_philosopher *philo);

/* Monitor functions */
void					*monitor_routine(void *arg);
int						has_dead(t_data *data);
int						has_completed(t_data *data);

/* Utility functions */
void					log_action(t_philosopher *philo, const char *action);
long					get_time(void);
struct timeval			*timeval_from_ms(struct timeval *dest, long ms);
struct timeval			*add_timeval(struct timeval *dest, struct timeval *add);
struct timeval			*sub_timeval(struct timeval *dest, struct timeval *sub);
int						cmp_timeval(struct timeval *t1, struct timeval *t2);
void					sleep_until(struct timeval *wait_for);
int						ft_atoi(const char *str);
char					*timeval_to_string(struct timeval *src);

#endif
