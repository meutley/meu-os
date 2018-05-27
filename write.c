#include "common.h"
#include "write.h"

const unsigned short framebuffer_width = 80;
const unsigned short framebuffer_height = 25;

unsigned short *framebuffer = (unsigned short*)0x000B8000;

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

void framebuffer_clear()
{
    uint8_t attributeByte = (0 << 4) | (15 & 0x0F);
    uint16_t blank = 0x20 | (attributeByte << 8);
    
    for (int i = 0; i < framebuffer_width * framebuffer_height; ++i)
    {
        framebuffer[i] = blank;
    }
}

void framebuffer_write_cell(unsigned int i, char c, uint8_t fg, uint8_t bg)
{
    uint8_t  attributeByte = (bg << 4) | (fg & 0x0F);
    uint16_t attribute = attributeByte << 8;

    uint16_t* where = framebuffer + i;
    *where = c | attribute;
}

void framebuffer_write(const char* text)
{
    uint16_t c = 0;
    while (text[c] != '\0')
    {
        framebuffer_write_cell(c, text[c], foreground_color, background_color);
        ++c;
    }
}