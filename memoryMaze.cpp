#include "splashkit.h"

const int SCREEN_WIDTH = 1020;
const int SCREEN_HEIGHT = 682;
const int PLAYER_RADIUS = 25;
const int PLAYER_SPEED = 1;
const int MAP_ROWS = 12;
const int MAP_COLS = 17;
const int TILE_WIDTH = 60;
const int TILE_HEIGHT = 60;

// for the second argument in load_font, add the location of your font file
font myFont = load_font("Custom Font", "/Users/dishasharma/Documents/Coding/SIT102/Tasks/D4/CustomFont.ttf");
font myFont2 = load_font("Custom Font2", "/Users/dishasharma/Documents/Coding/SIT102/Tasks/D4/Poppins-SemiBold.otf");

// PLAYER
struct player_data
{
    int row;
    int col;
    double x;
    double y;
    int score;
    int timer;
    int level;
    bool hint_active;
};

enum tile_kind
{
    GROUND_TILE,
    WALL_TILE_VISIBLE,
    WALL_TILE_INVISIBLE,
    TOP_TILE
};

struct tile_data
{
    tile_kind kind;
};

struct map_data
{
    tile_data tiles[MAP_ROWS][MAP_COLS];
    int maze_timer;
};

enum game_states
{
    MENU,
    START,
    INSTRUCTIONS,
    WIN,
    EXIT
};

struct game_data
{
    game_states current_state;
    bool quit;
};

void reset_hint(player_data &player)
{
    player.hint_active = false;
    player.timer = 0;
}

player_data initialize_player(map_data &map, player_data &player)
{
    reset_hint(player);
    player.row = MAP_ROWS - 3;
    player.col = 0;

    player.x = player.col * TILE_WIDTH + TILE_WIDTH / 2;
    player.y = player.row * TILE_HEIGHT + TILE_HEIGHT / 2;

    player.score = 0;

    player.level = 1;
    map.maze_timer = 140;

    return player;
}

player_data initialize_player_level_two(map_data &map, player_data &player)
{
    reset_hint(player);
    player.row = MAP_ROWS - 3;
    player.col = 0;

    player.x = player.col * TILE_WIDTH + TILE_WIDTH / 2;
    player.y = player.row * TILE_HEIGHT + TILE_HEIGHT / 2;

    player.score = 1;

    player.level = 2;
    map.maze_timer = 140;

    return player;
}

player_data initialize_player_level_three(map_data &map, player_data &player)
{
    reset_hint(player);
    player.row = MAP_ROWS - 3;
    player.col = 0;

    player.x = player.col * TILE_WIDTH + TILE_WIDTH / 2;
    player.y = player.row * TILE_HEIGHT + TILE_HEIGHT / 2;

    player.score = 2;

    player.level = 3;
    map.maze_timer = 140;

    return player;
}

void draw_player(const player_data &player)
{
    fill_circle(color_pale_violet_red(), player.x, player.y, PLAYER_RADIUS);
}

void move_player(player_data &player, const map_data &map)
{

    if (key_down(RIGHT_KEY) && player.col + 1 < MAP_COLS && map.tiles[player.row][player.col + 1].kind == GROUND_TILE)
    {
        player.col += 1;
        player.x = player.col * TILE_WIDTH + TILE_WIDTH / 2;
    }

    if (key_down(LEFT_KEY) && player.col > 0 && map.tiles[player.row][player.col - 1].kind == GROUND_TILE)
    {
        player.col -= 1;
        player.x = player.col * TILE_WIDTH + TILE_WIDTH / 2;
    }

    if (key_down(UP_KEY) && player.row - 1 >= 0 && map.tiles[player.row - 1][player.col].kind == GROUND_TILE)
    {
        player.row -= 1;
        player.y = player.row * TILE_HEIGHT + TILE_HEIGHT / 2;
    }

    if (key_down(DOWN_KEY) && player.y + PLAYER_RADIUS < SCREEN_HEIGHT && map.tiles[player.row + 1][player.col].kind == GROUND_TILE)
    {
        player.row += 1;
        player.y = player.row * TILE_HEIGHT + TILE_HEIGHT / 2;
    }
}

