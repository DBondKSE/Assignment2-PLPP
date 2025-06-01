#include <iostream>

class TextEditor {
public:
    char **text;
    int row = 0;
    int symbol = 0;

    TextEditor() {
        text = (char**)malloc(memory_allocated);
    }

    ~TextEditor(){
        for (int i = 0; i < row; i++)
            free(*(text + i));
        free(text);
    }

    void newLine();

    void save();

    void load();

    void print();

    void insert(int r, int s);

    void search();

    void _delete(int r, int s, int c);

    void undo();

    void redo();

    void cut(int r, int s, int c);

    void paste(int r, int s);

    void copy(int r, int s, int c);

    void insert_r(int r, int s);
private:
    int memory_allocated = 1;
    char* clipboard;
    static char *readInput();
};

int main() {
    system("clear");
    TextEditor text = TextEditor();
    while (true) {
        printf("> Choose the command ([h] - help):\n");
        char command[32];
        scanf(" %s", &command);
        system("clear");
        int number = -1;
        try {
            number = std::stoi(command);
        } catch (...) {
            number = -1;
        }
        switch (number) {
            case 0: {
                printf("-----PROGRAM END-----");
                return 0;
            }
            case 1: {
                printf("Enter text to append: ");
                text.insert(text.row, text.symbol);
                break;
            }
            case 2: {
                text.newLine();
                break;
            }
            case 3: {
                printf("Enter the file name for saving: ");
                text.save();
                break;
            }
            case 4: {
                printf("Enter the file name for loading: ");
                text.load();
                break;
            }
            case 5: {
                text.print();
                break;
            }
            case 6: {
                printf("Choose row and index to insert [%%d %%d]: ");
                int r = text.row;
                int s = text.symbol;
                if (scanf(" %d %d", &r, &s) == 2) {
                    printf("Enter text to insert: ");
                    text.insert(r, s);
                } else printf("Invalid input, try again\n");
                break;
            }
            case 7: {
                printf("Enter text to search: ");
                text.search();
                break;
            }
            case 8: {
                printf("Choose row, index, and number of symbols to delete [%%d %%d %%d]: ");
                int r = text.row;
                int s = text.symbol;
                int c = 0;
                if (scanf(" %d", &r) && scanf(" %d", &s) && scanf(" %d", &c)) {
                    text._delete(r, s, c);
                } else printf("Invalid input, try again\n");
                break;
            }
            case 9: {
                text.undo();
                break;
            }
            case 10: {
                text.redo();
                break;
            }
            case 11: {
                printf("Choose row, index, and number of symbols to cut [%%d %%d %%d]: ");
                int r = text.row;
                int s = text.symbol;
                int c = 0;
                if (scanf(" %d", &r) && scanf(" %d", &s) && scanf(" %d", &c)) {
                    text.cut(r, s, c);
                } else printf("Invalid input, try again\n");
                break;
            }
            case 12: {
                printf("Choose row and index to paste [%%d %%d]: ");
                int r = text.row;
                int s = text.symbol;
                if (scanf(" %d %d", &r, &s) == 2) {
                    text.paste(r, s);
                } else printf("Invalid input, try again\n");
                break;
            }
            case 13: {
                printf("Choose row, index, and number of symbols to copy [%%d %%d %%d]: ");
                int r = text.row;
                int s = text.symbol;
                int c = 0;
                if (scanf(" %d", &r) && scanf(" %d", &s) && scanf(" %d", &c)) {
                    text.copy(r, s, c);
                } else printf("Invalid input, try again\n");
                break;
            }
            case 14: {
                printf("Choose row and index to insert with replacement [%%d %%d]: ");
                int r = text.row;
                int s = text.symbol;
                if (scanf(" %d %d", &r, &s) == 2) {
                    printf("Enter text to insert: ");
                    text.insert_r(r, s);
                } else printf("Invalid input, try again\n");
                break;
            }
            default: {
                printf("Valid command numbers:\n");
                printf("[0] - exit\n");
                printf("[1] - append\n");
                printf("[2] - new line\n");
                printf("[3] - save\n");
                printf("[4] - load\n");
                printf("[5] - print\n");
                printf("[6] - insert\n");
                printf("[7] - search\n");
                // New commands
                printf("[8] - delete\n");
                printf("[9] - undo\n");
                printf("[10] - redo\n");
                printf("[11] - cut\n");
                printf("[12] - paste\n");
                printf("[13] - copy\n");
                printf("[14] - insert replacement\n");
                break;
            }
        }
    }
}

void TextEditor::newLine() {
    row++;
    symbol = 0;
    text = (char**)realloc(text, memory_allocated += sizeof(char *));
    printf("New line started\n\n");
}

void TextEditor::save() {
    int temp; while ((temp = getchar()) != '\n' && temp != EOF) {}
    FILE *file = fopen(readInput(), "w");
    if (file != nullptr)
    {
        for (int i = 0; i <= row; i++) {
            fputs(*(text + i), file);
            fputc('\n', file);
        }
        fclose(file);
        printf("Text has been saved successfully\n");
    }
}

