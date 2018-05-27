#include "common.h"

void framebuffer_set_background_color(uint8_t color);
void framebuffer_set_foreground_color(uint8_t color);
void framebuffer_init(uint8_t mapped_to_himem);
void framebuffer_clear();
void framebuffer_write(const char* text);