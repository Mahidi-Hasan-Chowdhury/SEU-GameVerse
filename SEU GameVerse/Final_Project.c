#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h> // For sleep() and nanosleep

#define MAX_WORD_LENGTH 50
#define MAX_TRIES 6

// Define colors and effects
#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define BOLD "\033[1m"

// Tic-Tac-Toe board
char board[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

// Linked list node to store game results
typedef struct GameResult 
{
    char gameType[20];
    char outcome[20];
    struct GameResult* next;
} GameResult;

GameResult* head = NULL;

// Function to add a result to the linked list
void addResult(const char* gameType, const char* outcome) 
{
    GameResult* newResult = (GameResult*)malloc(sizeof(GameResult));
    strcpy(newResult->gameType, gameType);
    strcpy(newResult->outcome, outcome);
    newResult->next = head;
    head = newResult;
}

// Function to print all results stored in the linked list
void printResults() 
{
    GameResult* current = head;
    printf("\n" YELLOW BOLD "Game History:" RESET "\n");
    if (!current) 
    {
        printf(RED "No games played yet.\n" RESET);
    }
    while (current) 
    {
        printf("Game: " CYAN "%s" RESET " | Outcome: " GREEN "%s\n" RESET, current->gameType, current->outcome);
        current = current->next;
    }
}

// Tic-Tac-Toe functions
void printBoard() 
{
    printf(BLUE "|||| Tic_Tac_Toe ||||" RESET "\n\n");
    printf("     |     |     \n");
    printf("  %c  |  %c  |  %c  \n", board[1], board[2], board[3]);
    printf("_____|_____|_____\n");
    printf("     |     |     \n");
    printf("  %c  |  %c  |  %c  \n", board[4], board[5], board[6]);
    printf("_____|_____|_____\n");
    printf("     |     |     \n");
    printf("  %c  |  %c  |  %c  \n", board[7], board[8], board[9]);
    printf("     |     |     \n\n");
}

void animatedPrint(const char* text, int delay) 
{
    struct timespec req = {0};
    req.tv_sec = 0;
    req.tv_nsec = delay * 1000000L; // Control animation speed

    for (int i = 0; text[i] != '\0'; i++) 
    {
        printf("%c", text[i]);
        fflush(stdout);
        nanosleep(&req, (struct timespec *)NULL);
    }
}

int checkWin() 
{
    if (board[1] == board[2] && board[2] == board[3]) return 1;
    if (board[1] == board[4] && board[4] == board[7]) return 1;
    if (board[7] == board[8] && board[8] == board[9]) return 1;
    if (board[3] == board[6] && board[6] == board[9]) return 1;
    if (board[1] == board[5] && board[5] == board[9]) return 1;
    if (board[3] == board[5] && board[5] == board[7]) return 1;
    if (board[2] == board[5] && board[5] == board[8]) return 1;
    if (board[4] == board[5] && board[5] == board[6]) return 1;

    int count = 0;
    for (int i = 1; i <= 9; i++) 
    {
        if (board[i] == 'X' || board[i] == 'O') 
        {
            count++;
        }
    }
    return (count == 9) ? 0 : -1;
}

void playTicTacToe() 
{
    int player = 1, input, status = -1;
    printBoard();

    while (status == -1) 
    {
        player = (player % 2 == 0) ? 2 : 1;
        char mark = (player == 1) ? 'X' : 'O';

        printf(YELLOW "Player %d, enter a number: " RESET, player);
        scanf("%d", &input);

        if (input < 1 || input > 9 || board[input] == 'X' || board[input] == 'O') 
        {
            printf(RED "Invalid move, try again.\n" RESET);
        } 
        else 
        {
            board[input] = mark;
            printBoard();
            status = checkWin();

            if (status == 1) 
            {
                printf(GREEN "Player %d wins!\n" RESET, player);
                addResult("Tic-Tac-Toe", player == 1 ? "Player 1 Wins" : "Player 2 Wins");
                break;
            } 
            else if (status == 0) 
            {
                printf(MAGENTA "It's a draw!\n" RESET);
                addResult("Tic-Tac-Toe", "Draw");
                break;
            }
            player++;
        }
    }
}

// Rock-Paper-Scissors functions
int game(char you, char computer) 
{
    if (you == computer) return -1;
    if ((you == 'r' && computer == 'p') || (you == 'p' && computer == 's') || (you == 's' && computer == 'r')) return 0;
    return 1;
}

void playRockPaperScissors() 
{
    char you, computer;
    int n = rand() % 100;

    if (n < 33) computer = 'r';
    else if (n > 33 && n < 66) computer = 'p';
    else computer = 's';

    animatedPrint("Enter " GREEN "r" RESET " for ROCK, " BLUE "p" RESET " for PAPER, and " RED "s" RESET " for SCISSOR: ", 50);
    getchar();
    scanf("%c", &you);

    int result = game(you, computer);

    if (result == -1) 
    {
        printf(YELLOW "\nGame Draw!\n" RESET);
        addResult("Rock-Paper-Scissors", "Draw");
    } 
    else if (result == 1) 
    {
        printf(GREEN "\nWow! You have won the game!\n" RESET);
        addResult("Rock-Paper-Scissors", "Player Wins");
    } 
    else 
    {
        printf(RED "\nOh! You have lost the game!\n" RESET);
        addResult("Rock-Paper-Scissors", "Computer Wins");
    }
    printf("You chose: %c and Computer chose: %c\n", you, computer);
}

// Hangman functions
void displayWord(const char word[], const bool guessed[]) 
{
    printf("Word: ");
    for (int i = 0; word[i] != '\0'; i++) 
    {
        if (guessed[word[i] - 'a']) 
        {
            printf("%c ", word[i]);
        } 
        else 
        {
            printf("_ ");
        }
    }
    printf("\n");
}

void drawHangman(int tries) 
{
    const char* hangmanParts[] = {" _________", " |       |", " |       O", " |      /|\\", " |      / \\", " |"};
    printf("\n");
    for (int i = 0; i <= tries; i++) 
    {
        printf("%s\n", hangmanParts[i]);
    }
}

bool isWordGuessed(const char* word, const bool guessed[]) 
{
    for (int i = 0; word[i] != '\0'; i++) 
    {
        if (!guessed[word[i] - 'a']) 
        {
            return false;
        }
    }
    return true;
}

void playHangman() 
{
    struct 
    { 
        char word[MAX_WORD_LENGTH]; char hint[MAX_WORD_LENGTH]; 
    } 
    wordList[] = 
    {
        {"python", "Coding language and snake name"},
        {"tiger", "National Animal of Bangladesh"},
        {"pizza", "A popular Italian dish"},
        {"ronaldo", "Greatest Of All Time"},
        {"mango", "A common fruit"},
        {"book", "Something you read"},
        {"snow", "White and falls in winter"},
        {"fish", "Lives in water and has fins"},
        {"moon", "Shines at night in the sky"},
        {"cake", "A sweet treat for celebrations"},
        {"train", "Moves on tracks, often carries people"},
        {"lamp", "Gives light in a room"},
        {"tree", "Gives oxygen to nature"},
        {"shoe", "Worn on your feet"}
    };

    int wordIndex = rand() % 14;
    const char* secretWord = wordList[wordIndex].word;
    const char* hint = wordList[wordIndex].hint;
    int wordLength = strlen(secretWord);
    bool guessedLetters[26] = {false};
    int tries = 0;
    bool won = false;

    printf("Welcome to Hangman! Here is your hint: " CYAN "%s\n" RESET, hint);
    while (tries < MAX_TRIES) 
    {
        drawHangman(tries);
        displayWord(secretWord, guessedLetters);
        printf("Guesses left: %d\n", MAX_TRIES - tries);
        printf(YELLOW "Enter a letter: " RESET);

        char guess;
        scanf(" %c", &guess);
        guess = tolower(guess);

        if (guessedLetters[guess - 'a']) 
        {
            printf(RED "You've already guessed that letter. Try again.\n" RESET);
            continue;
        }

        guessedLetters[guess - 'a'] = true;
        bool found = false;

        for (int i = 0; i < wordLength; i++) 
        {
            if (secretWord[i] == guess) 
            {
                found = true;
            }
        }

        if (found) 
        {
            printf(GREEN "Good guess!\n" RESET);
            if (isWordGuessed(secretWord, guessedLetters)) 
            {
                won = true;
                break;
            }
        } 
        else 
        {
            printf(RED "Sorry, the letter '%c' is not in the word.\n" RESET, guess);
            tries++;
        }
    }

    if (won) 
    {
        printf(GREEN "Congratulations! You guessed the word: %s\n" RESET, secretWord);
        addResult("Hangman", "Player Wins");
    } 
    else 
    {
        printf(RED "You lost! The word was: %s\n" RESET, secretWord);
        addResult("Hangman", "Computer Wins");
    }
}


// Main function with linked list integration
int main() 
{
    srand(time(NULL));
    int choice;

    while (1) 
    {
        printf(YELLOW "Choose a game to play:\n" RESET);
        printf(CYAN "1. Tic-Tac-Toe\n" RESET);
        printf(MAGENTA "2. Rock-Paper-Scissors\n" RESET);
        printf(GREEN "3. Hangman\n" RESET);
        printf(BLUE "4. View Game History\n" RESET);
        printf(RED "5. Exit\n" RESET);
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) 
        {
            case 1:
                for (int i = 1; i <= 9; i++) 
                {
                    board[i] = '0' + i;
                }
                playTicTacToe();
                break;
            case 2:
                playRockPaperScissors();
                break;
            case 3:
                playHangman();
                break;
            case 4:
                printResults();
                break;
            case 5:
                printf(RED "Exiting program. Goodbye!\n" RESET);
                return 0;
            default:
                printf(RED "Invalid choice! Please try again.\n" RESET);
                break;
        }
    }
    return 0;
}
