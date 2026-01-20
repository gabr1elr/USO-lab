#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern char **environ;

char *get_var_name(const char *env_var) {
    const char *equal_sign = strchr(env_var, '=');
    if (equal_sign == NULL) {
        return NULL;
    }
    size_t name_len = equal_sign - env_var;
    char *name = (char *)malloc(name_len + 1);
    if (name) {
        strncpy(name, env_var, name_len);
        name[name_len] = '\0';
    }
    return name;
}

const char *get_var_value(const char *env_var) {
    const char *equal_sign = strchr(env_var, '=');
    if (equal_sign == NULL) {
        return ""; 
    }
    return equal_sign + 1;
}

void print_environment(const char *title) {
    printf("\n %s \n", title);
    
    // Verificare pentru robustete (cazul env -i)
    if (environ == NULL || environ[0] == NULL) { 
        printf("Lista de variabile de mediu este goala \n");
        return;
    }

    for (int i = 0; environ[i] != NULL; i++) {
        printf("%d: %s\n", i, environ[i]);
    }
    printf("\n");
}

int main() {
    print_environment("MEDIUL INAINTE DE MODIFICARE");

    if (environ == NULL || environ[0] == NULL) {
        print_environment("MEDIUL DUPA MODIFICARE");
        return 0;
    }

    for (int i = 1; environ[i] != NULL; i += 2) {
        int odd_pos = i;          
        int even_pos = i + 1;     
        if (environ[even_pos] == NULL) {
            break; 
        }

        char *name_odd = get_var_name(environ[odd_pos]);
        const char *value_odd = get_var_value(environ[odd_pos]);
        const char *value_even = get_var_value(environ[even_pos]);

        if (name_odd == NULL) {
            continue; 
        }

        size_t new_value_len = strlen(value_odd) + strlen(value_even);
        
        char *new_value = (char *)malloc(new_value_len + 1);

        if (new_value == NULL) {
            fprintf(stderr, "eroare de alocare memorie pentru noua valoare \n");
            free(name_odd);
            continue;
        }
        sprintf(new_value, "%s%s", value_odd, value_even);
        if (setenv(name_odd, new_value, 1) != 0) {
             perror("setenv a dat fail");
        }
        free(name_odd);
        free(new_value);
    }
    
    print_environment("MEDIUL DUPA MODIFICARE");

    return 0;
}