// START PLAYING

color color_for_tile_kind(tile_kind kind)
{
    switch (kind)
    {
    case GROUND_TILE:
        return color_dark_slate_blue();
    case WALL_TILE_VISIBLE:
        return color_dodger_blue();
    case WALL_TILE_INVISIBLE:
        return color_dark_slate_blue();
    case TOP_TILE:
        return color_indigo();
    default:
        return color_white();
    }
}

void set_tile(map_data &map, int row, int col, tile_kind kind)
{
    map.tiles[row][col].kind = kind;
}

void draw_tile_hint(map_data &map, player_data &player)
{
    for (int y = 0; y < MAP_ROWS; y++)
    {
        for (int x = 0; x < MAP_COLS; x++)
        {
            if (map.tiles[y][x].kind == WALL_TILE_VISIBLE && player.hint_active && map.maze_timer == 0)
            {
                fill_rectangle(color_gray(), x * TILE_WIDTH, y * TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT);
            }
        }
    }
}

void toggle_hint(player_data &player, map_data &map)
{
    if (key_typed(H_KEY) && !player.hint_active && map.maze_timer == 0)
    {
        player.hint_active = true;
        player.timer = 60;
    }
    draw_tile_hint(map, player);

    if (player.hint_active && player.timer > 0)
    {
        player.timer--;
    }
    else if (player.timer == 0 && player.hint_active)
    {
        player.hint_active = false;
    }
}

void draw_tile(const map_data &map, player_data &player)
{
    for (int y = 0; y < MAP_ROWS; y++)
    {
        for (int x = 0; x < MAP_COLS; x++)
        {
            color tile_color;

            if (player.hint_active && map.tiles[y][x].kind == WALL_TILE_VISIBLE)
            {
                tile_color = color_gray();
            }
            else
            {
                tile_color = color_for_tile_kind(map.tiles[y][x].kind);
            }
            fill_rectangle(tile_color, x * TILE_WIDTH, y * TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT);
        }
    }
}

void draw_tile_vanished(const map_data &map)
{
    for (int y = 0; y < MAP_ROWS; y++)
    {
        for (int x = 0; x < MAP_COLS; x++)
        {
            if (map.tiles[y][x].kind == WALL_TILE_INVISIBLE && map.tiles[y][x].kind != TOP_TILE)
            {
                fill_rectangle(color_dark_slate_blue(), x * TILE_WIDTH, y * TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT);
            }
        }
    }
}

game_data draw_level_passed(map_data &map, player_data &player, game_data &game)
{
    clear_screen(color_dark_slate_blue());

    draw_text("Level Passed!!", COLOR_WHITE, myFont, 100, SCREEN_WIDTH / 2 - 265, SCREEN_HEIGHT / 2 - 100);

    int wait_time = 240; // 4 seconds at 60 frames per second
    while (wait_time > 0)
    {
        if (key_typed(Q_KEY) || key_typed(ESCAPE_KEY))
        {
            game.quit = true;
            break;
        }
        wait_time--;
        refresh_screen(60);
    }
    return game;
}

void level_one_tiles(map_data &map)
{
    for (int y = 0; y < MAP_ROWS; y++)
    {
        for (int x = 0; x < MAP_COLS; x++)
        {
            if ((y == 0 && (x == 6 || x == 7 || x == 8 || x == 9 || x == 10)) ||
                (y == 1 && (x == 8)) ||
                (y == 2 && (x == 0 || x == 1 || x == 2 || x == 3 || x == 4 || x == 5 || x == 8 || x == 12 || x == 13 || x == 14 || x == 15 || x == 16)) ||
                (y == 3 && (x == 8 || x == 12 || x == 13 || x == 14 || x == 15 || x == 16)) ||
                (y == 4 && (x == 5 || x == 8)) ||
                (y == 5 && (x == 5 || x == 8)) ||
                (y == 6 && (x == 5 || x == 8 || x == 9 || x == 10 || x == 11 || x == 12)) ||
                (y == 7 && (x == 0 || x == 1 || x == 2 || x == 3 || x == 5 || x == 9 || x == 10 || x == 11 || x == 12 || x == 14 || x == 15 || x == 16)) ||
                (y == 8 && (x == 5 || x == 14 || x == 15 || x == 16)) ||
                (y == 9 && (x == 5 || x == 14 || x == 15 || x == 16)))
            {
                set_tile(map, y, x, WALL_TILE_VISIBLE);
            }
            else
            {
                set_tile(map, y, x, GROUND_TILE);
            }
        }
    }
}

