/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinner.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpflegha <jpflegha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 12:28:40 by jpflegha          #+#    #+#             */
/*   Updated: 2025/05/14 17:05:12 by jpflegha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*one_philo(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	set_long(&philo->philo_mtx, &philo->last_meal, time_now());
	increase_long(&philo->table->table_mtx, &philo->table->threads_running_nbr);
	write_philo_status("has taken a fork", philo);
	while (!simulation_finish(philo->table))
	{
		usleep(200);
	}
	return (NULL);
}

static void	eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->first_fork->fork);
	write_philo_status("has taken a fork", philo);
	pthread_mutex_lock(&philo->second_fork->fork);
	write_philo_status("has taken a fork", philo);
	set_long(&philo->philo_mtx, &philo->last_meal, time_now());
	philo->meals++;
	write_philo_status(B "is eating", philo);
	ft_usleep(philo->table->time_to_eat, philo->table);
	pthread_mutex_unlock(&philo->second_fork->fork);
	pthread_mutex_unlock(&philo->first_fork->fork);
	if (philo->table->meals_limit > 0
		&& philo->meals == philo->table->meals_limit)
		set_bool(&philo->philo_mtx, &philo->full, true);
}

void	*dinner_simulation(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	pthread_mutex_lock(&philo->table->table_mtx);
	pthread_mutex_unlock(&philo->table->table_mtx);
	set_long(&philo->philo_mtx, &philo->last_meal, time_now());
	increase_long(&philo->table->table_mtx, &philo->table->threads_running_nbr);
	de_sync_philo(philo);
	while (!simulation_finish(philo->table))
	{
		if (philo->full)
			break ;
		eat(philo);
		write_philo_status(Y "is sleeping", philo);
		ft_usleep(philo->table->time_to_sleep, philo->table);
		thinking(philo);
	}
	return (NULL);
}

void	start_process(t_table *table)
{
	int	i;

	i = -1;
	if (0 == table->meals_limit)
		return ;
	pthread_mutex_lock(&table->table_mtx);
	if (1 == table->philosophers)
		safe_thread(&table->philo[0].thread_id, one_philo, &table->philo[0],
			CREATE);
	else
	{
		while (++i < table->philosophers)
			safe_thread(&table->philo[i].thread_id, dinner_simulation,
				&table->philo[i], CREATE);
	}
	safe_thread(&table->monitor, monitor_dinner, table, CREATE);
	table->start = time_now();
	pthread_mutex_unlock(&table->table_mtx);
	i = -1;
	while (++i < table->philosophers)
		safe_thread(&table->philo[i].thread_id, NULL, NULL, JOIN);
	set_bool(&table->table_mtx, &table->ende_program, true);
	safe_thread(&table->monitor, NULL, NULL, JOIN);
}
