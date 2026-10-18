/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpflegha <jpflegha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 16:21:58 by jpflegha          #+#    #+#             */
/*   Updated: 2025/05/14 17:48:11 by jpflegha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	time_now(void)
{
	struct timeval	now;

	gettimeofday(&now, NULL);
	return ((now.tv_sec * 1000) + (now.tv_usec / 1000));
}

void	ft_usleep(long time, t_table *table)
{
	long int	start_time;
	long		elapsed;

	start_time = time_now();
	while (1)
	{
		if (simulation_finish(table))
			return ;
		usleep(10000);
		elapsed = time_now() - start_time;
		if (elapsed >= time)
			return ;
	}
}

void	error_and_exit(const char *error)
{
	printf(RED "%s\n" RST, error);
	exit(EXIT_FAILURE);
}

void	clean_all(t_table *tabel)
{
	t_philo	*philo;
	int		i;

	i = -1;
	while (++i < tabel->philosophers)
	{
		philo = tabel->philo + i;
		safe_mtx(&philo->philo_mtx, DESTROY);
	}
	i = -1;
	while (++i < tabel->philosophers)
		safe_mtx(&tabel->forks[i].fork, DESTROY);
	pthread_mutex_destroy(&tabel->table_mtx);
	free(tabel->forks);
	free(tabel->philo);
}

void	write_philo_status(char *msg, t_philo *philo)
{
	long	time;

	time = time_now() - philo->table->start;
	pthread_mutex_lock(&philo->table->table_mtx);
	if (!philo->table->ende_program)
		printf(M "%-6ld" C "%d %s\n" RST, time, philo->id, msg);
	pthread_mutex_unlock(&philo->table->table_mtx);
}