void level_two_tiles(map_data &map)
{
    for (int y = 0; y < MAP_ROWS; y++)
    {
        for (int x = 0; x < MAP_COLS; x++)
        {
            if ((y == 0 && (x == 1 || x == 2 || x == 3 || x == 10 || x == 11 || x == 12)) ||
                (y == 1 && (x == 1 || x == 2 || x == 3 || x == 7 || x == 8 || x == 10 || x == 11 || x == 12 || x == 14)) ||
                (y == 2 && (x == 5 || x == 7 || x == 8 || x == 10 || x == 11 || x == 12 || x == 14)) ||
                (y == 3 && (x == 0 || x == 2 || x == 3 || x == 4 || x == 5 || x == 6 || x == 7 || x == 14)) ||
                (y == 4 && (x == 0 || x == 2 || x == 3 || x == 4 || x == 5 || x == 7 || x == 8 || x == 9 || x == 10 || x == 11 || x == 14)) ||
                (y == 5 && (x == 7 || x == 8 || x == 14)) ||
                (y == 6 && (x == 3 || x == 7 || x == 8 || x == 9 || x == 11 || x == 12)) ||
                (y == 7 && (x == 0 || x == 1 || x == 2 || x == 3 || x == 4 || x == 5 || x == 7 || x == 8 || x == 11 || x == 12 || x == 14 || x == 15)) ||
                (y == 8 && (x == 7 || x == 8 || x == 14 || x == 15)) ||
                (y == 9 && (x == 5 || x == 6 || x == 7 || x == 8 || x == 9 || x == 10 || x == 12 || x == 14 || x == 15)))
            {
                set_tile(map, y, x, WALL_TILE_VISIBLE);
            }
            else
            {
                set_tile(map, y, x, GROUND_TILE);
            }
        }
    }
}

void level_three_tiles(map_data &map)
{
    for (int y = 0; y < MAP_ROWS; y++)
    {
        for (int x = 0; x < MAP_COLS; x++)
        {
            if ((y == 0 && (x == 0 || x == 4 || x == 9 || x == 13)) ||
                (y == 1 && (x == 2 || x == 6 || x == 7 || x == 9 || x == 11 || x == 13 || x == 15)) ||
                (y == 2 && (x == 2 || x == 6 || x == 9 || x == 12 || x == 13)) ||
                (y == 3 && (x == 0 || x == 3 || x == 6 || x == 8 || x == 9 || x == 10 || x == 13 || x == 15)) ||
                (y == 4 && (x == 1 || x == 4 || x == 6 || x == 9 || x == 12)) ||
                (y == 5 && (x == 2 || x == 5 || x == 6 || x == 7 || x == 12 || x == 14)) ||
                (y == 6 && (x == 3 || x == 6 || x == 9 || x == 11 || x == 15)) ||
                (y == 7 && (x == 1 || x == 4 || x == 6 || x == 7 || x == 9 || x == 12 || x == 16)) ||
                (y == 8 && (x == 2 || x == 6 || x == 8 || x == 9 || x == 10 || x == 13)) ||
                (y == 9 && (x == 3 || x == 6 || x == 9 || x == 10 || x == 16)))
            {
                set_tile(map, y, x, WALL_TILE_VISIBLE);
            }
            else
            {
                set_tile(map, y, x, GROUND_TILE);
            }
        }
    }
}

