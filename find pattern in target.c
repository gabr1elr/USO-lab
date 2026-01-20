#include <stdio.h>
#include <stdlib.h>

long get_file_size(FILE *file) {
    long current_pos = ftell(file);
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, current_pos, SEEK_SET);  
    return size;
}

int compare_files_at_pos(FILE *pattern, FILE *target, long pattern_size, long target_pos) {
    long pattern_original_pos = ftell(pattern);
    long target_original_pos = ftell(target);

    fseek(pattern, 0, SEEK_SET);
    fseek(target, target_pos, SEEK_SET);

    int match = 1;
    for (long i = 0; i < pattern_size; i++) {
        int pattern_byte = fgetc(pattern);
        int target_byte = fgetc(target);
        if (pattern_byte != target_byte || pattern_byte == EOF || target_byte == EOF) {
            match = 0;
            break;
        }
    }

    fseek(pattern, pattern_original_pos, SEEK_SET);
    fseek(target, target_original_pos, SEEK_SET);

    return match;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Utilizare: %s <fisier_pattern> <fisier_target>\n", argv[0]);
        return 1;
    }

    FILE *pattern_file = fopen(argv[1], "rb");
    FILE *target_file = fopen(argv[2], "rb");

    if (pattern_file == NULL || target_file == NULL) {
        printf("eroare la deschiderea fisierelor \n");
        if (pattern_file) fclose(pattern_file);
        if (target_file) fclose(target_file);
        return 1;
    }

    long pattern_size = get_file_size(pattern_file);
    long target_size = get_file_size(target_file);

    printf("Dimensiune pattern: %ld\n", pattern_size);
    printf("Dimensiune target: %ld\n", target_size);

    if (pattern_size == 0) {
        printf("Fisierul pattern este gol\n");
        fclose(pattern_file);
        fclose(target_file);
        return 0;
    }

    if (target_size < pattern_size) {
        printf("Fisierul target este mai mic decat pattern-ul\n");
        fclose(pattern_file);
        fclose(target_file);
        return 0;
    }

    int count = 0;

    for (long i = 0; i <= target_size - pattern_size; i++) {
        if (compare_files_at_pos(pattern_file, target_file, pattern_size, i)) {
            count++;
            printf("Aparitie la pozitia: %ld\n", i);
        }
    }

    printf("Numar total de aparitii: %d\n", count);

    fclose(pattern_file);
    fclose(target_file);
    return 0;
}
