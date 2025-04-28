#include <unistd.h> // For getcwd
#include <stdio.h>
#include <string.h>
<<<<<<< Updated upstream
#include <stdbool.h>
=======
#include <stdlib.h>
#ifdef __linux__
    #include <conio.h>
#endif

>>>>>>> Stashed changes

typedef struct metadata {
    char nombre[30];
    int longitud;
} metadata;

//-------------------------------

<<<<<<< Updated upstream
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
=======
// Prototipos de funciones
void limpiarConsola();
void definirMetadata();
void cargarMetadata();
void altaRegistro();
void bajaRegistro();
void modificarRegistro();
void mostrarRegistros();
int hashClave(const char* clave);

int main() {
    int opcion;

    // Intentar cargar metadata existente
    cargarMetadata();

    do {
        printf("\n--- MENU PRINCIPAL ---\n");
        printf("1. Definir metadata\n");
        printf("2. Alta de registro\n");
        printf("3. Baja de registro\n");
        printf("4. Modificar registro\n");
        printf("5. Mostrar registros\n");
        printf("0. Salir\n");
        printf("Opcion: ");
        scanf("%d", &opcion);
        getchar(); // Limpiar buffer

        switch(opcion) {
            case 1: limpiarConsola(); definirMetadata(); break;
            case 2: limpiarConsola(); altaRegistro(); break;
            case 3: limpiarConsola(); bajaRegistro(); break;
            case 4: limpiarConsola(); modificarRegistro(); break;
            case 5: limpiarConsola(); mostrarRegistros(); break;
            case 0: printf("Fin del programa\n"); break;
            default: printf("Opcion invalida\n");
        }
    } while(opcion != 0);

    return 0;
}

void limpiarConsola(){
#ifdef _WIN32
    system("cls");
#else
    #ifdef __linux__
        system("clear");
    #endif
#endif // _WIN32
}

// Función hash simple
int hashClave(const char* clave) {
    int hash = 0;
    for (int i = 0; clave[i] != '\0'; i++) {
        hash = (hash + clave[i]) % TAMANIO_TABLA;
>>>>>>> Stashed changes
    }
    fclose(fptr);
}

void altaDeCampo() {

    printf("El primer campo ingresado sera la clave primaria!\n");
    char ch;
    FILE *fptr = fopen("metadata.dat", "a");

    if (!fptr) {
        printf("No se pudo abrir el archivo para aÃ±adir el campo.\n");
        return;
    }

<<<<<<< Updated upstream
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
=======
    numCampos = 0;
    printf("\n-- DEFINICION DE METADATA --\n");

    // Ingresar campos hasta indicar "fin"
    char nombreCampo[30];
    int longitud;

    while (1) {
        printf("Nombre del campo (o 'fin' para terminar): ");
        scanf("%s", nombreCampo);
        getchar(); // Limpiar buffer

        if (strcmp(nombreCampo, "fin") == 0) {
            break;
        }

        printf("Longitud del campo: ");
        scanf("%d", &longitud);
        getchar(); // Limpiar buffer

        // Guardar en arreglo y archivo
        strcpy(campos[numCampos].nombre, nombreCampo);
        campos[numCampos].longitud = longitud;
        fprintf(archivo, "%s,%d\n", nombreCampo, longitud);

        numCampos++;
        if (numCampos >= MAX_CAMPOS) {
            printf("Limite de campos alcanzado\n");
            break;
        }
    }

    // Pedir clave primaria
    if (numCampos > 0) {
        printf("\nCampos definidos:\n");
        for (int i = 0; i < numCampos; i++) {
            printf("%d. %s\n", i+1, campos[i].nombre);
        }

        printf("Numero del campo que sera clave primaria: ");
        scanf("%d", &indiceClavePrimaria);
        getchar(); // Limpiar buffer

        if (indiceClavePrimaria < 1 || indiceClavePrimaria > numCampos) {
            indiceClavePrimaria = 1; // Por defecto el primer campo
            printf("Valor invalido. Se usara el primer campo como clave.\n");
        } else {
            indiceClavePrimaria--; // Ajustar a base 0 por que si elejis 1 tiene que ser el elemento 0
        }

        fprintf(archivo, "CLAVE_PRIMARIA,%d\n", indiceClavePrimaria);
        printf("El campo '%s' sera la clave primaria.\n", campos[indiceClavePrimaria].nombre);
    }

    fclose(archivo);
}

