/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpflegha <jpflegha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 17:08:01 by jpflegha          #+#    #+#             */
/*   Updated: 2025/05/06 19:59:06 by jpflegha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static inline bool	ft_is_digit(char c)
{
	return (c >= '0' && c <= '9');
}

static inline bool	ft_is_space(char c)
{
	return ((c >= 9 && c <= 13) || c == 32);
}

/// @brief check for: negative nbr, correct nbr, check for INT_MAX,
/// @return the correct numbr or exit
static const char	*correct_input(const char *str)
{
	const char	*nbr;

	while (ft_is_space(*str))
	{
		str++;
	}
	if (*str == '+')
		str++;
	else if (*str == '-')
		error_and_exit("No negative numbers!");
	if (!ft_is_digit(*str))
		error_and_exit("The input is not a digit!");
	nbr = str;
	while (ft_is_digit(*str))
		str++;
	if (*str != '\0' && !ft_is_space(*str))
		error_and_exit("Invalid character in input");
	return (nbr);
}

static long	ft_atol(const char *str)
{
	long	nbr;

	nbr = 0;
	str = correct_input(str);
	while (ft_is_digit(*str))
	{
		nbr = (nbr * 10) + (*str++ - 48);
		if (nbr > INT_MAX)
			error_and_exit("Number is bigger than INT_MAX (2147483647)");
	}
	return (nbr);
}

/// @brief Checks argv if they are numbers < INT_MAX and if timestamp > 60ms
/// @param table Pointer to the t_table structure
/// @param argv Command-line arguments to check
void	check_input(t_table *table, char **argv)
{
	table->philosophers = ft_atol(argv[1]);
	table->time_to_die = ft_atol(argv[2]);
	table->time_to_eat = ft_atol(argv[3]);
	table->time_to_sleep = ft_atol(argv[4]);
	if (table->philosophers < 1)
		error_and_exit("Use at least one philosopher!");
	if (table->time_to_die < 60 || table->time_to_eat < 60
		|| table->time_to_sleep < 60)
		error_and_exit("Use values greater than 60ms!");
	if (argv[5])
		table->meals_limit = ft_atol(argv[5]);
	else
		table->meals_limit = -1;
}
