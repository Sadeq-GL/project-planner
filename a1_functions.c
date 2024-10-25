/********* a1_functions.c ********
	

*/
#include "a1_functions.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>


#define DAYHOURS 8
#define AVGPAY 30

unsigned short int get_input_usi(void) {
    unsigned short int input_value;

    // Read input from the user
    scanf("%hu", &input_value);

    // Keep asking for input until a non-negative value is entered
    while (input_value < 0) {

        printf("\nEnter a positive value: ");
        scanf("%i", &input_value);
    }

    return input_value;  // Return the valid input as unsigned short int
}

float get_input_f(void) {
    float input;
    do {
        scanf("%f", &input);
        if (input < 0) {
            printf("\nPlease enter a positive value: ");
        }
    } while (input < 0);
    return input;
}



// checks the ids if they are unique
_Bool check_id(unsigned short int id, milestone_t *milestones, int num_milestones, int current_milestone, int current_activity, bool is_milestone) {
    _Bool is_unique = true;  // Default to true which is unique
    
    

    // Loop through all milestones id and compares them to see if any ids are the same as the one entered
    if(is_milestone){
        for (int i=0; i< num_milestones; i++ ){
            if (i != current_milestone && milestones[i].id == id){
                is_unique = false;//found same id
                break;
            }
        }
    }else{// compares the activities id to see if any two are the same
        for (int i = 0; i < num_milestones; i++) {
            for (int j = 0; j < MAX_ACTIVITIES_PER_MILESTONE; j++) {
                // Skip the current activity being checked (to avoid self-check)
                if (i == current_milestone && j == current_activity) {
                    continue;
                }

                // If ID matches, set is_unique to false
                if (milestones[i].activity_list[j].id == id) {
                    is_unique = false;
                    break;
                }
            }

            if (!is_unique) {
                break;
            }
        }
    }
    return is_unique;
}

void init_activity(activity_t * activity_to_int){
    // Get the activity name
    printf("Enter the activity name: ");
    scanf("%s", activity_to_int->name);

    // Get the activity ID
    //printf("Enter the activity id. IDs must be unique: ");
    //activity->id = get_input_usi();  // Store the activity ID

    // Set initial values for activity cost and completion status
    activity_to_int->actual_cost = 0;
    activity_to_int->completed = false;

    // Get the planned duration for the activity
    printf("Enter the planned duration in hours: ");
    activity_to_int->planned_duration = get_input_usi();  

    // Calculate and set the planned cost based on duration (e.g., hourly rate of 30)
    activity_to_int->planned_cost = (float)(activity_to_int->planned_duration * AVGPAY);
}

void init_milestone(milestone_t * milestone_to_int, unsigned short int num_activities){
    

    // Get the milestone's name
    printf("Enter the milestone name: ");
    scanf("%s", milestone_to_int->name);

    // Get the milestone ID (assuming uniqueness is checked elsewhere)
    //printf("Enter the milestone id. IDs must be unique: ");
    //milestone_to_int->id = get_input_usi();  // Store the milestone ID

    milestone_to_int->actual_cost = 0;       
    milestone_to_int->completed = false;      

    // Initialize activities within this milestone
    for (int i = 0; i < num_activities; i++) {
        init_activity(&milestone_to_int->activity_list[i]);  // Initialize each activity
    }
}

project_t init_project(char name[], milestone_t *milestone_list, int number_milestones, const int * number_activities) {
    project_t new_p;
    float totalpC = 0.0;
    float totalpD = 0.0;

   
    strcpy(new_p.name, name);

    // Calculate total planned cost and duration
    for (int i = 0; i < number_milestones; i++) {
        for (int j = 0; j < number_activities[i]; j++) {
            totalpC += milestone_list[i].activity_list[j].planned_cost;
            totalpD += milestone_list[i].activity_list[j].planned_duration;
        }
    }

   
    new_p.planned_cost = totalpC;
    new_p.planned_duration = (unsigned short int)ceilf(totalpD / DAYHOURS);  // Convert hours to days
    new_p.completed = false;

    return new_p;
}

void print_main_menu(void) {
    printf("\nPlease select one of the following!");
    printf("\n1. Update activity");
    printf("\n2. Print stats");
    printf("\n3. Exit");
}

