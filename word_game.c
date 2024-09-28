#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <string.h>
#include <windows.h>

#define MAX_PLACES 1000 // Define a maximum number of places
#define MAX_PLACE_LEN 60
#define MAX_NAME_LEN 50 // Maximum length for player names

// Function to convert a string to lowercase
void toLowerCase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

// Function to get a random letter, excluding 'x'
char getRandomLetter() {
    char letter;
    do {
        letter = 'a' + (rand() % 26);
    } while (letter == 'x'); // Exclude 'x'
    return letter;
}

// Function to load places from a file into an array
int loadPlacesFromFile(char places[][MAX_PLACE_LEN], const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error opening file '%s'\n", filename);
        return 0;
    }

    int count = 0;
    while (fgets(places[count], MAX_PLACE_LEN, file)) {
        // Remove newline character
        places[count][strcspn(places[count], "\n")] = 0;
        count++;
        if (count >= MAX_PLACES) {
            break; // Stop if the limit is reached
        }
    }

    fclose(file);
    return count;
}

// Function to check if a place is in the predefined list
int isPlaceValid(char *place, char places[][MAX_PLACE_LEN], int placeCount) {
    for (int i = 0; i < placeCount; i++) {
        char temp[MAX_PLACE_LEN];
        strcpy(temp, places[i]);
        toLowerCase(temp);
        if (strcmp(temp, place) == 0) {
            return 1; // Valid place
        }
    }
    return 0; // Invalid place
}

// Function to check if a place is already used
int isAlreadyUsed(char *place, char usedCountries[][20], int usedCount) {
    for (int i = 0; i < usedCount; i++) {
        if (strcmp(usedCountries[i], place) == 0) {
            return 1; // Already used
        }
    }
    return 0; // Not used
}

// Function to run the timer
DWORD WINAPI show_timer(LPVOID lpParam) {
    int *timeLimit = (int *)lpParam;
    Sleep(*timeLimit * 1000); // Wait for the defined seconds based on the difficulty
    printf("\nTime's up!\n");
    exit(0); // End the game if time is up
    return 0;
}

// Function to update and display the leaderboard
void updateLeaderboard(char player1Name[], char player2Name[], int player1Score, int player2Score) {
    printf("\n===== Leaderboard =====\n");
    printf("%s Score: %d\n", player1Name, player1Score);
    printf("%s Score: %d\n", player2Name, player2Score);

    if (player1Score > player2Score) {
        printf("%s is the Winner!\n", player1Name);
    } else if (player2Score > player1Score) {
        printf("%s is the Winner!\n", player2Name);
    } else {
        printf("It's a Draw!\n");
    }
    printf("========================\n");
}

