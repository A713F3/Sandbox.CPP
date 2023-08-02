#include <SDL2/SDL.h>

enum cell_state {EMPTY, SAND, WATER};

struct Vec2 {int x, y; };
struct Vec3 {int x, y, z; };

class Board
{
    struct cell
    {
        cell_state state;
        Vec2 velo;
        Vec3 color;
        bool updated_this_frame;
    };

    int _width, _height, _cell_size;
    cell** _board;

public:
    Board(int w, int h, int cell_size);

    int cell_size() {return _cell_size;}
    int width() {return _width;}
    int height() {return _height;}

    inline cell sand_cell();
    inline cell water_cell();
    inline cell empty_cell();

    void update_sand(int x, int y);
    void update_water(int x, int y);

    void set_cell(int x, int y, cell_state state);
    void switch_cells(int x1, int y1, int x2, int y2);

    void render(SDL_Renderer* renderer);
    void update();
};

Board::cell Board::sand_cell()  {return {.state = SAND,  .velo = {0,0}, .color = {194, 178, 128}};};
Board::cell Board::water_cell() {return {.state = WATER, .velo = {1,0}, .color = {116, 204, 244}};};
Board::cell Board::empty_cell() {return {.state = EMPTY, .velo = {0,0}, .color = { 51,  51,  51}};};

Board::Board(int w, int h, int cell_size)
{
    _width = w;
    _height = h;
    _cell_size = cell_size;

    _board = new cell*[_height];

    for (int y = 0; y < _height; y++)
    {
        _board[y] = new cell[_width];
        
        for (int x = 0; x < _width; x++)
        {
            _board[y][x] = empty_cell();
        } 
    }

}

void Board::render(SDL_Renderer* renderer)
{   
    static SDL_Rect cell;
    static int x, y;
    static Vec3 c;

    for (y = 0; y < _height; y++)
    {
        for (x = 0; x < _width; x++)
        {
            // Draw cell.
            c = _board[y][x].color;

            SDL_SetRenderDrawColor(renderer, c.x, c.y, c.z, SDL_ALPHA_OPAQUE);

            cell = {.x = x*_cell_size, .y = y*_cell_size, .w = _cell_size, .h = _cell_size};
            SDL_RenderFillRect(renderer, &cell);
        }
    }
}

void Board::update()
{   
    static int x, y;

    for (y = _height - 1; y >= 0; y--)
    {
        for (x = _width - 1; x >= 0; x--)
        {
            if (!_board[y][x].updated_this_frame)
            {
                if (_board[y][x].state == SAND)  update_sand(x, y);
                if (_board[y][x].state == WATER) update_water(x, y);
            }
        }
    }

    for (y = _height - 1; y >= 0; y--)
    {
        for (x = _width - 1; x >= 0; x--)
        {
            _board[y][x].updated_this_frame = false;
        }
    }
}

void Board::update_sand(int x, int y)
{   
    _board[y][x].updated_this_frame = true;
    // Fall bottom.
    if (y < _height-1 && _board[y+1][x].state != SAND)
    {
        switch_cells(x, y, x, y+1);
        return;
    }

    // Fall bottom right.
    if (y < _height-1 && x < _width-1 && _board[y+1][x+1].state != SAND)
    {
        switch_cells(x, y, x+1, y+1);
        return;
    }

    // Fall bottom left.
    if (y < _height-1 && x > 0 && _board[y+1][x-1].state != SAND)
    {
        switch_cells(x, y, x-1, y+1);
        return;
    }
}

void Board::update_water(int x, int y)
{   
    _board[y][x].updated_this_frame = true;

    bool turn = false;

    // Fall to bottom.
    if (y < _height-1 && _board[y+1][x].state == EMPTY)
    {
        switch_cells(x, y, x, y+1);
        return;
    }

    // Fall to bottom right.
    if (y < _height-1 && x < _width-1 && _board[y+1][x+1].state == EMPTY)
    {
        switch_cells(x, y, x+1, y+1);
        return;
    }

    // Fall to bottom left.
    if (y < _height-1 && x > 0 && _board[y+1][x-1].state == EMPTY)
    {
        switch_cells(x, y, x-1, y+1);
        return;
    }

    // Flow to right.
    if (_board[y][x].velo.x > 0 && x < _width-1 && _board[y][x+1].state == EMPTY)
    {
        switch_cells(x, y, x+1, y);
    }
    // Flow to left.
    else if (_board[y][x].velo.x < 0 && x > 0 && _board[y][x-1].state == EMPTY)
    {
        switch_cells(x, y, x-1, y);
    }
    else 
    {
        _board[y][x].velo.x *= -1;
    }
}

void Board::switch_cells(int x1, int y1, int x2, int y2)
{
    cell temp = _board[y1][x1];

    _board[y1][x1] = _board[y2][x2];
    _board[y2][x2] = temp;
}

void Board::set_cell(int x, int y, cell_state state)
{
    if (_board[y][x].state == EMPTY)
    {
        switch (state)
        {
        case SAND:
            _board[y][x] = sand_cell();
            break;

        case WATER:
            _board[y][x] = water_cell();
            break;
        
        default:
            break;
        }
    }
}

