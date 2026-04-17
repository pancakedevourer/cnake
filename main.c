#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <raylib.h>

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 400
#define WINDOW_TITLE "cnake"
#define FPS 60
#define LINES 16
#define COLUMNS 16
#define CELL_WIDTH (GetScreenWidth() / LINES)
#define CELL_HEIGHT (GetScreenHeight() / COLUMNS)
#define CELL_RECTANGLE columns * CELL_WIDTH, lines * CELL_HEIGHT, CELL_WIDTH, CELL_HEIGHT

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
    SetTargetFPS(FPS);
    // if running
    int running = 1;
    // grid
    int grid[LINES][COLUMNS] = {0};
    // snake
    int snake_lines[LINES * COLUMNS], snake_columns[LINES * COLUMNS]; 
    int snake_length = 1;
    snake_lines[0] = 0;
    snake_columns[0] = 0;
    // apple
    srand(time(NULL));
    int apple_line = rand() % LINES, apple_column = rand() % COLUMNS;
    grid[apple_line][apple_column] = 2;
    // movement direction
    int direction_line = 0, direction_column = 0;
    // timer
    double current_time = GetTime();
    double movement_time = GetTime();
    int timer = 0;
    char *timer_string = 0;
    asprintf(&timer_string, "time: 0");
    // fps counter
    int fps = GetFPS();
    char *fps_string = 0;
    asprintf(&fps_string, "fps: %d", fps);
    // score
    int score = 0;
    char *score_string = 0;
    asprintf(&score_string, "score: %d", score);
    // main loop
    while(!WindowShouldClose() && running) {
        // movement
        switch(GetKeyPressed()) {
            case KEY_W:
                if(direction_line != 1) {
                    direction_line = -1;
                    direction_column = 0;
                }
                break;
            case KEY_A:
                if(direction_column != 1) {
                    direction_line = 0;
                    direction_column = -1;
                }
                break;
            case KEY_S:
                if(direction_line != -1) {
                    direction_line = 1;
                    direction_column = 0;
                }
               break;
            case KEY_D:
                if(direction_column != -1) {
                    direction_line = 0;
                    direction_column = 1;
                }
               break;
        }
        // movement timer
        if(GetTime() - movement_time >= 0.5) {
            movement_time = GetTime();
            // clear previous position 
            grid[snake_lines[snake_length - 1]][snake_columns[snake_length - 1]] = 0;
            // move body
            for(int index = snake_length - 1; index > 0; index--) {
                snake_lines[index] = snake_lines[index - 1];
                snake_columns[index] = snake_columns[index - 1];
            }
            // move head
            snake_lines[0] += direction_line;
            snake_columns[0] += direction_column;
            for(int index = 1; index < snake_length; index++) {
                if(snake_lines[0] == snake_lines[index] && snake_columns[0] == snake_columns[index]) {
                    running = 0;
                    break;
                }
            }
            // score condition
            if(grid[snake_lines[0]][snake_columns[0]] == 2 ) {
                snake_lines[snake_length] = snake_lines[snake_length - 1];
                snake_columns[snake_length] = snake_columns[snake_length - 1];
                snake_length++;
                score++;
                free(score_string);
                asprintf(&score_string, "score: %d", score);
                apple_line = rand() % LINES;
                apple_column = rand() % COLUMNS;
                grid[apple_line][apple_column] = 2;
            }
            grid[snake_lines[0]][snake_columns[0]] = 1;
        }
        // timer
        if(GetTime() - current_time >= 1) {
            timer += 1;
            current_time = GetTime();
            free(timer_string);
            asprintf(&timer_string, "time: %d", timer);
        }
        // fps counter
        fps = GetFPS();
        free(fps_string);
        asprintf(&fps_string, "fps: %d", fps);
        // lose condition
        if(snake_lines[0] < 0 || snake_lines[0] >= LINES || snake_columns[0] < 0 || snake_columns[0] >= COLUMNS) {
            break;
        }
        // drawing loop
        BeginDrawing();
            ClearBackground(BLACK);
            for(int lines = 0; lines < LINES; lines++) {
                for(int columns = 0; columns < COLUMNS; columns++) {
                    DrawRectangleLines(CELL_RECTANGLE, WHITE);
                    if(grid[lines][columns] == 1) {
                        DrawRectangle(CELL_RECTANGLE, GREEN);
                    }
                    if(grid[lines][columns] == 2) {
                        DrawRectangle(CELL_RECTANGLE, RED);
                    }
                }
            }
            // fps counter
            DrawText(fps_string, 30, 30, 20, RAYWHITE);
            // timer
            DrawText(timer_string, 30, 60, 20, RAYWHITE);
            // score
            DrawText(score_string, 30, 90, 20, RAYWHITE);
        EndDrawing();
    }
    int highscore = 0;
    FILE *score_file = fopen("score", "r");
    if(score_file) {
        fscanf(score_file, "highscore: %d\ncongratulations, you got a new highscore! thank you very much for playing my game :D", &highscore);
        fclose(score_file);
    }
    if(score > highscore) {
        score_file = fopen("score", "w");
        fprintf(score_file, "highscore: %d\ncongratulations! thank you very much for playing my game :D", score);
        fclose(score_file);
    }
    free(timer_string);
    free(fps_string);
    CloseWindow();
    return 0;
}