// Cargar metadata desde archivo
void cargarMetadata() {
    FILE* archivo = fopen(ARCHIVO_METADATA, "r");
    if (!archivo) {
        printf("No existe archivo de metadata\n");
        return;
    }

    char linea[100];
    char nombreCampo[30];
    int valor;
    numCampos = 0;

    while (fgets(linea, sizeof(linea), archivo)) {
        // Quitar el salto de línea
        linea[strcspn(linea, "\n")] = 0;

        char* token = strtok(linea, ",");
        if (!token) continue;

        strcpy(nombreCampo, token);

        token = strtok(NULL, ",");
        if (!token) continue;

        valor = atoi(token);

        if (strcmp(nombreCampo, "CLAVE_PRIMARIA") == 0) {
            indiceClavePrimaria = valor;
        } else {
            strcpy(campos[numCampos].nombre, nombreCampo);
            campos[numCampos].longitud = valor;
            numCampos++;
        }
    }

    fclose(archivo);

    if (numCampos > 0) {
        printf("Metadata cargada: %d campos\n", numCampos);
        printf("Clave primaria:: %s\n", campos[indiceClavePrimaria].nombre);
    }
}

// Alta de registro
void altaRegistro() {
    if (numCampos == 0) {
        printf("Error: No hay metadata definida\n");
        return;
    }

    // Buffer para almacenar todos los datos del registro
    char registro[MAX_CAMPOS * MAX_LONGITUD];
    char valores[MAX_CAMPOS][MAX_LONGITUD];
    memset(registro, 0, sizeof(registro));
    int offset = 0;

    printf("\n-- ALTA DE REGISTRO --\n");

    // Leer valores para cada campo
    for (int i = 0; i < numCampos; i++) {
        printf("%s (max %d caracteres): ", campos[i].nombre, campos[i].longitud);
        scanf("%s", valores[i]);
        getchar(); // Limpiar buffer

        // Truncar si excede la longitud
        if (strlen(valores[i]) > campos[i].longitud) {
            valores[i][campos[i].longitud] = '\0';
            printf("Valor truncado a %d caracteres\n", campos[i].longitud);
        }

        // Copiar al buffer del registro con padding
        strcpy(registro + offset, valores[i]);
        offset += campos[i].longitud;
    }

    // Agregar indicador de activo (0 = activo, 1 = borrado)
    registro[offset] = '0';
    offset++;

    // Obtener clave primaria
    const char* clave = valores[indiceClavePrimaria];
    int posicion = hashClave(clave);

    // Abrir archivo para escritura/lectura
    FILE* archivo = fopen(ARCHIVO_DATOS, "r+b");
    if (!archivo) {
        // Si no existe, crearlo
        archivo = fopen(ARCHIVO_DATOS, "wb");
        if (!archivo) {
            printf("Error al crear archivo de datos\n");
            return;
        }
    }

    // Calcular tamaño de registro
    int tamRegistro = offset;

    // Verificar si ya existe registro con esa clave
    char regExistente[MAX_CAMPOS * MAX_LONGITUD];
    fseek(archivo, posicion * tamRegistro, SEEK_SET);

    if (fread(regExistente, tamRegistro, 1, archivo) == 1) {
        // Verificar si el registro está activo
        if (regExistente[tamRegistro - 1] == '0') {
            // Extraer clave del registro existente
            char claveExistente[MAX_LONGITUD];
            int offsetClave = 0;

            for (int i = 0; i < indiceClavePrimaria; i++) {
                offsetClave += campos[i].longitud;
            }

            memcpy(claveExistente, regExistente + offsetClave, campos[indiceClavePrimaria].longitud);
            claveExistente[campos[indiceClavePrimaria].longitud] = '\0';

            // Comparar claves
            if (strcmp(claveExistente, clave) == 0) {
                printf("Error: Ya existe un registro con esa clave\n");
                fclose(archivo);
                return;
            }
        }
    }

    // Escribir nuevo registro
    fseek(archivo, posicion * tamRegistro, SEEK_SET);
    fwrite(registro, tamRegistro, 1, archivo);

    printf("Registro agregado correctamente\n");
    fclose(archivo);
}
>>>>>>> Stashed changes

    if (fptr == NULL) {
        printf("Error al abrir el archivo.\n");
        return;
    }
