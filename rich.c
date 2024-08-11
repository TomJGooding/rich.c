#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    ColorType type;
    ColorTriplet triplet;
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
    Color color = {.type = TRUECOLOR, .triplet = {red, green, blue}};
    return color;
}

Color color_from_rgb_hex(char *hex_string) {
    if (strlen(hex_string) != 6) {
        fprintf(
            stderr,
            "Error: Invalid hex color code '%s': Must be six characters.\n",
            hex_string
        );
        exit(EXIT_FAILURE);
    }

    Color color = {.type = TRUECOLOR};
    char slice[3] = {0};
    char *endptr;

    for (int i = 0; i < 6; i += 2) {
        strncpy(slice, hex_string + i, 2);
        long value = strtol(slice, &endptr, 16);

        if (*endptr) {
            fprintf(
                stderr,
                "Error: Invalid hex color code '%s': Invalid character '%s'.\n",
                hex_string,
                endptr
            );
            exit(EXIT_FAILURE);
        }

        if (i == 0) {
            color.triplet.red = (int)value;
        } else if (i == 2) {
            color.triplet.green = (int)value;
        } else if (i == 4) {
            color.triplet.blue = (int)value;
        }
    }

    return color;
}

char *color_get_ansi_code(Color color, bool foreground) {
    char *ansi_code = NULL;
    switch (color.type) {
        case DEFAULT:
            asprintf(&ansi_code, "%d", foreground ? 39 : 49);
            break;
        case TRUECOLOR:
            asprintf(
                &ansi_code,
                "%d;2;%d;%d;%d",
                foreground ? 38 : 48,
                color.triplet.red,
                color.triplet.green,
                color.triplet.blue
            );
            break;
    }

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
        .bgcolor = color_from_rgb_hex("ffff00")
    };
    Text text = {"Hello, World!\n", style};

    rich_print(text);

    return 0;
}