void draw_level_elements(map_data &map, player_data &player)
{
    fill_rectangle(color_indigo(), 0, 600, SCREEN_WIDTH, 2 * TILE_HEIGHT);

    draw_text("LEVEL: " + std::to_string(player.level), COLOR_WHITE, myFont2, 30, 10, SCREEN_HEIGHT - 70);

    rectangle hint_button = {SCREEN_WIDTH - 120, SCREEN_HEIGHT - 70, 100, 40};
    draw_rectangle(color_indigo(), hint_button);
    draw_text("HINT", COLOR_WHITE, myFont2, 30, hint_button.x, hint_button.y);
    fill_circle(color_forest_green(), SCREEN_WIDTH - TILE_HEIGHT / 2, TILE_HEIGHT / 2, PLAYER_RADIUS);
}

void draw_playing(map_data &map, player_data &player)
{
    fill_rectangle(color_indigo(), 0, 600, SCREEN_WIDTH, 2 * TILE_HEIGHT);

    for (int x = 0; x < MAP_COLS; x++)
    {
        set_tile(map, MAP_ROWS - 1, x, TOP_TILE);
        set_tile(map, MAP_ROWS - 2, x, TOP_TILE);
    }
    if (map.maze_timer > 0)
    {
        map.maze_timer--;
        draw_tile(map, player);
    }

    //////////////////////////////////////////
    rectangle hint_button = {SCREEN_WIDTH - 120, SCREEN_HEIGHT - 70, 100, 40};
    draw_rectangle(color_indigo(), hint_button);
    draw_text("HINT", COLOR_WHITE, myFont2, 30, hint_button.x, hint_button.y);

    fill_circle(color_forest_green(), SCREEN_WIDTH - TILE_HEIGHT / 2, TILE_HEIGHT / 2, PLAYER_RADIUS);
    draw_text("LEVEL: " + std::to_string(player.score + 1), COLOR_WHITE, myFont2, 30, 10, SCREEN_HEIGHT - 70);
    //////////////////////////////////////////

    draw_player(player);

    if (mouse_clicked(LEFT_BUTTON))
    {
        point_2d mouse_pos = mouse_position();
        if (point_in_rectangle(mouse_pos, hint_button))
        {
            toggle_hint(player, map);
            draw_tile_hint(map, player);
            player.hint_active = true;
            player.timer = 60;
        }
    }
    if (player.hint_active)
    {
        draw_tile_hint(map, player);
    }
    toggle_hint(player, map);

    if (circles_intersect(player.x, player.y, PLAYER_RADIUS, SCREEN_WIDTH - TILE_HEIGHT / 2, TILE_HEIGHT / 2, PLAYER_RADIUS))
    {
        player.score++;
        player.level++;

        game_data game = draw_level_passed(map, player, game);
        process_events();
        refresh_screen(60);

        int wait_time = 140;
        if (player.level == 2)
        {
            level_two_tiles(map);
            player = initialize_player_level_two(map, player);
            while (wait_time > 0)
            {
                wait_time--;
                process_events();
                draw_tile(map, player);
                draw_player(player);
                draw_level_elements(map, player);
                toggle_hint(player, map);
                refresh_screen(60);
            }
        }
        else if (player.level == 3)
        {
            level_three_tiles(map);
            player = initialize_player_level_three(map, player);
            process_events();
            while (wait_time > 0)
            {
                wait_time--;
                process_events();
                draw_tile(map, player);
                draw_level_elements(map, player);
                draw_player(player);
                toggle_hint(player, map);
                refresh_screen(60);
            }
        }
    }
}

