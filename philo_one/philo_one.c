/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_one.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obouykou <obouykou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/28 19:25:01 by obouykou          #+#    #+#             */
/*   Updated: 2021/04/10 16:17:46 by obouykou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/philo_one.h"

void	*philo(void *raw_data)
{
	t_data			*data;
	t_philo			*p;
	pthread_t		thrd;

	p = (t_philo *)raw_data;
	data = p->data;
	p->limit = get_time(0U) + data->time2die;
	pthread_create(&thrd, NULL, dying_checker, p);
	pthread_detach(thrd);
	while (!p->done)
	{
		take_forks(data, p);
		tasks(data, p);
		if (p->num_of_eating == data->eating_times)
		{
			++data->done_eatings;
			p->done = 1;
		}
	}
	return (raw_data);
}

int	simulate(t_data *data)
{
	unsigned int	i;
	pthread_t		thrd;

	if (data->eating_times != -1)
	{
		pthread_create(&data->eating_checker, NULL, eating_checker, data);
		pthread_detach(data->eating_checker);
	}
	pthread_mutex_lock(&data->mutex_philo);
	i = 0;
	while (i < data->num_of_philo)
	{
		pthread_create(&thrd, NULL, philo, &data->philos[i]);
		pthread_detach(thrd);
		usleep(50);
		i++;
	}
	pthread_mutex_lock(&data->mutex_philo);
	pthread_mutex_unlock(&data->mutex_philo);
	return (0);
}

int	main(int ac, char **av)
{
	t_data	*data;

	if (ac != 5 && ac != 6)
	{
		printf("USAGE: ./philo_one  number_of_philosophers  time_to_die");
		printf("  time_to_eat time_to_sleep");
		printf("  [number_of_times_each_philosopher_must_eat]\n");
		return (1);
	}
	data = (t_data *)malloc(sizeof(t_data));
	if (!data)
	{
		printf("Memory allocation error\n");
		return (1);
	}
	if (get_args(ac, av, data) || init_data(data))
		return (1);
	simulate(data);
	clean(data);
	return (0);
}
