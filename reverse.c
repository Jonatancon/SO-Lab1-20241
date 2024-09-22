#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024

void reverse_lines(FILE *input, FILE *output) {
    char **lines = NULL;
    size_t line_count = 0;
    size_t capacity = 0;
    char buffer[MAX_LINE_LENGTH];

    // Leer las líneas desde el archivo de entrada
    while (fgets(buffer, MAX_LINE_LENGTH, input) != NULL) {
        if (line_count >= capacity) {
            capacity = capacity == 0 ? 10 : capacity * 2;
            char **temp = realloc(lines, capacity * sizeof(char *));
            if (temp == NULL) {
                fprintf(stderr, "malloc failed\n");
                exit(1);
            }
            lines = temp;
        }

        lines[line_count] = malloc(strlen(buffer) + 1);
        if (lines[line_count] != NULL) {
            strcpy(lines[line_count], buffer);
        } else {
            fprintf(stderr, "malloc failed\n");
            exit(1);
        }
        line_count++;
    }

    // Imprimir las líneas en orden inverso
    for (int i = line_count - 1; i >= 0; i--) {
        fprintf(output, "%s", lines[i]);
        free(lines[i]);
    }

    free(lines);
}

int main(int argc, char *argv[]) {
    FILE *input = stdin;
    FILE *output = stdout;

    // Manejar el caso de demasiados argumentos
    if (argc > 3) {
        fprintf(stderr, "usage: reverse <input> <output>\n");
        exit(1);
    }

    // Si hay un archivo de entrada
    if (argc >= 2) {
        input = fopen(argv[1], "r");
        if (input == NULL) {
            fprintf(stderr, "reverse: cannot open file '%s'\n", argv[1]);
            exit(1);
        }
    }

    // Si hay un archivo de salida
    if (argc == 3) {
        if (strcmp(argv[1], argv[2]) == 0) {
            fprintf(stderr, "reverse: input and output file must differ\n");
            exit(1);
        }
        char comando[256];
        snprintf(comando,sizeof(comando),"cmp -s %s %s",argv[1],argv[2]);
        int result = system(comando);
        if(result==0){
            fprintf(stderr, "reverse: input and output file must differ\n");
            exit(1);
        }
        output = fopen(argv[2], "w");
        if (output == NULL) {
            fprintf(stderr, "reverse: cannot open file '%s'\n", argv[2]);
            exit(1);
        }
    }

    // Invertir las líneas
    reverse_lines(input, output);

    // Cerrar los archivos si no son stdin o stdout
    if (input != stdin) {
        fclose(input);
    }
    if (output != stdout) {
        fclose(output);
    }

    return 0;
}