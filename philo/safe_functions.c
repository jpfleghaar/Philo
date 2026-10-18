/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   safe_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpflegha <jpflegha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 12:34:52 by jpflegha          #+#    #+#             */
/*   Updated: 2025/05/07 11:59:58 by jpflegha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*safe_malloc(size_t bytes)
{
	void	*ret;

	ret = malloc(bytes);
	if (NULL == ret)
		error_and_exit("Error with malloc");
	return (ret);
}

static void	mutex_error(int status, t_opcode opcode)
{
	if (0 == status)
		return ;
	if (EINVAL == status && (INIT == opcode))
		error_and_exit("The value specified by attr is invalid");
	else if (EDEADLK == status)
		error_and_exit("A deadlock would occur if the"
			"thread blocked waiting for mutex.");
	else if (EPERM == status)
		error_and_exit("The current thread does not hold a lock on mutex.");
	else if (ENOMEM == status)
		error_and_exit("Proccess cannot allocate enough memory to"
			"create another mutex.");
	else if (EBUSY == status)
		error_and_exit("Mutex is locked");
}

void	safe_mtx(t_mtx *mutex, t_opcode opcode)
{
	if (INIT == opcode)
		mutex_error(pthread_mutex_init(mutex, NULL), opcode);
	else if (DESTROY == opcode)
		mutex_error(pthread_mutex_destroy(mutex), opcode);
	else
		error_and_exit("Wrong opcode for mutex handle");
}

static void	thread_error(int status, t_opcode opcode)
{
	if (0 == status)
		return ;
	else if (EAGAIN == status)
		error_and_exit("No resources to create another thread.");
	else if (EPERM == status)
		error_and_exit("The caller does not have appropriate permisson.\n");
	else if (EINVAL == status)
		error_and_exit("The value specified by attr is invalid.");
	else if (EINVAL == status && (JOIN == opcode || DETACH == opcode))
		error_and_exit("The value specified by thread is not joinable.\n");
	else if (ESRCH == status)
		error_and_exit("No thread could be found corresponding to that"
			"specified by the given thread ID, thread.");
	else if (EDEADLK == status)
		error_and_exit("A deadlock was detected or the value of "
			"thread specifies the calling thread.");
}

void	safe_thread(pthread_t *thread, void *(*foo)(void *), void *data,
		t_opcode opcode)
{
	if (CREATE == opcode)
		thread_error(pthread_create(thread, NULL, foo, data), opcode);
	else if (JOIN == opcode)
		thread_error(pthread_join(*thread, NULL), opcode);
	else if (DETACH == opcode)
		thread_error(pthread_detach(*thread), opcode);
	else
		error_and_exit("Wrong opcode for thread_handle:"
			" use <CREATE> <JOIN> <DETACH>");
}
