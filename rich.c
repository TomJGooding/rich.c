#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum {
    DEFAULT,
    TRUECOLOR,
} ColorType;

typedef struct {
    int red;
    int green;
    int blue;
} ColorTriplet;

typedef struct {
    ColorTriplet triplet;
    ColorType type;
} Color;

typedef struct {
    Color color;
    Color bgcolor;
} Style;

typedef struct {
    char *text;
    Style style;
} Text;

Color color_from_rgb(int red, int green, int blue) {
    Color color = {.triplet = {red, green, blue}, .type = TRUECOLOR};
    return color;
}

char *color_get_ansi_code(Color color, bool foreground) {
    char *ansi_code = NULL;
    asprintf(
        &ansi_code,
        "%d;2;%d;%d;%d",
        foreground ? 38 : 48,
        color.triplet.red,
        color.triplet.green,
        color.triplet.blue
    );
    return ansi_code;
}

char *style_make_ansi_codes(Style style) {
    char *ansi_codes = NULL;

    char *bgcolor_ansi_code = color_get_ansi_code(style.bgcolor, false);
    char *color_ansi_code = color_get_ansi_code(style.color, true);

    asprintf(&ansi_codes, "%s;%s", bgcolor_ansi_code, color_ansi_code);

    free(color_ansi_code);
    free(bgcolor_ansi_code);

    return ansi_codes;
}

void rich_print(Text text) {
    char *ansi_codes = style_make_ansi_codes(text.style);
    printf("\x1b[%sm%s\x1b[0m", ansi_codes, text.text);
    free(ansi_codes);
}

int main() {
    Style style = {
        .color = color_from_rgb(255, 0, 0),
        .bgcolor = color_from_rgb(255, 255, 0)
    };
    Text text = {"Hello, World!\n", style};

    rich_print(text);

    return 0;
}