void TextEditor::load() {
    int row_counter = 0;
    int temp; while ((temp = getchar()) != '\n' && temp != EOF) {}
    FILE *file = fopen(readInput(), "r");
    if (file == nullptr)
    {
        printf("Error opening file");
        return;
    }

    free(text);
    text = nullptr;
    int alloc = sizeof(char*);
    text = (char**)malloc(alloc);

    int size = 10;
    int count = 0;
    char* buf = (char*)malloc(size);
    char ch;
    while ((ch = (char)fgetc(file)) != EOF) {
        if (count >= size)
            buf = (char*)realloc(buf, size*=2);
        if (ch == '\n') {
            *(buf + count) = '\0';
            row = row_counter;
            symbol = count;
            *(text + row_counter++) = buf;
            buf = (char*)malloc(size = 10);
            count = 0;
            text = (char**)realloc(text, alloc+=sizeof(char*));
        } else {
            *(buf + count++) = ch;
        }
    }
    fclose(file);
    printf("Text has been loaded successfully\n");
}

void TextEditor::insert(int r, int s) {
    int temp;
    while ((temp = getchar()) != '\n' && temp != EOF) {}

    char *input = readInput();

    int length = 0; while (*(input + length) != '\0') length++;

    if (r == row && s == symbol && symbol == 0) *(text + r) = (char*)malloc(length);
    else *(text + r) = (char*)realloc(*(text + r), memory_allocated += length);

    if (r == row && s == symbol)
        for (int i = 0; i < length; i++)
            *(*(text + r) + s + i) = *(input + i);
    else{
        for (int i = strlen(*(text + r)); i >= s; i--)
            *(*(text + r) + length + i) = *(*(text + r) + i);
        for (int i = 0; i < length; i++)
            *(*(text + r) + s + i) = *(input + i);
    }

    symbol += length;
    free(input);
    printf("\n");
}

void TextEditor::print() {
    for (int i = 0; i <= row; i++)
        printf("%s\n\n", *(text + i));
}

void TextEditor::search() {
    int temp; while ((temp = getchar()) != '\n' && temp != EOF) {}
    char* prompt = readInput();
    if (strlen(prompt) == 0)
        return;
    int length = 0; while (*(prompt + length) != '\0') length++;
    for (int i = 0; i <= row; i++) {
        char* whole = *(text + i);
        char* found;

        while ((found = strstr(whole, prompt)) != nullptr) {
            while (whole < found) putchar(*whole++);
            printf("[>%.*s<]", length, found);
            whole = found + length;
        }
        printf("%s\n", whole);
    }
}

char *TextEditor::readInput() {
    int size = 10;
    char *buffer = (char*)malloc(size);
    if (buffer == nullptr)
        return nullptr;

    int index = 0;
    int current;

    while ((current = getchar()) != '\n' && current != EOF) {
        if (index + 1 > size) {
            size *= 2;
            char *temp = (char*)realloc(buffer, size);
            if (temp == nullptr) {
                free(buffer);
                return nullptr;
            }
            buffer = temp;
        }
        *(buffer + index++) = (char) current;
    }
    *(buffer + index) = '\0';
    return buffer;
}

void TextEditor::_delete(int r, int s, int c) {
    int i = 0;
    while (*(*(text + r) + s + i) != '\0') {
        *(*(text + r) + s + i) = *(*(text + r) + s + i + c);
        i++;
    }
}

void TextEditor::undo() {

}

void TextEditor::redo() {

}

void TextEditor::cut(int r, int s, int c) {
    clipboard = (char*)malloc(c);
    for (int i = 0; i < c; i++) {
        if (*(*(text + r) + s + i) == '\0')
            break;
        *(clipboard + i) =  *(*(text + r) + s + i);
    }
    _delete(r,s,c);
}

void TextEditor::paste(int r, int s) {
    if (clipboard == nullptr)
        return;
    int length = 0; while (*(clipboard + length) != '\0') length++;

    if (r == row && s == symbol && symbol == 0) *(text + r) = (char*)malloc(length);
    else *(text + r) = (char*)realloc(*(text + r), memory_allocated += length);

    if (r == row && s == symbol)
        for (int i = 0; i < length; i++)
            *(*(text + r) + s + i) = *(clipboard + i);
    else{
        for (int i = strlen(*(text + r)); i >= s; i--)
            *(*(text + r) + length + i) = *(*(text + r) + i);
        for (int i = 0; i < length; i++)
            *(*(text + r) + s + i) = *(clipboard + i);
    }

    symbol += length;
    printf("\n");
}

void TextEditor::copy(int r, int s, int c) {
    clipboard = (char*)malloc(c);
    for (int i = 0; i < c; i++) {
        if (*(*(text + r) + s + i) == '\0')
            break;
        *(clipboard + i) =  *(*(text + r) + s + i);
    }
}

void TextEditor::insert_r(int r, int s) {

}

