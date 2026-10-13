#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <stdbool.h>

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
                printf("> \033[1m%s\033[0m\n", options[i]);
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
        } else if (input_char == 'd' || arrow_key_char == 'C') {
            // Go down one folder
        } else if (input_char == 'a' || arrow_key_char == 'D') {
            // Go up one folder
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
    // selection_list("How are you doing?", options, sizeof(options)/sizeof(options[0]));

    // Get CWD and its content
    char *cwd = getcwd(NULL, 0);
    printf("Current Working Directory: %s\n", cwd);

    DIR *dir = opendir(cwd);
    if (dir == NULL) {
        printf("Error when reading entries in current directory");
    }
    struct dirent *entry;
    
    while ((entry = readdir(dir)) != NULL) {
        char *entry_name = entry->d_name;
        bool isFolder = (entry->d_type == DT_DIR ? true : false);
        if (strcmp(entry_name, ".") == 0 || strcmp(entry_name, "..") == 0) {
            continue;
        }

        printf("%s%c\n", entry_name, isFolder ? '/' : ' ');
    }

    closedir(dir);
    free(cwd);

    return 0;
}