<<<<<<< Updated upstream
=======

    char clave[MAX_LONGITUD];
    printf("\n-- BAJA DE REGISTRO --\n");
    printf("Ingrese %s (clave primaria): ", campos[indiceClavePrimaria].nombre);
    scanf("%s", clave);
    getchar(); // Limpiar buffer

    // Calcular posición
    int posicion = hashClave(clave);

    // Abrir archivo
    FILE* archivo = fopen(ARCHIVO_DATOS, "r+b");
    if (!archivo) {
        printf("Error: No existe archivo de datos\n");
        return;
    }

    // Calcular tamaño de registro
    int tamRegistro = 0;
    for (int i = 0; i < numCampos; i++) {
        tamRegistro += campos[i].longitud;
    }
    tamRegistro++; // Para el indicador de borrado

    // Leer registro
    char registro[MAX_CAMPOS * MAX_LONGITUD];
    fseek(archivo, posicion * tamRegistro, SEEK_SET);

    if (fread(registro, tamRegistro, 1, archivo) != 1) {
        printf("Error: No existe registro en esa posicion\n");
        fclose(archivo);
        return;
    }

    // Verificar si está borrado
    if (registro[tamRegistro - 1] == '1') {
        printf("Error: El registro ya fue borrado\n");
        fclose(archivo);
        return;
    }

    // Extraer clave del registro
    char claveReg[MAX_LONGITUD];
    int offsetClave = 0;

    for (int i = 0; i < indiceClavePrimaria; i++) {
        offsetClave += campos[i].longitud;
    }

    memcpy(claveReg, registro + offsetClave, campos[indiceClavePrimaria].longitud);
    claveReg[campos[indiceClavePrimaria].longitud] = '\0';

    // Comparar claves
    if (strcmp(claveReg, clave) != 0) {
        printf("Error: No se encontro registro con esa clave\n");
        fclose(archivo);
        return;
    }

    // Mostrar registro antes de borrar
    printf("\nRegistro encontrado:\n");
    int offset = 0;
    for (int i = 0; i < numCampos; i++) {
        char valor[MAX_LONGITUD];
        memcpy(valor, registro + offset, campos[i].longitud);
        valor[campos[i].longitud] = '\0';
        printf("%s: %s\n", campos[i].nombre, valor);
        offset += campos[i].longitud;
    }

    // Confirmar borrado
    char confirma;
    printf("\nConfirma el borrado? (s/n): ");
    scanf("%c", &confirma);
    getchar(); // Limpiar buffer

    if (confirma != 's' && confirma != 'S') {
        printf("Operacion cancelada\n");
        fclose(archivo);
        return;
    }

    // Marcar como borrado
    registro[tamRegistro - 1] = '1';

    // Guardar cambios
    fseek(archivo, posicion * tamRegistro, SEEK_SET);
    fwrite(registro, tamRegistro, 1, archivo);

    printf("Registro borrado correctamente\n");
    fclose(archivo);
}
>>>>>>> Stashed changes

    printf("---------- Campos ----------\n");
    metadata data;
    char buffer[100];
    char lineas[100][100];
    int cantidadLineas = 0;

    // Leer todas las lÃ­neas y almacenarlas
    while (fgets(buffer, sizeof(buffer), fptr)) {
        strcpy(lineas[cantidadLineas], buffer);
        cantidadLineas++;
    }

