#include <stdio.h>
#include <termios.h>
#include <unistd.h>

// Source: https://viewsourcecode.org/snaptoken/kilo/02.enteringRawMode.html
struct termios orig_termios;

void disableRawMode() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}
void enableRawMode() {
  tcgetattr(STDIN_FILENO, &orig_termios);
  struct termios raw = orig_termios;
  raw.c_lflag &= ~(ECHO | ICANON);
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}
//

int selection_list(const char question[], const char *options[], int option_count) {
    int selected_option_index = 0;

    enableRawMode();

    while (1) {
        printf("%s\n", question);

        for (int i = 0; i < option_count; i++) {
            if (i == selected_option_index) {
                printf("> %s\n", options[i]);
            } else {
                printf("- %s\n", options[i]);
            }
        }
        
        char input_char = getchar();
        char discard_char = '_', arrow_key_char = '_';
        if (input_char == '\033') {
            discard_char = getchar(), arrow_key_char = getchar();
        }
        if ((input_char == 'w' || arrow_key_char == 'A') && selected_option_index - 1 >= 0) {
            selected_option_index -= 1;
        } else if ((input_char == 's' || arrow_key_char == 'B') && selected_option_index + 1 < option_count) {
            selected_option_index += 1;
        }

        printf("\033[%dA\r\033[K", option_count + 1);
    }

    disableRawMode();

    return 0;
}

int main() {
    const char *options[] = {
        "file1",
        "file2",
        "file3"
    };
    selection_list("How are you doing?", options, sizeof(options)/sizeof(options[0]));

    return 0;
}