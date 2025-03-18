#include <unistd.h> // For getcwd
#include <stdio.h>
#include <string.h>


typedef struct metadata {
   int numero;
   char descripcion[30];
   char tipo; // int O string (i o s)
   int longitud;
} metadata;

void altaDeCampo();
void crearArchivoMetadata();
void mostrarArchivoMetadata();

int main(void) {

   int eleccion;
   printf("1. Crear archivo metadata (Si existe, sobreescribe)\n");
   printf("2. Crear nuevo campo(alta)\n");
   printf("3. Mostrar archivo metadata\n");
   printf("Ingrese una operacion:  ");
   scanf("%d", &eleccion);

   if (eleccion == 1) {
      crearArchivoMetadata();
   } else if (eleccion == 2) {
      altaDeCampo();
   }else if (eleccion == 3) {
      mostrarArchivoMetadata();
   }

   return 0;
}

void crearArchivoMetadata() {

   remove("metadata.dat");
   FILE* fptr = fopen("metadata.dat", "w");

   if (fptr != NULL) {
      printf("Archivo de metadata creado.\n");
   } else {
      printf("Error el crear el archivo de metadata\n");
   }
   fclose(fptr);
}
void altaDeCampo() {

   FILE* fptr = fopen("metadata.dat", "a");

   // en cada linea de la metadata debo definir numero desc tipo y long
   metadata data;

   //data numero
   printf("Ingrese el numero: ");
   scanf("%d", &data.numero);
   //data descripcion
   printf("Ingrese la descripcion: ");
   scanf("%s", data.descripcion);
   //data tipo
   printf("Ingrese el tipo: ");
   scanf(" %c", &data.tipo);
   //data longitud
   printf("Ingrese la longitud: ");
   scanf("%d", &data.longitud);

   fprintf(fptr, "%d,%s,%c,%d \n", data.numero, data.descripcion, data.tipo, data.longitud);

   fclose(fptr);

}
void mostrarArchivoMetadata() {
   FILE* fptr = fopen("metadata.dat", "r");

   printf("---------- Campos ----------\n");
   metadata data;
   char buffer[100];

   while (fgets(buffer, 100, fptr)) {
      sscanf(buffer, "%d,%[^,], %c,%d", &data.numero, data.descripcion, &data.tipo, &data.longitud);

      printf("Numero -> %d\n", data.numero);
      printf("Descripcion -> %s\n", data.descripcion);
      printf("Tipo -> %c\n", data.tipo);
      printf("Longitud -> %d\n", data.longitud);
      printf("----------------------------\n");
   };
   fclose(fptr);
}