int main() {
    srand(time(0)); // Seed the random number generator

    char usedCountries[100][20]; // To store used countries
    int usedCount = 0;

    char places[MAX_PLACES][MAX_PLACE_LEN]; // Array to store places from file
    int placeCount = loadPlacesFromFile(places, "places.txt"); // Load places from the file
    if (placeCount == 0) {
        printf("No places loaded. Exiting...\n");
        return 1;
    }

    // Player name inputs
    char player1Name[MAX_NAME_LEN], player2Name[MAX_NAME_LEN];
    printf("\nEnter Player 1 name: ");
    fgets(player1Name, MAX_NAME_LEN, stdin);
    player1Name[strcspn(player1Name, "\n")] = 0; // Remove newline

    printf("Enter Player 2 name: ");
    fgets(player2Name, MAX_NAME_LEN, stdin);
    player2Name[strcspn(player2Name, "\n")] = 0; // Remove newline

    // Difficulty level selection
    int timeLimit;
    int choice;
    printf("\nSelect Difficulty Level:\n");
    printf("1. Easy (15 seconds)\n");
    printf("2. Medium (10 seconds)\n");
    printf("3. Hard (5 seconds)\n");
    printf("Enter your choice (1-3): ");
    scanf("%d", &choice);
    getchar(); // Consume newline left by scanf

    switch (choice) {
        case 1:
            timeLimit = 15;
            break;
        case 2:
            timeLimit = 10;
            break;
        case 3:
            timeLimit = 5;
            break;
        default:
            printf("Invalid choice. Defaulting to Medium (10 seconds).\n");
            timeLimit = 10;
            break;
    }

    char firstLetter = getRandomLetter();
    char player1Country[60], player2Country[60];
    char lastLetter;
    int questionCount = 0;
    int player1Score = 0, player2Score = 0; // Initialize scores

    printf("\nRandom letter generated: %c\n", firstLetter);

    while (1) {
        questionCount++;

        // Player 1 input
        printf("Remaining time: %d seconds\n", timeLimit);
        printf("\n%s, enter a place starting with '%c': ", player1Name, firstLetter);
        fflush(stdout); // Ensure the prompt is printed before the timer starts

        HANDLE hThread = CreateThread(NULL, 0, show_timer, &timeLimit, 0, NULL); // Start the timer in a separate thread

        // Wait for input
        fgets(player1Country, sizeof(player1Country), stdin);
        TerminateThread(hThread, 0); // Stop the timer when input is received
        player1Country[strcspn(player1Country, "\n")] = 0; // Remove newline character
        toLowerCase(player1Country);

        // Clear the timer line
        printf("\r                                          \r");

        int player1Invalid = 0; // Track if Player 1 has an invalid input

        if (player1Country[0] != firstLetter) {
            printf("Place does not start with %c.\n", firstLetter);
            player1Invalid = 1; // Mark as invalid
        }

        // Check if player1Country is valid
        if (!isPlaceValid(player1Country, places, placeCount)) {
            printf("Invalid place.\n");
            player1Invalid = 1; // Mark as invalid
        }

        // Check if player1Country is already used
        if (isAlreadyUsed(player1Country, usedCountries, usedCount)) {
            printf("Place already used.\n");
            player1Invalid = 1; // Mark as invalid
        }

        // Player 1's turn result
        if (player1Invalid) {
            printf("Invalid input by %s.\n", player1Name);
        } else {
            // Add to used list
            strcpy(usedCountries[usedCount++], player1Country);
            player1Score++; // Increment Player 1's score
            lastLetter = player1Country[strlen(player1Country) - 1]; // Next letter for Player 2
        }

        // Player 2 input
        printf("Remaining time: %d seconds\n", timeLimit);
        printf("%s, enter a place starting with '%c': ", player2Name, player1Invalid ? firstLetter : lastLetter);
        fflush(stdout); // Ensure the prompt is printed before the timer starts

        hThread = CreateThread(NULL, 0, show_timer, &timeLimit, 0, NULL); // Start the timer for Player 2

        // Wait for input
        fgets(player2Country, sizeof(player2Country), stdin);
        TerminateThread(hThread, 0); // Stop the timer when input is received
        player2Country[strcspn(player2Country, "\n")] = 0; // Remove newline character
        toLowerCase(player2Country);

        // Clear the timer line
        printf("\r                                          \r");

        int player2Invalid = 0; // Track if Player 2 has an invalid input

        if (player2Country[0] != (player1Invalid ? firstLetter : lastLetter)) {
            printf("Place does not start with %c.\n", player1Invalid ? firstLetter : lastLetter);
            player2Invalid = 1; // Mark as invalid
        }

        // Check if player2Country is valid
        if (!isPlaceValid(player2Country, places, placeCount)) {
            printf("Invalid place.\n");
            player2Invalid = 1; // Mark as invalid
        }

        // Check if player2Country is already used
        if (isAlreadyUsed(player2Country, usedCountries, usedCount)) {
            printf("Place already used.\n");
            player2Invalid = 1; // Mark as invalid
        }

        // Player 2's turn result
        if (player2Invalid) {
            printf("Invalid input by %s.\n", player2Name);
        } else {
            // Add to used list
            strcpy(usedCountries[usedCount++], player2Country);
            player2Score++; // Increment Player 2's score
            firstLetter = player2Country[strlen(player2Country) - 1]; // Next letter for Player 1
        }

        // Display scores after each round
        printf("\nScores after round %d:\n", questionCount);
        printf("%s Score: %d\n", player1Name, player1Score);
        printf("%s Score: %d\n", player2Name, player2Score);

        // End game condition after both players have attempted
        if (player1Invalid || player2Invalid) {
            // Update and display leaderboard
            updateLeaderboard(player1Name, player2Name, player1Score, player2Score);
            break; // End the game
        }
    }

    return 0;
}
