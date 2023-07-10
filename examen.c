#include <stdio.h>
#include <stdlib.h>
#include <string.h>


 struct {
    int codigo;
    char nombre[50];
    char carrera[50];
    float nota1;
    float nota2;
    float nota3;
    float notaPromedio;
} Estudiante;

// Función para leer los datos de un archivo y almacenarlos en un arreglo de estudiantes
void leerDatosArchivo(const char* nombreArchivo, Estudiante** estudiantes, int* numEstudiantes) {
    FILE* archivo = fopen(nombreArchivo, "r");
    if (archivo == NULL) {
        printf("Error al abrir el archivo %s\n", nombreArchivo);
        exit(1);
    }

    // Contar el número de estudiantes en el archivo
    *numEstudiantes = 0;
    char linea[256];
    while (fgets(linea, sizeof(linea), archivo) != NULL) {
        (*numEstudiantes)++;
    }

    // Volver al inicio del archivo
    rewind(archivo);

    // Asignar memoria para el arreglo de estudiantes
    *estudiantes = (Estudiante*)malloc(*numEstudiantes * sizeof(Estudiante));
    if (*estudiantes == NULL) {
        printf("Error al asignar memoria\n");
        exit(1);
    }

    // Leer los datos de cada estudiante y almacenarlos en el arreglo
    int i = 0;
    while (fgets(linea, sizeof(linea), archivo) != NULL) {
        Estudiante* estudiante = &((*estudiantes)[i]);

        // Utilizar sscanf para leer los campos de cada línea
        sscanf(linea, "%d;%[^;];%[^;];%f;%f;%f", &estudiante->codigo, estudiante->nombre, estudiante->carrera,
               &estudiante->nota1, &estudiante->nota2, &estudiante->nota3);

        // Calcular la nota promedio
        estudiante->notaPromedio = (estudiante->nota1 + estudiante->nota2 + estudiante->nota3) / 3.0;

        i++;
    }

    // Cerrar el archivo
    fclose(archivo);
}

// Función de comparación para ordenar los estudiantes de manera descendente según su nota promedio
int compararEstudiantes(const void* estudiante1, const void* estudiante2) {
    Estudiante* e1 = (Estudiante*)estudiante1;
    Estudiante* e2 = (Estudiante*)estudiante2;

    if (e1->notaPromedio < e2->notaPromedio) {
        return 1;
    } else if (e1->notaPromedio > e2->notaPromedio) {
        return -1;
    } else {
        return 0;
    }
}

// Función para escribir los datos ordenados en un nuevo archivo
void escribirDatosArchivo(const char* nombreArchivo, Estudiante* estudiantes, int numEstudiantes) {
    FILE* archivo = fopen(nombreArchivo, "w");
    if (archivo == NULL) {
        printf("Error al abrir el archivo %s\n", nombreArchivo);
        exit(1);
    }

    for (int i = 0; i < numEstudiantes; i++) {
        fprintf(archivo, "%d;%s;%s;%.2f;%.2f;%.2f;%.2f\n", estudiantes[i].codigo, estudiantes[i].nombre,
                estudiantes[i].carrera, estudiantes[i].nota1, estudiantes[i].nota2, estudiantes[i].nota3,
                estudiantes[i].notaPromedio);
    }

    // Cerrar el archivo
    fclose(archivo);
}

int main() {
    // Nombre del archivo de entrada y salida
    const char* archivoEntrada = "datos.txt";
    const char* archivoSalida = "datos_ordenados.txt";

    Estudiante* estudiantes;
    int numEstudiantes;

    // Leer los datos del archivo de entrada
    leerDatosArchivo(archivoEntrada, &estudiantes, &numEstudiantes);

    // Ordenar los estudiantes según su nota promedio
    qsort(estudiantes, numEstudiantes, sizeof(Estudiante), compararEstudiantes);

    // Escribir los datos ordenados en el archivo de salida
    escribirDatosArchivo(archivoSalida, estudiantes, numEstudiantes);

    // Liberar la memoria asignada al arreglo de estudiantes
    free(estudiantes);

    printf("Datos ordenados y promedios calculados correctamente en el archivo %s\n", archivoSalida);

    return 0;
}
