#include <unistd.h> // For getcwd
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

typedef struct metadata {
    char nombre[30];
    int longitud;
} metadata;

//-------------------------------

void altaDeCampo();

void crearArchivoMetadata();

void mostrarArchivoMetadata();

void definirEstructura();

bool estaDefinida();

bool comprobarExistencia();

//-------------------------------
int main(void) {
    int eleccion;
    char salir;
    char ch;
    do {
        printf("\n------------------------------------------------------------\n");
        printf("1. Crear archivo metadata (Si existe, sobreescribe)\n");
        printf("2. Crear nuevo campo(alta)\n");
        printf("3. Mostrar archivo metadata\n");
        printf("\nIngrese una operacion:  ");

        while (scanf(" %d", &eleccion) != 1 || (eleccion != 1 && eleccion != 2 && eleccion != 3) || getchar() != '\n') {
            while ((ch = getchar()) != '\n' && ch != EOF);
            printf("Ingrese [1,2,3]: ");
        }

        if (eleccion == 1) {
            crearArchivoMetadata();
        } else if (eleccion == 2) {
            if (comprobarExistencia()) {
                if (estaDefinida()) {
                    printf("La estructura ya ha sido definida!!!");
                } else {
                    altaDeCampo();
                }
            } else {
                printf("\nDebe crear el archivo metadata antes de realizar operaciones!");
            }
        } else if (eleccion == 3) {
            if (comprobarExistencia()) {
                mostrarArchivoMetadata();
            } else {
                printf("\nDebe crear el archivo metadata antes de realizar operaciones!");
            }
        }

        printf("\nDesea realizar otra operacion? [y,n]: ");
        while (scanf(" %c", &salir) != 1 || (salir != 'y' && salir != 'Y' && salir != 'n' && salir != 'N')  || getchar() != '\n') {
            while ((ch = getchar()) != '\n' && ch != EOF);
            printf("Ingrese [y,n]: ");
        }
    } while (salir == 'y' || salir == 'Y');

    if (comprobarExistencia() && !estaDefinida()) {
        definirEstructura();
        printf("ESTRUCTURA DE DATOS DEFINIDA!\n");
    }
    return 0;
}

void crearArchivoMetadata() {
    remove("metadata.dat");
    FILE *fptr = fopen("metadata.dat", "w");

    if (fptr != NULL) {
        printf("Archivo de metadata creado.\n");
    } else {
        printf("Error el crear el archivo de metadata\n");
    }
    fclose(fptr);
}

void altaDeCampo() {
    char ch;
    FILE *fptr = fopen("metadata.dat", "a");

    if (!fptr) {
        printf("No se pudo abrir el archivo para añadir el campo.\n");
        return;
    }

    // en cada linea de la metadata debo definir numero desc tipo y long
    metadata data;

    //data descripcion
    printf("Ingrese el nombre del campo: ");
    scanf("%s", data.nombre);
    //data longitud
    printf("\nIngrese la longitud: ");
    while (scanf("%d", &data.longitud) != 1) {
        while ((ch = getchar()) != '\n' && ch != EOF);
        printf("La longitud debe ser un valor numerico\n");
        printf("Ingrese nuevamente: ");
    }


    fprintf(fptr, "%s,%d\n", data.nombre, data.longitud);
    fclose(fptr);
}

void mostrarArchivoMetadata() {

    FILE *fptr = fopen("metadata.dat", "r");

    if (fptr == NULL) {
        printf("Error al abrir el archivo.\n");
        return;
    }

    printf("---------- Campos ----------\n");
    metadata data;
    char buffer[100];
    char lineas[100][100];
    int cantidadLineas = 0;

    // Leer todas las líneas y almacenarlas
    while (fgets(buffer, sizeof(buffer), fptr)) {
        strcpy(lineas[cantidadLineas], buffer);
        cantidadLineas++;
    }

    // Mostrar todas las líneas, excepto la última
    for (int i = 0; i < cantidadLineas; i++) {

        if (i == cantidadLineas - 1 && strchr(lineas[i], '*') != NULL) {
            // Si es la última línea y contiene '*', la omitimos
            continue;
        }

        sscanf(lineas[i], "%[^,],%d", data.nombre, &data.longitud);
        printf("Nombre de campo -> %s\n", data.nombre);
        printf("Longitud -> %d\n", data.longitud);
        printf("----------------------------\n");
    }

    fclose(fptr);
}

void definirEstructura() {
    FILE *fptr = fopen("metadata.dat", "a");
    fprintf(fptr, "*");
    fclose(fptr);
}

bool estaDefinida() {
    FILE *fptr = fopen("metadata.dat", "r");

    if (!fptr) {
        return false;
    }


    fseek(fptr, 0, SEEK_END);
    long pos = ftell(fptr);

    if (pos == 0) {
        fclose(fptr);
        return false;
    }


    fseek(fptr, pos - 1, SEEK_SET);
    char ch;
    while (ftell(fptr) > 0 && (ch = getc(fptr)) != '\n') {
        fseek(fptr, -2, SEEK_CUR);
    }

    if (getc(fptr) == '*') {
        fclose(fptr);
        return true;
    }

    fclose(fptr);
    return false;
}

bool comprobarExistencia() {
    FILE *fptr = fopen("metadata.dat", "r");
    if (fptr) {
        fclose(fptr);
        return true;
    }
    return false;
}