<<<<<<< Updated upstream
    // Mostrar todas las lÃ­neas, excepto la Ãºltima
    for (int i = 0; i < cantidadLineas; i++) {

        if (i == cantidadLineas - 1 && strchr(lineas[i], '*') != NULL) {
            // Si es la Ãºltima lÃ­nea y contiene '*', la omitimos
            continue;
        }

        sscanf(lineas[i], "%[^,],%d", data.nombre, &data.longitud);
        if (i == 0) {
            printf("<<< CLAVE PRIMARIA >>>\n");
=======
    char clave[MAX_LONGITUD];
    printf("\n-- MODIFICAR REGISTRO --\n");
    printf("Ingrese %s (clave primaria): ", campos[indiceClavePrimaria].nombre);
    scanf("%s", clave);
    getchar(); // Limpiar buffer

    // Calcular posición
    int posicion = hashClave(clave);

    // Abrir archivo
    FILE* archivo = fopen(ARCHIVO_DATOS, "r+b");
    if (!archivo) {
        printf("Error: No existe archivo de datos\n");
        return;
    }

    // Calcular tamaño de registro
    int tamRegistro = 0;
    for (int i = 0; i < numCampos; i++) {
        tamRegistro += campos[i].longitud;
    }
    tamRegistro++; // Para el indicador de borrado

    // Leer registro
    char registro[MAX_CAMPOS * MAX_LONGITUD];
    fseek(archivo, posicion * tamRegistro, SEEK_SET);

    if (fread(registro, tamRegistro, 1, archivo) != 1) {
        printf("Error: No existe registro en esa posición\n");
        fclose(archivo);
        return;
    }

    // Verificar si está borrado
    if (registro[tamRegistro - 1] == '1') {
        printf("Error: El registro fuie borrado\n");
        fclose(archivo);
        return;
    }

    // Extraer clave del registro
    char claveReg[MAX_LONGITUD];
    int offsetClave = 0;

    for (int i = 0; i < indiceClavePrimaria; i++) {
        offsetClave += campos[i].longitud;
    }

    memcpy(claveReg, registro + offsetClave, campos[indiceClavePrimaria].longitud);
    claveReg[campos[indiceClavePrimaria].longitud] = '\0';

    // Comparar claves
    if (strcmp(claveReg, clave) != 0) {
        printf("Error: No se encontro registro con esa clave\n");
        fclose(archivo);
        return;
    }

    // Mostrar registro actual
    printf("\nRegistro actual:\n");
    int offset = 0;
    char valores[MAX_CAMPOS][MAX_LONGITUD];

    for (int i = 0; i < numCampos; i++) {
        memcpy(valores[i], registro + offset, campos[i].longitud);
        valores[i][campos[i].longitud] = '\0';
        printf("%s: %s\n", campos[i].nombre, valores[i]);
        offset += campos[i].longitud;
    }

    // Modificar campos
    printf("\nIngrese nuevos valores (Enter para mantener el valor actual):\n");

    for (int i = 0; i < numCampos; i++) {
        // No modificar clave primaria
        if (i == indiceClavePrimaria) {
            printf("%s: %s (clave primaria, no modificable)\n", campos[i].nombre, valores[i]);
            continue;
        }

        printf("%s: ", campos[i].nombre);
        char buffer[MAX_LONGITUD];
        fgets(buffer, MAX_LONGITUD, stdin);
        buffer[strcspn(buffer, "\n")] = 0; // Quitar salto de línea

        // Si ingresó algo, actualizar valor
        if (strlen(buffer) > 0) {
            // Truncar si excede longitud
            if (strlen(buffer) > campos[i].longitud) {
                buffer[campos[i].longitud] = '\0';
                printf("Valor truncado a %d caracteres\n", campos[i].longitud);
            }
            strcpy(valores[i], buffer);
>>>>>>> Stashed changes
        }
        printf("Nombre de campo -> %s\n", data.nombre);
        printf("Longitud -> %d\n", data.longitud);
        printf("----------------------------\n");
    }

<<<<<<< Updated upstream
    fclose(fptr);
=======
    // Reconstruir registro
    offset = 0;
    for (int i = 0; i < numCampos; i++) {
        // Copiar con padding al registro
        memset(registro + offset, 0, campos[i].longitud);
        strcpy(registro + offset, valores[i]);
        offset += campos[i].longitud;
    }

    // Mantener indicador de activo
    registro[offset] = '0';

    // Guardar cambios
    fseek(archivo, posicion * tamRegistro, SEEK_SET);
    fwrite(registro, tamRegistro, 1, archivo);

    printf("Registro modificado correctamente\n");
    fclose(archivo);
>>>>>>> Stashed changes
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

<<<<<<< Updated upstream

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
=======
    FILE* archivo = fopen(ARCHIVO_DATOS, "rb");
    if (!archivo) {
        printf("Error: No existe archivo de datos\n");
        return;
    }

    // Calcular tamaño de registro
    int tamRegistro = 0;
    for (int i = 0; i < numCampos; i++) {
        tamRegistro += campos[i].longitud;
    }
    tamRegistro++; // Para el indicador de borrado

    printf("\n-- REGISTROS EXISTNTES --\n");

    char registro[MAX_CAMPOS * MAX_LONGITUD];
    int contador = 0;

    // Recorrer todas las posiciones posibles en la tabla hash
    for (int i = 0; i < TAMANIO_TABLA; i++) {
        fseek(archivo, i * tamRegistro, SEEK_SET);

        if (fread(registro, tamRegistro, 1, archivo) == 1) {
            // Verificar si el registro está activo
            if (registro[tamRegistro - 1] == '0') {
                printf("\nRegistro #%d (posicion %d):\n", ++contador, i);

                int offset = 0;
                for (int j = 0; j < numCampos; j++) {
                    char valor[MAX_LONGITUD];
                    memcpy(valor, registro + offset, campos[j].longitud);
                    valor[campos[j].longitud] = '\0';
                    printf("%s: %s\n", campos[j].nombre, valor);
                    offset += campos[j].longitud;
                }
                printf("-----------------\n");
            }
        }
    }

    if (contador == 0) {
        printf("No hay registros para mostrar\n");
    }

    fclose(archivo);
}
>>>>>>> Stashed changes
