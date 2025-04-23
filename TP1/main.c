#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ARCHIVO_METADATA "metadata.dat"
#define ARCHIVO_DATOS "datos.dat"
#define MAX_CAMPOS 10
#define MAX_LONGITUD 100
#define TAMANIO_TABLA 10

// Estructura para metadatos
typedef struct {
    char nombre[30];
    int longitud;
} Campo;

// Variables globales
Campo campos[MAX_CAMPOS];
int numCampos = 0;
int indiceClavePrimaria = 0;

// Prototipos de funciones
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
            case 1: definirMetadata(); break;
            case 2: altaRegistro(); break;
            case 3: bajaRegistro(); break;
            case 4: modificarRegistro(); break;
            case 5: mostrarRegistros(); break;
            case 0: printf("Fin del programa\n"); break;
            default: printf("Opción inválida\n");
        }
    } while(opcion != 0);
    
    return 0;
}

// Función hash simple
int hashClave(const char* clave) {
    int hash = 0;
    for (int i = 0; clave[i] != '\0'; i++) {
        hash = (hash + clave[i]) % TAMANIO_TABLA;
    }
    return hash;
}

// Definir estructura de metadatos
void definirMetadata() {
    FILE* archivo = fopen(ARCHIVO_METADATA, "w");
    if (!archivo) {
        printf("Error al crear archivo de metadata\n");
        return;
    }
    
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
            printf("Límite de campos alcanzado\n");
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
            printf("Valor inválido. Se usará el primer campo como clave.\n");
        } else {
            indiceClavePrimaria--; // Ajustar a base 0 por que si elejis 1 tiene que ser el elemento 0
        }
        
        fprintf(archivo, "CLAVE_PRIMARIA,%d\n", indiceClavePrimaria);
        printf("El campo '%s' será la clave primaria.\n", campos[indiceClavePrimaria].nombre);
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
        printf("Clave primaria: %s\n", campos[indiceClavePrimaria].nombre);
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

// Baja de registro
void bajaRegistro() {
    if (numCampos == 0) {
        printf("Error: No hay metadata definida\n");
        return;
    }
    
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
        printf("Error: No existe registro en esa posición\n");
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
        printf("Error: No se encontró registro con esa clave\n");
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
        printf("Operación cancelada\n");
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

// Modificar registro
void modificarRegistro() {
    if (numCampos == 0) {
        printf("Error: No hay metadata definida\n");
        return;
    }
    
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
        printf("Error: El registro fue borrado\n");
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
        printf("Error: No se encontró registro con esa clave\n");
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
        }
    }
    
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
}

// Mostrar registros
void mostrarRegistros() {
    if (numCampos == 0) {
        printf("Error: No hay metadata definida\n");
        return;
    }
    
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
    
    printf("\n-- REGISTROS EXISTENTES --\n");
    
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