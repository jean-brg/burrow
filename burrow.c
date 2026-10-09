#include <stdio.h>

int selection_list(char question[], char *options[], int option_count) {
    printf("%s", question);

    for (int i = 0; i < option_count; i++) {
        printf("%s", options[i]);
    }

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