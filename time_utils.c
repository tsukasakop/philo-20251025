/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkondo <tkondo@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 09:35:00 by tkondo           #+#    #+#             */
/*   Updated: 2025/01/26 09:35:00 by tkondo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

struct timeval	*timeval_from_ms(struct timeval *dest, long ms)
{
	memset(dest, 0, sizeof(struct timeval));
	dest->tv_sec = ms / 1000;
	dest->tv_usec = (ms % 1000) * 1000;
	return (dest);
}

struct timeval	*add_timeval(struct timeval *dest, struct timeval *add)
{
	if ((1000000 - dest->tv_usec) <= add->tv_usec)
	{
		dest->tv_sec += add->tv_sec + 1;
		dest->tv_usec -= 1000000 - add->tv_usec;
	}
	else
	{
		dest->tv_sec += add->tv_sec;
		dest->tv_usec += add->tv_usec;
	}
	return (dest);
}

struct timeval	*sub_timeval(struct timeval *dest, struct timeval *sub)
{
	if (dest->tv_usec < sub->tv_usec)
	{
		dest->tv_sec -= sub->tv_sec + 1;
		dest->tv_usec = 1000000 - sub->tv_usec + dest->tv_usec;
	}
	else
	{
		dest->tv_sec -= sub->tv_sec;
		dest->tv_usec -= sub->tv_usec;
	}
	return (dest);
}

int	cmp_timeval(struct timeval *t1, struct timeval *t2)
{
	if (t1->tv_sec > t2->tv_sec)
		return (CMP_GREATER_THAN);
	else if (t1->tv_sec < t2->tv_sec)
		return (CMP_LESS_THAN);
	if (t1->tv_usec > t2->tv_usec)
		return (CMP_GREATER_THAN);
	else if (t1->tv_usec < t2->tv_usec)
		return (CMP_LESS_THAN);
	return (CMP_EQUAL);
}

void	sleep_until(struct timeval *wait_for)
{
	struct timeval	now;

	gettimeofday(&now, NULL);
	while (cmp_timeval(&now, wait_for) & CMP_LESS_THAN)
	{
		usleep(0);
		gettimeofday(&now, NULL);
	}
}
