#include <stdlib.h>
#include <conio.h>
#include <random>
#include <time.h>

#define NumberOfStudents 10
#define AmountOfCourses 5

struct COURSE
{
	int NumberOfCourse;
	COURSE *next;
};

struct LIST0
{
	COURSE* next;
	LIST0* lnext;
};

struct LIST
{
	int a;
	LIST0 *cnext;
	LIST *next;
};

void CreateList(LIST *current_list, int i)
{
	current_list->a=i;
	current_list->next=NULL;
}	


void CreateList(COURSE *current_list, int i)
{
	current_list->NumberOfCourse=i;
	current_list->next=NULL;
}


COURSE* GetCount(COURSE *current_list, int n)
{
	for(int i = 0; i < n; ++i)
	{
		current_list = current_list->next;
		if (!current_list) {
			
			printf("Error\n");
			return 0;
		}
	}

	return current_list;
}

void PrintList(LIST *current_list)
{
	current_list = current_list->next;
	LIST0* list0;

	while (current_list) {
	
		list0 = current_list->cnext;
		printf("Student %d ",current_list->a);
		current_list = current_list->next;
		printf("Courses ");

		while(list0->lnext) {
		
			printf("%d ",list0->next->NumberOfCourse);
			list0 = list0->lnext;
		}
		printf("\n");
	}
	printf("\n");
}

void PrintList(COURSE *current_list)
{
	current_list = current_list->next;
	
	while(current_list) {

		printf("%d ",current_list->NumberOfCourse);
		current_list = current_list->next;
	}
	printf("\n");
}

void PrintListAll(LIST *current_list,COURSE *current_list0,LIST *First)
{
	current_list0=current_list0->next;


	while(current_list0) {

		current_list = First;
		current_list = current_list->next;
		printf("Course %d ",current_list0->NumberOfCourse);
		printf("Students ");
		while(current_list) {

			LIST0 *list0;
			list0 = current_list->cnext;

			while(list0->lnext) {
			
				if(list0->next->NumberOfCourse == current_list0->NumberOfCourse) {

					printf("%d ",current_list->a);
					break;
				}
				list0 = list0->lnext;
			}
			current_list = current_list->next;
		}
		current_list0 = current_list0->next;
		printf("\n");
	}
}

void main()
{
	LIST *current_list,*First=new LIST;
	COURSE *course_current_list,*cFirst=new COURSE;
	First->next = NULL;
	First->cnext = NULL;
	cFirst->next = NULL;
	srand(time(NULL));

	for(int i = 0; i < NumberOfStudents; ++i) {
		
		if(!First->next) {
			current_list=new LIST;
			First->next=current_list;

		} else {

			current_list->next=new LIST;
			current_list=current_list->next;
		}
		CreateList(current_list,i);
		current_list->cnext=new LIST0;
	}
	current_list=First;

	for(int i = 0; i < AmountOfCourses; ++i) {
		
		if(!cFirst->next) {
			
			course_current_list=new COURSE;
			cFirst->next=course_current_list;
		} else {
			
			course_current_list->next=new COURSE;
			course_current_list=course_current_list->next;
		}
		CreateList(course_current_list,i);
	}
	course_current_list=cFirst;


	LIST0* current_list0;
	current_list=current_list->next;
	while(current_list) {

		current_list0 = current_list->cnext;
		int k1 = rand() % 4;
		int k = k1+rand() % (AmountOfCourses - k1) + 1;
		
		for(int i = k1; i < k; ++i) {
			current_list0->next = GetCount(course_current_list,i + 1);
			current_list0->lnext = new LIST0;
			current_list0 = current_list0->lnext;
			current_list0->lnext = NULL;
		}

		current_list0->lnext = NULL;
		current_list = current_list->next;
	}

	course_current_list = cFirst;
	current_list = First;
	PrintList(current_list);
	printf("\n");
	course_current_list = cFirst;
	current_list = First;
	PrintListAll(current_list, course_current_list, First);

	getch();
}