#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <assert.h>
#include "list.h"

typedef  void (*FUNC)(int);

typedef struct TIMER {
	LIST list;
	int num;
	FUNC func;
} TIMER;

TIMER head;

void b(int a)
{
	printf("timer start %s\n", __func__);
}

void a(int b)
{
	printf("timer start %s\n", __func__);
}

int user_timer_create(int second, FUNC func)
{
	TIMER *t = malloc(sizeof(TIMER));
	t->num = second;
	t->func = func;
	list_insert_behind(&head.list, &t->list);
	return 0;
}

void sigroutine(int signo)
{
	LIST *tmp = &head.list;
	TIMER *d;
	while (!is_list_last(tmp)) {
		d = list_entry(tmp->next, TIMER, list);
		tmp = tmp->next;
		if (d->num == 0)
			d->func(0);
		else
			d->num--;
	}
}

int system_clock_init()
{
	int second  = 1;
	struct itimerval value, ovalue;

	printf("process id is %d\n", getpid());
	signal(SIGALRM, sigroutine);
	value.it_value.tv_sec = second;
	value.it_value.tv_usec = 0;
	value.it_interval.tv_sec = second;
	value.it_interval.tv_usec = 0;
	setitimer(ITIMER_REAL, &value, &ovalue);

	return 0;
}

int main()
{
	list_init(&head.list);
	system_clock_init();

	user_timer_create(1, b);
	user_timer_create(2, a);
	for (;;);
	return 0;
}
