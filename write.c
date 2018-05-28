#include "common.h"
#include "write.h"
#include "io.h"

// I/O ports
#define FRAMEBUFFER_COMMAND_PORT         0x3D4
#define FRAMEBUFFER_DATA_PORT            0x3D5

// I/O port commands
#define FRAMEBUFFER_HIGH_BYTE_COMMAND    14
#define FRAMEBUFFER_LOW_BYTE_COMMAND     15

const unsigned short framebuffer_width = 80;
const unsigned short framebuffer_height = 25;

unsigned short *framebuffer = (unsigned short*)0x000B8000;

unsigned short cursor_x = 0;
unsigned short cursor_y = 0;

unsigned short get_cursor_position()
{
    return cursor_y * framebuffer_width + cursor_x;
}

uint8_t background_color;
uint8_t foreground_color;

void framebuffer_set_background_color(uint8_t color)
{
    background_color = color;
}

void framebuffer_set_foreground_color(uint8_t color)
{
    foreground_color = color;
}

void framebuffer_move_cursor(unsigned short position);
void framebuffer_write_cell(unsigned int i, char c, uint8_t fg, uint8_t bg);

void framebuffer_init(uint8_t mapped_to_himem)
{
    if (mapped_to_himem)
    {
        framebuffer = (unsigned short*)0x4B8000;
    }

    background_color = COLOR_BLACK;
    foreground_color = COLOR_WHITE;
}

void cls()
{
    uint8_t attributeByte = (0 << 4) | (15 & 0x0F);
    // 0x20 = space
    uint16_t blank = 0x20 | (attributeByte << 8);
    
    // Set all framebuffer values to the 'blank' value
    for (int i = 0; i < framebuffer_width * framebuffer_height; ++i)
    {
        framebuffer[i] = blank;
    }
}

void framebuffer_write_cell(unsigned int i, char c, uint8_t fg, uint8_t bg)
{
    uint8_t  attributeByte = (bg << 4) | (fg & 0x0F);
    uint16_t attribute = attributeByte << 8;
    uint16_t* where;

    if (c == '\n')
    {
        // Move cursor to a new line
        cursor_x = 0;
        ++cursor_y;
    }
    else if (c == '\t')
    {
        // Insert a tab (four spaces)
        cursor_x += 4;
        if (cursor_x > 79)
        {
            cursor_x = 0;
            ++cursor_y;
        }
    }
    else if (c == '\b')
    {
        // Handle backspace
        if (cursor_x != 0)
        {
            --cursor_x;
        }

        where = framebuffer + get_cursor_position();
        *where = 0x20 | attribute;
    }
    else
    {
        where = framebuffer + get_cursor_position();
        *where = c | attribute;
        if (i == 79)
        {
            // Start a new line
            cursor_x = 0;
            ++cursor_y;
        }
        else
        {
            ++cursor_x;
        }
    }
}

void framebuffer_write(const char* text)
{
    uint16_t c = 0;
    while (text[c] != '\0')
    {
        // Write the current character to the appropriate framebuffer cell
        framebuffer_write_cell(c, text[c], foreground_color, background_color);
        ++c;
    }

    framebuffer_move_cursor(get_cursor_position());
}

void framebuffer_write_char(char c)
{
    framebuffer_write_cell(get_cursor_position(), c, foreground_color, background_color);
    framebuffer_move_cursor(get_cursor_position());
}

void framebuffer_move_cursor(unsigned short position)
{
    outb(FRAMEBUFFER_COMMAND_PORT, FRAMEBUFFER_HIGH_BYTE_COMMAND);
    outb(FRAMEBUFFER_DATA_PORT,    ((position >> 8) & 0x00FF));
    outb(FRAMEBUFFER_COMMAND_PORT, FRAMEBUFFER_LOW_BYTE_COMMAND);
    outb(FRAMEBUFFER_DATA_PORT,    position & 0x00FF);
}