// GAME
game_data draw_menu(game_data &game)
{
    clear_screen(rgb_color(60, 28, 69));

    bitmap background = load_bitmap("background", "background.jpg");
    draw_bitmap(background, 256, 0, option_to_screen());

    for (int i = 0; i < 25; ++i)
    {
        int x = rnd(0, 257);
        int y = rnd(0, 683);
        fill_circle(color_white(), x, y, 1);
    }
    for (int i = 0; i < 25; ++i)
    {
        int x = rnd(766, 1020);
        int y = rnd(0, 683);
        fill_circle(color_white(), x, y, 1);
    }

    rectangle start_button = {180, 520, 700, 50};
    rectangle instructions_button = {180, 570, 700, 50};
    rectangle quit_button = {180, 620, 700, 50};

    draw_rectangle(rgb_color(60, 28, 69), start_button);
    draw_rectangle(rgb_color(60, 28, 69), instructions_button);
    draw_rectangle(rgb_color(60, 28, 69), quit_button);

    // I can use refrence x and y because i have already declred and initialized them in rectangle (from splashkit)
    int center_x = start_button.x + (start_button.width / 2);
    int center_y_start = start_button.y + (start_button.height / 2);
    int center_y_instructions = instructions_button.y + (instructions_button.height / 2);
    int center_y_quit = quit_button.y + (quit_button.height / 2);

    draw_text("Start Game", COLOR_WHITE, myFont2, 40, center_x - (text_width("Start Game", myFont2, 40) / 2), center_y_start - 20);
    draw_text("Instructions", COLOR_WHITE, myFont2, 40, center_x - (text_width("Instructions", myFont2, 40) / 2), center_y_instructions - 20);
    draw_text("Quit Game", COLOR_WHITE, myFont2, 40, center_x - (text_width("Quit Game", myFont2, 40) / 2), center_y_quit - 20);

    if (mouse_clicked(LEFT_BUTTON))
    {
        point_2d mouse_pos = mouse_position();

        if (point_in_rectangle(mouse_pos, start_button))
        {
            game.current_state = START;
        }
        else if (point_in_rectangle(mouse_pos, instructions_button))
        {
            game.current_state = INSTRUCTIONS;
        }
        else if (point_in_rectangle(mouse_pos, quit_button))
        {
            game.current_state = EXIT;
        }
    }
    if (key_typed(Q_KEY) || key_typed(ESCAPE_KEY))
    {
        game.quit = true;
    }

    refresh_screen(60);
    return game;
}

game_data draw_instructions(game_data &game)
{
    clear_screen(rgb_color(60, 28, 69));

    rectangle back_button = {40, 540, 700, 50};
    rectangle continue_button = {40, 600, 700, 50};

    draw_rectangle(rgb_color(60, 28, 69), back_button);
    draw_text("To go back to Menu, press HERE", COLOR_WHITE, myFont, 40, back_button.x + 20, back_button.y - 5);

    draw_rectangle(rgb_color(60, 28, 69), continue_button);
    draw_text("To continue, press HERE", COLOR_WHITE, myFont, 40, continue_button.x + 20, continue_button.y - 5);

    draw_text("Instructions:", COLOR_WHITE, myFont, 50, 40, 40);
    draw_text("->  To move the player use the arrow keys from the keyboard ", COLOR_WHITE, myFont2, 20, 40, 120);
    draw_text("      ^ - to move up, v - to move down, < - to move left, > - to move right ", COLOR_WHITE, myFont2, 20, 40, 160);
    draw_text("->  Once the game starts, you will see the maze for 3 seconds, after that you will see a plain ", COLOR_WHITE, myFont2, 20, 40, 200);
    draw_text("       screen. Try to navigate your way through the maze and remember  what the way was!!", COLOR_WHITE, myFont2, 20, 40, 240);
    draw_text("->  You will be able to see the screen again for a brief time if you press HINT.", COLOR_WHITE, myFont2, 20, 40, 280);
    draw_text("->  If you want to exit anytime, just press the ESCAPE key.", COLOR_WHITE, myFont2, 20, 40, 320);
    draw_text("->  The game has 3 levels, win all of them to win the game! GOOD LUCK!!!", COLOR_WHITE, myFont2, 20, 40, 360);

    if (mouse_clicked(LEFT_BUTTON))
    {
        point_2d mouse_pos = mouse_position();

        if (point_in_rectangle(mouse_pos, back_button))
        {
            game.current_state = MENU;
        }

        if (point_in_rectangle(mouse_pos, continue_button))
        {
            game.current_state = START;
        }
    }

    if (key_typed(Q_KEY) || key_typed(ESCAPE_KEY))
    {
        game.quit = true;
    }
    return game;
}

