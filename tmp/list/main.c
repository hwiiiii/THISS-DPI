#include <stdio.h>
#include "list.h"

int main()
{
    PA_table *head = INIT_TABLE();
    PA_table *tmp;// = INIT_TABLE();

    int i = 0;
	int j;

    printf("1 : add table\n");
    ADD_TABLE(head, 1, 5);
    printf("2 : table added\n");
    printf("3 : head->address : ");
    printf("%d\n", head->address);
    printf("4 : head->target : ");
    printf("%d\n", head->target);
    printf("5 : head->next : %d\n", head->next);
    printf("6 : next->address : ");
    printf("%d\n", (head->next)->address);
    printf("7 : next->next : %d\n", (head->next)->next);
    printf("8 : next->target : ");
    printf("%d\n", (head->next)->target);

	printf("\n1 : add 100 tables\n");
	for (i = 50 ; i <100; i++)
	{
		ADD_TABLE(head, i, i*2);
	}

	printf("2 : print all table\n");
	tmp = head->next;
	i = 0;
	while (tmp->next != NULL)
	{
		printf("	%d : %d : %d\n",i,tmp->address, tmp->target);
		i++;
		tmp = tmp->next;
	}

	printf("3 : remove 10 even address\n");
	for(i=0;i<10;i = i+2)
	{
		printf("	%d is pulled : %d\n", 50+i, PULL_TABLE(head, 50+i));
	}
	printf("4 : scan all tables\n");
	tmp = head->next;
	i = 0;
	while (tmp->next != NULL)
	{
		printf("	%d : %d : %d\n",i,tmp->address, tmp->target);
		i++;
		tmp = tmp->next;
	}
	
	printf("5 : try to pull wrong address\n");
	printf("	%d is pulled : %d\n", 50, PULL_TABLE(head, 30));
	

		

    return 0;

}
    