void print_milestone_stats(const milestone_t * list_milestones, int num_milestones, const int * number_activities) {
    for (int i = 0; i < num_milestones; i++) {
        float planned_cost = 0.0;
        unsigned int planned_duration = 0;

        // Calculate total planned cost and duration for this milestone
        for (int j = 0; j < number_activities[i]; j++) {
            planned_cost += list_milestones[i].activity_list[j].planned_cost;
            planned_duration += list_milestones[i].activity_list[j].planned_duration;
        }

        if (!list_milestones[i].completed) {
            // Milestone is not completed yet
            printf("Milestone %s is not completed yet.\n", list_milestones[i].name);
            int completed_activities = 0;

            // Count completed activities
            for (int j = 0; j < number_activities[i]; j++) {
                if (list_milestones[i].activity_list[j].completed) {
                    completed_activities++;
                }
            }

            printf("%d out of %d activities completed.\n", completed_activities, number_activities[i]);
            printf("The milestone planned duration is %d hours.\n", planned_duration);
            printf("The milestone planned cost is $%.2f.\n", planned_cost);
        } else {
            // Milestone is completed
            printf("Milestone %s is completed.\n", list_milestones[i].name);
            
            float cost_difference = list_milestones[i].actual_cost - planned_cost;
            int duration_difference = list_milestones[i].actual_duration - planned_duration;

            // Prints budget 
            if (cost_difference >= 0) {
                printf("Finished $%.2f overbudget and ", cost_difference);
            } else {
                printf("Finished $%.2f underbudget and ", -cost_difference);
            }

            // Prints time 
            if (duration_difference < 0) {
                printf("%d days early.\n", (int)ceil(duration_difference / DAYHOURS));
            } else {
                printf("%d days late.\n", (int)ceil(duration_difference / DAYHOURS));
            }
        }
    }
}

void print_project_stats(const project_t details, const milestone_t * list_milestones, int num_milestones, const int * number_activities) {
    printf("Project: %s\n", details.name);

    if (!details.completed) {
        // If the project is not completed
        puts("The project is not completed.");
        print_milestone_stats(list_milestones, num_milestones, number_activities);
    } else {
        // If the project is completed
        puts("The project is completed.");

        // Calculate budget difference
        float cost_difference = details.actual_cost - details.planned_cost;
        unsigned short int duration_difference = details.actual_duration - details.planned_duration;

        // Print budget status
        if (cost_difference >= 0) {
            printf("Finished $%.2f overbudget and ", cost_difference);
        } else {
            printf("Finished $%.2f underbudget and ", -cost_difference);
        }

        // Print time status
        if (duration_difference < 0) {
            printf("%d days early.\n", -duration_difference);
        } else {
            printf("%d days late.\n", duration_difference);
        }
    }
}

void update_activity(activity_t * activity_to_update) {
    float actual_duration;
    char is_finished;

    // Get the actual duration from the user
    printf("Enter the actual duration: ");
    actual_duration = get_input_f();
    activity_to_update->actual_duration = (unsigned short int)actual_duration;  // Store as short int

    // Check if the activity is finished
    printf("Is the activity finished? (Y/N): ");
    scanf(" %c", &is_finished);

    // Update the completed status based on the user's input
    if (is_finished == 'Y' || is_finished == 'y') {
        activity_to_update->completed = true;
    }

    // Calculate the actual cost based on the duration 
    activity_to_update->actual_cost = activity_to_update->actual_duration * AVGPAY;
}

void update_milestone(milestone_t * milestone_to_update, int activities_in_milestone) {
    bool all_activities_completed = true;
    float total_actual_cost = 0.0;
    short total_actual_duration = 0;

    // Loop through all activities in the milestone
    for (int i = 0; i < activities_in_milestone; i++) {
        if (!milestone_to_update->activity_list[i].completed) {
            all_activities_completed = false;  // If any activity is not completed, mark as incomplete
            break;  // No need to check further if one activity is incomplete
        }
    }

    if (all_activities_completed) {
        milestone_to_update->completed = true;  // Mark milestone as complete

        // Sum up the actual costs and durations
        for (int i = 0; i < activities_in_milestone; i++) {
            total_actual_cost += milestone_to_update->activity_list[i].actual_cost;
            total_actual_duration += milestone_to_update->activity_list[i].actual_duration;
        }

        milestone_to_update->actual_duration = total_actual_duration;
        milestone_to_update->actual_cost = total_actual_cost;
    }
}

void update_project(const milestone_t * milestone_array, int num_milestones, const int * number_activities, project_t * my_project) {
    float total_actual_cost = 0.0;
    float total_actual_duration = 0.0;
    bool all_milestones_completed = true;

    // Loop through all milestones
    for (int i = 0; i < num_milestones; i++) {
        total_actual_cost += milestone_array[i].actual_cost;
        total_actual_duration += milestone_array[i].actual_duration;

        // Check if all activities in this milestone are completed
        for (int j = 0; j < number_activities[i]; j++) {
            if (!milestone_array[i].activity_list[j].completed) {
                all_milestones_completed = false;
                break;
            }
        }
    }

    if (all_milestones_completed) {
        my_project->actual_cost = total_actual_cost;
        my_project->actual_duration = (unsigned short int)ceil(total_actual_duration / DAYHOURS);  
        my_project->completed = true;
    } else {
        my_project->actual_cost = 0;
        my_project->actual_duration = 0;
        my_project->completed = false;
    }
}




