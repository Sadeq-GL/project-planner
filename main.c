/********* main.c ********
	
	Student Name 	= sadeq shkab
	Student Number	= 101285707
*/

//Complete the includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "a1_functions.h"

#define MAX_MILESTONES 5
#define MAX_ACTIVITIES_PER_MILESTONE 3

int main() {
    unsigned int num_milestones;
    milestone_t milestones[MAX_MILESTONES];
    int activities[MAX_MILESTONES];  // Store the number of activities for each milestone

    // Project Name
    char project_name[100];
    printf("\n**** welcome to project builder ****");
    printf("\nEnter the project name: ");
    scanf("%s", project_name);

    // Number of milestones
    printf("\nEnter the number of milestones (up to 5): ");
    num_milestones = get_input_usi();
    while (num_milestones == 0 || num_milestones > MAX_MILESTONES) {
        printf("\nInvalid input. Enter the number of milestones (up to 5): ");
        num_milestones = get_input_usi();
    }

    int total_activities = 0;

    // Initialize each milestone
    for (int i = 0; i < num_milestones; i++) {
        printf("\n*** Initializing milestone %d ***", i + 1);

        printf("\nEnter milestone %d ID (must be unique): ", i + 1);
        milestones[i].id = get_input_usi();

    // Check if the milestone ID is unique across all milestones
        while (!check_id(milestones[i].id, milestones, num_milestones, i, 0, true)) {
            printf("\nMilestone ID is not unique, enter a valid ID: ");
            milestones[i].id = get_input_usi();
    }
        
        // Number of activities for this milestone
        printf("\nEnter the number of activities for milestone %d (up to 3): ", i + 1);
        activities[i] = get_input_usi();
        while (activities[i] == 0 || activities[i] > MAX_ACTIVITIES_PER_MILESTONE) {
            printf("\nInvalid input. Enter the number of activities (up to 3): ");
            activities[i] = get_input_usi();
        }

        total_activities += activities[i];

        // Initialize the milestone and its activities
        init_milestone(&milestones[i], activities[i]);

        // Check activityies IDs uniqueness 
        for (int j = 0; j < activities[i]; j++) {
            printf("\nEnter activity ID %d for milestone %d: ", j + 1, i + 1);
            milestones[i].activity_list[j].id = get_input_usi();

            while (!check_id(milestones[i].activity_list[j].id, milestones, num_milestones, i, j, false)) {
                printf("\nActivity ID is not unique, enter a valid ID: ");
                milestones[i].activity_list[j].id = get_input_usi();
            }
        }
    }

    // Initialize the project
    project_t project = init_project(project_name, milestones, num_milestones, activities);

    // Main menu loop
    int choice;
    do {
        print_main_menu();
        printf("\nPlease enter your choice: ");
        choice = get_input_usi();

        if (choice == 1) {
            // Updating an activity
            printf("\nWhich activity do you want to update? (Enter the ID only) ");
            printf("\nActivities available for update:\n");

            for (int i = 0; i < num_milestones; i++) {
                for (int j = 0; j < activities[i]; j++) {
                    if (!milestones[i].activity_list[j].completed) {  // Check if activity is incomplete
                        printf("Milestone %d - Activity %s (ID: %d)\n", i + 1, milestones[i].activity_list[j].name, milestones[i].activity_list[j].id);
                    }
                }
            }

            unsigned short int activity_id = get_input_usi();
            bool found_activity = false;

            int milestone_index = -1;
            int activity_index = -1;

            // Search for the activity by ID
            for (int i = 0; i < num_milestones; i++) {
                for (int j = 0; j < activities[i]; j++) {
                    if (milestones[i].activity_list[j].id == activity_id && !milestones[i].activity_list[j].completed) {
                        milestone_index = i;
                        activity_index = j;
                        found_activity = true;
                        break;
                    }
                }
                if (found_activity) break;
            }

            if (found_activity) {
                update_activity(&milestones[milestone_index].activity_list[activity_index]);
                update_milestone(&milestones[milestone_index], activities[milestone_index]);
                update_project(milestones, num_milestones, activities, &project);
            } else {
                printf("\nInvalid activity ID or the activity is already completed.");
            }

        } else if (choice == 2) {
            // Print project statistics
            print_project_stats(project, milestones, num_milestones, activities);
        } else if (choice == 3) {
            // Exit the program
            printf("\nExiting the program.");
        } else {
            printf("\nInvalid option, please choose again.\n");
        }

    } while (choice != 3);

    return 0;
}
