#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int red;
    int green;
    int blue;
} Color;

typedef struct {
    Color color;
    Color bgcolor;
} Style;

typedef struct {
    char *text;
    Style style;
} Text;

char *color_get_ansi_code(Color color, bool foreground) {
    char *ansi_code = NULL;
    asprintf(
        &ansi_code,
        "%d;2;%d;%d;%d",
        foreground ? 38 : 48,
        color.red,
        color.green,
        color.blue
    );
    return ansi_code;
}

void rich_print(Text text) {
    char *color_ansi_code = color_get_ansi_code(text.style.color, true);
    char *bgcolor_ansi_code = color_get_ansi_code(text.style.bgcolor, false);
    printf(
        "\x1b[%s;%sm%s\x1b[0m", color_ansi_code, bgcolor_ansi_code, text.text
    );
    free(color_ansi_code);
    free(bgcolor_ansi_code);
}

int main() {
    Style style = {.color = {255, 0, 0}, .bgcolor = {255, 255, 0}};
    Text text = {"Hello, World!\n", style};

    rich_print(text);

    return 0;
}
