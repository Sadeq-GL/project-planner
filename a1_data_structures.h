/********* a1_data_structures.h ********
	
*/
#include <stdbool.h>
#ifndef A1_DATA_STRUCTURES_H
#define A1_DATA_STRUCTURES_H
#define MAX_ACTIVITIES_PER_MILESTONE 3
#define MAX_MILESTONE 5
/********** DON'T MODIFY FROM HERE **********/

typedef struct activity {
    unsigned short int id;
    char name[100];
    float planned_cost, actual_cost;
    unsigned short int planned_duration, actual_duration;
    _Bool completed;
}activity_t;

typedef struct milestone {
    unsigned short int id;
    char name[100];
    activity_t activity_list[MAX_ACTIVITIES_PER_MILESTONE];

    _Bool completed;
    float actual_cost;
    short int actual_duration;
}milestone_t;

typedef struct project {
    char name[100]; 
    float planned_cost, actual_cost;
    unsigned short int planned_duration, actual_duration;
    _Bool completed;
}project_t;

/********** DON'T MODIFY UNTIL HERE **********/
#endif
