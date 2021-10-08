#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <malloc.h>
#include <time.h>
#include <string.h>

struct node
{
	int priority;
	char query[6];
	char query_type[10];
	int time_exe;
	int arrived;
	struct node *next;
};
typedef struct node Query;

typedef struct {
	Query* list;
	Query* current;			//the query that is currently executed					
	float avg_wait;			//avarage waiting time				
	float avg_exe;			//average executing time			
	int queries_executed;		//number of queries executed		
} List;

const char * type[] = {"update","select","join"}; 

struct node *create_ll(struct node *);
struct node *display(struct node *);
struct node *insert_beg(struct node *,Query );
struct node *delete_beg(struct node *);
struct node *sort_list(struct node *);

int main(int argc, char *argv[]) {
	srand(time(NULL));
	int next_query;
	float m1=0,m2=0,m3=0;
	next_query = rand()%5 + 1;
	List procs[3];					// One list for every processor			
	int i;												
	for (i=0;i<3;i++){				// processors initialization					
		procs[i].list = NULL;
		procs[i].current = NULL;
		procs[i].avg_wait=0.0;
		procs[i].queries_executed=0;
		procs[i].avg_exe = 0.0;
	}
	
	int t;
	//Loop for the discrete clock. If we are in time t = 12 and produce the number 3, the next question will arrive in time 15.
	for ( t=0;t<100;t++) {			
		printf("t = %d\n",t);
		// Query arrival check
		if(next_query == t) {
			printf("**QUERY ARRIVED**\n");
			Query q;
			int pos,proc;        				//pos = position on the query type array, proc = which processor it is going to be assigned to					
			
			q.priority = rand()%5 + 1;				
			strcpy(q.query,"query");						
			pos = rand()%3;
			strcpy(q.query_type,type[pos]);
			q.arrived=t;
			q.time_exe = rand()%10 + 1;
			
			proc = rand()%3 ;
			printf("p = %d\n",pos);										
			procs[proc].list = insert_beg(procs[proc].list,q);
			next_query = t + rand()%5 + 1;
		}
		
		// Termination check
		for ( i=0;i<3;i++) {
			
			if (procs[i].current == NULL) {
				procs[i].current = procs[i].list;
				procs[i].list = delete_beg(procs[i].list);
				continue;
			}
			procs[i].avg_exe += 1;
			if (--procs[i].current->time_exe == 0) {
				printf("**QUERY TERMINATED**\n");
				procs[i].queries_executed++ ;					
				procs[i].list = sort_list(procs[i].list);		//Sort to take the next query with priority			
				procs[i].current = procs[i].list;
				if (procs[i].list == NULL ) continue;
				procs[i].avg_wait += t - procs[i].current->arrived;
				procs[i].list = delete_beg (procs[i].list);
			}
		}
		
		// Print average wait time-execution time
		for (i=0;i<3;i++) {
			if (procs[i].queries_executed > 0) {
				printf("AVG wait time for processor %d: %f\n",i,procs[i].avg_wait/procs[i].queries_executed);
				printf("AVG execution time for processor %d: %f\n",i,procs[i].avg_exe/procs[i].queries_executed);
				printf("\n");
			}
		}
		
		// At each discrete step, examine the queues and in case a query expects more than 10 discrete steps then its priority should be increased by 1 and it will move to the correct position within the queues.
		for (i=0;i<3;i++) {
			Query *tmp = procs[i].list;
			while(tmp != NULL) {
				if (t - tmp->arrived > 10 ) tmp->priority++;
				tmp = tmp->next;
			}
			procs[i].list = sort_list(procs[i].list);
		}
		
		// Calculates and displays the number of each query type (update, select, join) in each queue.
		int NUMstr1;
		int NUMstr2;
		int NUMstr3;
		char str1[10]="update";
		char str2[10]="select";
		char str3[10]="join";
		for (i=0;i<3;i++) {
			Query *tmp = procs[i].list;
			while(tmp != NULL) {
				if (!strcmp(tmp->query_type,str1))
					NUMstr1++;
				else if(!strcmp(tmp->query_type,str2))
					NUMstr2++;
				else if(!strcmp(tmp->query_type,str3))
					NUMstr3++;
					
				tmp = tmp->next;
			}
			printf("The number of update types is : %d \n",NUMstr1);
			printf("The number of select types is : %d \n",NUMstr2);
			printf("The number of join types is : %d \n \n",NUMstr3);
			procs[i].list = sort_list(procs[i].list);
		}
		
	}
}

struct node *create_ll(struct node *start)
{
	struct node *new_node, *ptr;
	int num;
	printf("\n Enter -1 to end");
	printf("\n Enter the priority : "); 
	scanf("%d", &num);
	while(num!=-1)
	{
		new_node = (struct node*)malloc(sizeof(struct node));
		new_node -> priority=num;
		if(start==NULL)
		{
			new_node -> next = NULL;
			start = new_node;
		}
		else
		{
			ptr=start;
			while(ptr->next!=NULL)
				ptr=ptr->next;
			ptr->next = new_node;
			new_node->next=NULL;
		}
		printf("\n Enter the priority : ");
		scanf("%d", &num);
	}
	return start;
}

struct node *display(struct node *start)
{
	struct node *ptr;
	ptr = start;
	while(ptr != NULL)
	{
		printf("\t %d", ptr -> priority);
		ptr = ptr -> next;
	}
	return start;
}

struct node *insert_beg(struct node *start,Query q)
{
	if (start == NULL) {
		start = malloc(sizeof(Query));
		start -> priority = q.priority;
		strcpy(start -> query ,q.query);
		strcpy(start->query_type,q.query_type);
		start->time_exe = q.time_exe;
		start->arrived = q.arrived;
		
		start->next = NULL;
		return start;
	}
	struct node *new_node;
	new_node = (struct node *)malloc(sizeof(struct node));
	new_node -> priority = q.priority;
	strcpy(new_node -> query ,q.query);
	strcpy(new_node->query_type,q.query_type);
	new_node->time_exe = q.time_exe;
	new_node->arrived = q.arrived;
	new_node -> next = start;
	start = new_node;
	return start;
}

struct node *delete_beg(struct node *start)
{
	if(start==NULL) return start;
	struct node *ptr;
	ptr = start;
	start = start -> next;
	free(ptr);
	return start;
}


struct node *sort_list(struct node *start)
{
	if (start==NULL) return start;
	struct node *ptr1, *ptr2;
	int temp;
	ptr1 = start;
	while(ptr1 -> next != NULL)
	{
		ptr2 = ptr1 -> next;
		while(ptr2 != NULL)
		{
			if(ptr1 -> priority < ptr2 -> priority)
			{
				temp = ptr1 -> priority;							
				ptr1 -> priority = ptr2 -> priority;	
				ptr2 -> priority = temp;
				
				temp = ptr1->arrived;							
				ptr1->arrived = ptr2->arrived;			//Change in the fields after sort
				ptr2 -> arrived = temp;
				
				temp = ptr1->time_exe;									
				ptr1->time_exe = ptr2->time_exe;
				ptr2->time_exe = temp;
				
				char tmp[10];
				strcpy(tmp,ptr1->query_type);
				strcpy(ptr1->query_type,ptr2->query_type);
				strcpy(ptr2->query_type,tmp);
			}
			ptr2 = ptr2 -> next;
		}
		ptr1 = ptr1 -> next;
	}
	return start; 
}