game_data draw_start(map_data &map, player_data &player, game_data &game)
{
    clear_screen(color_dark_slate_blue());
    game.current_state = START;
    toggle_hint(player, map);
    move_player(player, map);
    draw_playing(map, player);
    draw_level_elements(map, player);

    if (player.level == 4)
    {
        game.current_state = WIN;
    }
    if (key_typed(Q_KEY) || key_typed(ESCAPE_KEY))
    {
        game.quit = true;
    }
    refresh_screen(60);
    return game;
}

game_data draw_win(game_data &game)
{
    clear_screen(rgb_color(60, 28, 69));

    rectangle continue_button = {40, 600, 700, 50};

    draw_rectangle(rgb_color(60, 28, 69), continue_button);
    draw_text("To continue, press HERE", COLOR_WHITE, myFont, 40, continue_button.x + 20, continue_button.y - 5);

    draw_text("Congratulations! You have won the game!", COLOR_WHITE, myFont, 50, 40, 40);
    draw_text("You have successfully completed all the levels!", COLOR_WHITE, myFont2, 40, 40, 180);

    if (mouse_clicked(LEFT_BUTTON))
    {
        point_2d mouse_pos = mouse_position();

        if (point_in_rectangle(mouse_pos, continue_button))
        {
            game.current_state = MENU;
        }
    }

    if (key_typed(Q_KEY) || key_typed(ESCAPE_KEY))
    {
        game.quit = true;
    }

    return game;
}

game_data draw_exit(game_data &game)
{
    clear_screen(rgb_color(60, 28, 69));

    rectangle exit_button = {130, 280, 700, 50};
    rectangle replay_button = {100, 400, 700, 50};

    draw_rectangle(rgb_color(60, 28, 69), 130, 325, 700, 50);
    draw_rectangle(rgb_color(60, 28, 69), 100, 450, 700, 50);

    int center_x = exit_button.x + (exit_button.width / 2);
    int center_y_exit = exit_button.y + (exit_button.height / 2);
    int center_y_replay = replay_button.y + (replay_button.height / 2);

    draw_text("To exit, press HERE", COLOR_WHITE, myFont, 40, center_x - (text_width("To exit, press HERE", myFont, 40) / 2), center_y_exit - 5);
    draw_text("To play again, press HERE", COLOR_WHITE, myFont, 40, center_x - (text_width("To play again, press HERE", myFont, 40) / 2), center_y_replay - 5);

    string thank_you_text = "Thank you for playing!";
    int thank_you_x = (screen_width() - text_width(thank_you_text, myFont2, 50)) / 2;
    draw_text(thank_you_text, COLOR_WHITE, myFont2, 60, thank_you_x - 50, 130);

    if (mouse_clicked(LEFT_BUTTON))
    {
        point_2d mouse_pos = mouse_position();

        if (point_in_rectangle(mouse_pos, exit_button))
        {
            game.quit = true;
        }

        if (point_in_rectangle(mouse_pos, replay_button))
        {
            game.current_state = MENU;
        }
    }

    if (key_typed(ESCAPE_KEY))
    {
        game.quit = true;
    }

    return game;
}

void draw_game(map_data &map, player_data &player, game_data &game)
{
    switch (game.current_state)
    {
    case MENU:
        draw_menu(game);
        break;
    case START:
        draw_start(map, player, game);
        break;
    case INSTRUCTIONS:
        draw_instructions(game);
        break;
    case EXIT:
        draw_exit(game);
        break;
    case WIN:
        draw_win(game);
        break;
    }
}

game_data initialize_game(game_data &game)
{
    game.current_state = MENU;
    game.quit = false;
    return game;
}

game_data new_game()
{
    game_data game;
    return initialize_game(game);
}

int main()
{
    open_window("Memory Maze", SCREEN_WIDTH, SCREEN_HEIGHT);
    myFont = load_font("Custom Font", "CustomFont.ttf");

    game_data game = new_game();
    map_data map;
    player_data player = initialize_player(map, player);

    level_one_tiles(map);

    while (!quit_requested() && !game.quit)
    {
        process_events();
        clear_screen();
        draw_game(map, player, game);
        refresh_screen(60);
    }
    close_window("Memory Maze");
    free_font(myFont);
    free_font(myFont2);

    return 0;
}
