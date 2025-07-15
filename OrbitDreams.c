#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAX_CLIENTES 100
#define MAX_CARACTERES 50
#define MAX_VIAJES 100

// --- ESTRUCTURAS ---
struct Cliente {
    char nombre[MAX_CARACTERES];
    char contra[MAX_CARACTERES];
};

struct Destino {
    char nombre[MAX_CARACTERES];
    int costo;
};

struct Cupon {
    char codigo[10];
    float descuento;
};

struct InformacionViaje {
    char nombreCliente[MAX_CARACTERES];
    char destinoViaje[MAX_CARACTERES];
    char lugarSalida[MAX_CARACTERES];
    char horaSalida[10];
    int numPersonas;
};


// --- PROTOTIPOS DE FUNCIONES ---

// Funciones de utilidad y persistencia
void inicializarDestinos(FILE *archivDestinos);
void reescribirArchivoClientes(struct Cliente clientes[], int numClientes);
void reescribirArchivoDestinos(struct Destino destinos[], int numDestinos);
void limpiarBufferEntrada();

// Funciones de Administrador
void menuAdministrador(struct Cliente clientes[], int *numClientes, struct Destino destinos[], int *numDestinos);
int verificarADM();
void editarCliente(struct Cliente clientes[], int *numClientes);
void agregarCliente(struct Cliente clientes[], int *numClientes);
void modificarCliente(struct Cliente clientes[], int numClientes);
void eliminarCliente(struct Cliente clientes[], int *numClientes);
void editarViaje(struct Destino destinos[], int *numDestinos);
void agregarViaje(struct Destino destinos[], int *numDestinos);
void modificarViaje(struct Destino destinos[], int numDestinos);
void eliminarViaje(struct Destino destinos[], int *numDestinos);

// Funciones de Cliente
void menuCliente(struct Destino destinos[], int numDestinos);
int verificarCliente();
void mostrarDestinos(struct Destino destinos[], int numDestinos);
void mostrarCupones(struct Cupon cupones[], int numCupones);
float aplicarDescuento(float costo, float descuento);
void generarTicketCompra(struct Destino destino, struct InformacionViaje informacionViaje);
void generarArchivoSaldoInsuficiente();
void obtenerHoraAleatoria(char horaAleatoria[]);

// Función de información
void mostrarInformacionEmpresa();


// --- FUNCIÓN PRINCIPAL ---
int main() {
    // Semilla para números aleatorios
    srand(time(NULL));

    // Abrir archivos
    FILE *archivDestinos = fopen("Destinos.txt", "r+");
    if (archivDestinos == NULL) {
        archivDestinos = fopen("Destinos.txt", "w+"); // Si no existe, lo crea
        if (archivDestinos == NULL){
            printf("Error crítico: No se pudo crear el archivo de destinos.\n");
            return 1;
        }
    }

    FILE *archivNombres = fopen("clientes.txt", "a+");
    FILE *archivContras = fopen("contraseñas.txt", "a+");

    if (archivNombres == NULL || archivContras == NULL) {
        printf("Error crítico: No se pudo abrir los archivos de clientes.\n");
        if(archivDestinos) fclose(archivDestinos);
        return 1;
    }

    // --- Carga de datos inicial ---
    struct Cliente clientes[MAX_CLIENTES];
    struct Destino destinos[MAX_VIAJES];
    int numClientes = 0;
    int numDestinos = 0;

    // Inicializar destinos si el archivo está vacío
    fseek(archivDestinos, 0, SEEK_END);
    if (ftell(archivDestinos) == 0) {
        inicializarDestinos(archivDestinos);
    }
    rewind(archivDestinos);

    // Cargar destinos del archivo a la memoria
    while (numDestinos < MAX_VIAJES && fscanf(archivDestinos, "%s %d", destinos[numDestinos].nombre, &destinos[numDestinos].costo) == 2) {
        numDestinos++;
    }

    // Cargar clientes de los archivos a la memoria
    rewind(archivNombres);
    rewind(archivContras);
    while (numClientes < MAX_CLIENTES &&
           fscanf(archivNombres, "%s", clientes[numClientes].nombre) == 1 &&
           fscanf(archivContras, "%s", clientes[numClientes].contra) == 1) {
        numClientes++;
    }

    int opcion;
    do {
        printf("\n--- Menu Principal ---\n");
        printf("1) Administrador\n");
        printf("2) Viajes Disponibles (Cliente)\n");
        printf("3) Contactar Programadores\n");
        printf("4) Salir\n");
        printf("Ingrese su opcion: ");
        scanf("%d", &opcion);
        limpiarBufferEntrada(); 

        switch (opcion) {
            case 1:
                if (verificarADM()) {
                    menuAdministrador(clientes, &numClientes, destinos, &numDestinos);
                } else {
                    printf("Usuario o contraseña de administrador incorrecta.\n");
                }
                break;
            case 2:
                if (verificarCliente()) {
                    menuCliente(destinos, numDestinos);
                } else {
                    printf("Credenciales de cliente incorrectas.\n");
                }
                break;
            case 3:
                mostrarInformacionEmpresa();
                break;
            case 4:
                printf("Saliendo del programa.\n");
                break;
            default:
                printf("Opcion invalida. Intente de nuevo.\n");
        }
    } while (opcion != 4);

    // Cerrar todos los archivos antes de terminar
    fclose(archivNombres);
    fclose(archivContras);
    fclose(archivDestinos);
    return 0;
}

void limpiarBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void inicializarDestinos(FILE *archivDestinos) {
    fprintf(archivDestinos, "Jupiter 7000\n");
    fprintf(archivDestinos, "La_Luna 5000\n");
    fprintf(archivDestinos, "Marte 6000\n");
    fprintf(archivDestinos, "Saturno 8000\n");
}

void reescribirArchivoClientes(struct Cliente clientes[], int numClientes) {
    FILE *archivNombres = fopen("clientes.txt", "w"); 
    FILE *archivContras = fopen("contraseñas.txt", "w");
    if (!archivNombres || !archivContras) {
        printf("Error al reescribir archivos de clientes.\n");
        return;
    }
    for (int i = 0; i < numClientes; i++) {
        fprintf(archivNombres, "%s\n", clientes[i].nombre);
        fprintf(archivContras, "%s\n", clientes[i].contra);
    }
    fclose(archivNombres);
    fclose(archivContras);
}

void reescribirArchivoDestinos(struct Destino destinos[], int numDestinos) {
    FILE *archivDestinos = fopen("Destinos.txt", "w");
    if (!archivDestinos) {
        printf("Error al reescribir archivo de destinos.\n");
        return;
    }
    for (int i = 0; i < numDestinos; i++) {
        fprintf(archivDestinos, "%s %d\n", destinos[i].nombre, destinos[i].costo);
    }
    fclose(archivDestinos);
}


// --- FUNCIONES DE ADMINISTRADOR ---
void menuAdministrador(struct Cliente clientes[], int *numClientes, struct Destino destinos[], int *numDestinos) {
    int opcion;
    do {
        printf("\n---- Menu Administrador ----\n");
        printf("1) Editar cuentas de clientes\n");
        printf("2) Editar viajes\n");
        printf("3) Volver al Menu Principal\n");
        printf("Ingrese su opcion: ");
        scanf("%d", &opcion);
        limpiarBufferEntrada();

        switch (opcion) {
            case 1:
                editarCliente(clientes, numClientes);
                break;
            case 2:
                editarViaje(destinos, numDestinos);
                break;
            case 3:
                return; // Vuelve al menú principal
            default:
                printf("Opción inválida. Intente de nuevo.\n");
        }
    } while (1);
}

int verificarADM() {
    char usuario[MAX_CARACTERES], contrasena[MAX_CARACTERES];
    printf("Ingrese el usuario de administrador: ");
    fgets(usuario, sizeof(usuario), stdin);
    usuario[strcspn(usuario, "\n")] = '\0';

    printf("Ingrese la contrasena: ");
    fgets(contrasena, sizeof(contrasena), stdin);
    contrasena[strcspn(contrasena, "\n")] = '\0';

    return (strcmp(usuario, "orbitdreams") == 0 && strcmp(contrasena, "dorito") == 0);
}

void editarCliente(struct Cliente clientes[], int *numClientes) {
    int opcion;
    printf("\n¿Qué desea hacer?\n");
    printf("1) Agregar la cuenta de un cliente\n");
    printf("2) Modificar la cuenta de un cliente\n");
    printf("3) Eliminar la cuenta de un cliente\n");
    printf("Ingrese su opcion: ");
    scanf("%d", &opcion);
    limpiarBufferEntrada();

    switch (opcion) {
        case 1: agregarCliente(clientes, numClientes); break;
        case 2: modificarCliente(clientes, *numClientes); break;
        case 3: eliminarCliente(clientes, numClientes); break;
        default: printf("Opción inválida.\n");
    }
}

void agregarCliente(struct Cliente clientes[], int *numClientes) {
    if (*numClientes >= MAX_CLIENTES) {
        printf("No se pueden agregar mas clientes, límite alcanzado.\n");
        return;
    }

    printf("Ingrese el nombre del nuevo cliente: ");
    fgets(clientes[*numClientes].nombre, MAX_CARACTERES, stdin);
    clientes[*numClientes].nombre[strcspn(clientes[*numClientes].nombre, "\n")] = '\0';

    printf("Ingrese la contrasena del nuevo cliente: ");
    fgets(clientes[*numClientes].contra, MAX_CARACTERES, stdin);
    clientes[*numClientes].contra[strcspn(clientes[*numClientes].contra, "\n")] = '\0';

    // Añadir a los archivos
    FILE *archivNombres = fopen("clientes.txt", "a");
    FILE *archivContras = fopen("contraseñas.txt", "a");
    if(archivNombres && archivContras){
        fprintf(archivNombres, "%s\n", clientes[*numClientes].nombre);
        fprintf(archivContras, "%s\n", clientes[*numClientes].contra);
        fclose(archivNombres);
        fclose(archivContras);
    }
    
    (*numClientes)++;
    printf("Cliente agregado con exito.\n");
}

void modificarCliente(struct Cliente clientes[], int numClientes) {
    if (numClientes == 0) {
        printf("No hay clientes para modificar.\n");
        return;
    }
    
    printf("Clientes disponibles:\n");
    for (int i = 0; i < numClientes; i++) {
        printf("%d) %s\n", i + 1, clientes[i].nombre);
    }

    int indice;
    printf("Ingrese el número del cliente a modificar: ");
    scanf("%d", &indice);
    limpiarBufferEntrada();

    if (indice < 1 || indice > numClientes) {
        printf("Número de cliente inválido.\n");
        return;
    }
    indice--; // Ajustar a índice de arreglo (base 0)

    printf("Ingrese el nuevo nombre para el cliente: ");
    fgets(clientes[indice].nombre, MAX_CARACTERES, stdin);
    clientes[indice].nombre[strcspn(clientes[indice].nombre, "\n")] = '\0';

    printf("Ingrese la nueva contraseña para el cliente: ");
    fgets(clientes[indice].contra, MAX_CARACTERES, stdin);
    clientes[indice].contra[strcspn(clientes[indice].contra, "\n")] = '\0';
    
    // Guardar todos los cambios en los archivos
    reescribirArchivoClientes(clientes, numClientes);

    printf("Cliente modificado con éxito.\n");
}

void eliminarCliente(struct Cliente clientes[], int *numClientes) {
    if (*numClientes == 0) {
        printf("No hay clientes para eliminar.\n");
        return;
    }

    printf("Clientes disponibles:\n");
    for (int i = 0; i < *numClientes; i++) {
        printf("%d) %s\n", i + 1, clientes[i].nombre);
    }
    
    int indice;
    printf("Ingrese el número del cliente a eliminar: ");
    scanf("%d", &indice);
    limpiarBufferEntrada();

    if (indice < 1 || indice > *numClientes) {
        printf("Número de cliente inválido.\n");
        return;
    }
    indice--;

    // Mover los elementos para llenar el espacio del eliminado
    for (int i = indice; i < *numClientes - 1; i++) {
        strcpy(clientes[i].nombre, clientes[i+1].nombre);
        strcpy(clientes[i].contra, clientes[i+1].contra);
    }
    (*numClientes)--;

    // Guardar los cambios en los archivos
    reescribirArchivoClientes(clientes, *numClientes);

    printf("Cliente eliminado con éxito.\n");
}


void editarViaje(struct Destino destinos[], int *numDestinos) {
    int opcion;
    do {
        printf("\n---- Menu Editar Viaje ----\n");
        printf("1) Agregar destino\n");
        printf("2) Modificar destino\n");
        printf("3) Eliminar destino\n");
        printf("4) Volver al Menu Administrador\n");
        printf("Ingrese su opcion: ");
        scanf("%d", &opcion);
        limpiarBufferEntrada();

        switch (opcion) {
            case 1: agregarViaje(destinos, numDestinos); break;
            case 2: modificarViaje(destinos, *numDestinos); break;
            case 3: eliminarViaje(destinos, numDestinos); break;
            case 4: return;
            default: printf("Opción inválida. Intente de nuevo.\n");
        }
    } while (1);
}

void agregarViaje(struct Destino destinos[], int *numDestinos) {
    if (*numDestinos >= MAX_VIAJES) {
        printf("No se pueden agregar más destinos, límite alcanzado.\n");
        return;
    }

    printf("Ingrese el nombre del nuevo destino (sin espacios): ");
    scanf("%s", destinos[*numDestinos].nombre);
    limpiarBufferEntrada();

    printf("Ingrese el costo del nuevo destino: ");
    scanf("%d", &destinos[*numDestinos].costo);
    limpiarBufferEntrada();

    (*numDestinos)++;
    reescribirArchivoDestinos(destinos, *numDestinos);
    printf("Destino agregado con éxito.\n");
}

void modificarViaje(struct Destino destinos[], int numDestinos) {
    if (numDestinos == 0) {
        printf("No hay destinos para modificar.\n");
        return;
    }
    
    mostrarDestinos(destinos, numDestinos);
    int indice;
    printf("Ingrese el número del destino a modificar: ");
    scanf("%d", &indice);
    limpiarBufferEntrada();

    if (indice < 1 || indice > numDestinos) {
        printf("Número de destino inválido.\n");
        return;
    }
    indice--;

    printf("Ingrese el nuevo nombre para el destino (sin espacios): ");
    scanf("%s", destinos[indice].nombre);
    limpiarBufferEntrada();

    printf("Ingrese el nuevo costo para el destino: ");
    scanf("%d", &destinos[indice].costo);
    limpiarBufferEntrada();

    reescribirArchivoDestinos(destinos, numDestinos);
    printf("Destino modificado con éxito.\n");
}

void eliminarViaje(struct Destino destinos[], int *numDestinos) {
     if (*numDestinos == 0) {
        printf("No hay destinos para eliminar.\n");
        return;
    }
    
    mostrarDestinos(destinos, *numDestinos);
    int indice;
    printf("Ingrese el número del destino a eliminar: ");
    scanf("%d", &indice);
    limpiarBufferEntrada();

    if (indice < 1 || indice > *numDestinos) {
        printf("Número de destino inválido.\n");
        return;
    }
    indice--;

    for (int i = indice; i < *numDestinos - 1; i++) {
        strcpy(destinos[i].nombre, destinos[i+1].nombre);
        destinos[i].costo = destinos[i+1].costo;
    }
    (*numDestinos)--;

    reescribirArchivoDestinos(destinos, *numDestinos);
    printf("Destino eliminado con éxito.\n");
}


// --- FUNCIONES DE CLIENTE ---
void menuCliente(struct Destino destinos[], int numDestinos) {
    // Definición de cupones (datos estáticos para el ejemplo)
    struct Cupon cupones[] = {
        {"VERANO25", 0.10}, // 10% descuento
        {"PRIMEVIP", 0.25}  // 25% descuento
    };
    int numCupones = sizeof(cupones) / sizeof(cupones[0]);
    
    // NOTA: El saldo es temporal y se reinicia cada vez que se entra al menú.
    float saldo = 0.0;
    int viajeElegido = 0; // 0 significa que no se ha elegido ninguno
    struct InformacionViaje infoViaje;
    int opcion;

    do {
        printf("\n------- Bienvenido al Menu Cliente ------\n");
        printf("Saldo actual: $%.2f\n", saldo);
        printf("1. Mostrar destinos disponibles\n");
        printf("2. Elegir viaje y aplicar cupon\n");
        printf("3. Ingresar dinero\n");
        printf("4. Comprar viaje\n");
        printf("5. Volver al menu principal\n");
        printf("Ingrese su opcion: ");
        scanf("%d", &opcion);
        limpiarBufferEntrada();

        switch (opcion) {
            case 1:
                mostrarDestinos(destinos, numDestinos);
                break;

            case 2: { 
                mostrarDestinos(destinos, numDestinos);
                printf("Elija el numero del viaje (1-%d): ", numDestinos);
                scanf("%d", &viajeElegido);
                limpiarBufferEntrada();

                if (viajeElegido >= 1 && viajeElegido <= numDestinos) {
                    float costoActual = destinos[viajeElegido - 1].costo;
                    printf("Ha elegido %s. Costo: $%d\n", destinos[viajeElegido - 1].nombre, (int)costoActual);
                    
                    mostrarCupones(cupones, numCupones);
                    char codigoCupon[10];
                    printf("Ingrese el codigo del cupon (o 'NINGUNO' si no tiene): ");
                    fgets(codigoCupon, sizeof(codigoCupon), stdin);
                    codigoCupon[strcspn(codigoCupon, "\n")] = '\0';

                    int cuponEncontrado = 0;
                    for (int i = 0; i < numCupones; i++) {
                        if (strcasecmp(codigoCupon, cupones[i].codigo) == 0) {
                            costoActual = aplicarDescuento(costoActual, cupones[i].descuento);
                            printf("¡Cupón aplicado! Nuevo costo del viaje: $%.2f\n", costoActual);
                            cuponEncontrado = 1;
                            break;
                        }
                    }
                    if (!cuponEncontrado) {
                        printf("Cupon no valido o no aplicado.\n");
                    }
                } else {
                    printf("Numero de viaje no valido.\n");
                    viajeElegido = 0; // Reiniciar si la elección es inválida
                }
                break;
            }

            case 3: { 
                float montoIngresado;
                printf("Ingrese la cantidad de dinero a ingresar: ");
                scanf("%f", &montoIngresado);
                limpiarBufferEntrada();
                if (montoIngresado > 0) {
                    saldo += montoIngresado;
                    printf("Saldo actualizado: $%.2f\n", saldo);
                } else {
                    printf("Monto inválido.\n");
                }
                break;
            }

            case 4:
                if (viajeElegido >= 1 && viajeElegido <= numDestinos) {
                    printf("Ingrese la cantidad de personas que van a viajar: ");
                    scanf("%d", &infoViaje.numPersonas);
                    limpiarBufferEntrada();

                    float costoTotal = destinos[viajeElegido - 1].costo * infoViaje.numPersonas;

                    if (saldo >= costoTotal) {
                        saldo -= costoTotal;
                        printf("Ingrese su nombre (para el ticket): ");
                        fgets(infoViaje.nombreCliente, sizeof(infoViaje.nombreCliente), stdin);
                        infoViaje.nombreCliente[strcspn(infoViaje.nombreCliente, "\n")] = '\0';
                        
                        strcpy(infoViaje.destinoViaje, destinos[viajeElegido - 1].nombre);
                        strcpy(infoViaje.lugarSalida, "Tierra");
                        obtenerHoraAleatoria(infoViaje.horaSalida);

                        generarTicketCompra(destinos[viajeElegido - 1], infoViaje);
                        viajeElegido = 0; // Resetear viaje después de la compra
                    } else {
                        generarArchivoSaldoInsuficiente();
                        printf("Saldo insuficiente. Se ha generado 'saldoinsuficiente.txt'.\n");
                    }
                } else {
                    printf("Seleccione un viaje antes de intentar comprar (Opción 2).\n");
                }
                break;

            case 5:
                return;

            default:
                printf("Opcion no válida. Intente de nuevo.\n");
        }
    } while (1);
}

int verificarCliente() {
    char usuario[MAX_CARACTERES], contrasena[MAX_CARACTERES];
    printf("Ingrese su usuario: ");
    fgets(usuario, sizeof(usuario), stdin);
    usuario[strcspn(usuario, "\n")] = '\0';

    printf("Ingrese su contraseña: ");
    fgets(contrasena, sizeof(contrasena), stdin);
    contrasena[strcspn(contrasena, "\n")] = '\0';

    FILE *archivNombres = fopen("clientes.txt", "r");
    FILE *archivContras = fopen("contraseñas.txt", "r");
    if (!archivNombres || !archivContras) {
        printf("Error al abrir archivos de clientes para verificación.\n");
        return 0;
    }

    char usuarioArchivo[MAX_CARACTERES];
    char contrasenaArchivo[MAX_CARACTERES];
    int credencialesValidas = 0;

    // Buscar si el usuario y la contraseña coinciden en la misma "línea"
    while (fscanf(archivNombres, "%s", usuarioArchivo) == 1 && fscanf(archivContras, "%s", contrasenaArchivo) == 1) {
        if (strcmp(usuario, usuarioArchivo) == 0 && strcmp(contrasena, contrasenaArchivo) == 0) {
            credencialesValidas = 1;
            break;
        }
    }

    fclose(archivNombres);
    fclose(archivContras);
    return credencialesValidas;
}

void mostrarDestinos(struct Destino destinos[], int numDestinos) {
    printf("\n--- Destinos y costos (USD) ---\n");
    if(numDestinos == 0){
        printf("No hay destinos disponibles en este momento.\n");
        return;
    }
    for (int i = 0; i < numDestinos; i++) {
        printf("%d) %s: $%d\n", i + 1, destinos[i].nombre, destinos[i].costo);
    }
    printf("-------------------------------\n");
}


void mostrarCupones(struct Cupon cupones[], int numCupones) {
    printf("--- Cupones disponibles ---\n");
    for (int i = 0; i < numCupones; i++) {
        printf("Codigo: %s (%.0f%% de descuento)\n", cupones[i].codigo, cupones[i].descuento * 100);
    }
    printf("---------------------------\n");
}

float aplicarDescuento(float costo, float descuento) {
    return costo * (1.0 - descuento);
}

void generarTicketCompra(struct Destino destino, struct InformacionViaje infoViaje) {
    FILE *ticketFile = fopen("ticketdecompra.txt", "w");
    if (ticketFile == NULL) {
        printf("No se pudo crear el archivo de ticket.\n");
        return;
    }

    fprintf(ticketFile, "--- TICKET DE COMPRA - ORBITDREAMS ---\n");
    fprintf(ticketFile, "Nombre del Cliente: %s\n", infoViaje.nombreCliente);
    fprintf(ticketFile, "Destino del Viaje: %s\n", infoViaje.destinoViaje);
    fprintf(ticketFile, "Lugar de Salida: %s\n", infoViaje.lugarSalida);
    fprintf(ticketFile, "Hora de Salida: %s\n", infoViaje.horaSalida);
    fprintf(ticketFile, "Numero de Personas: %d\n", infoViaje.numPersonas);
    fprintf(ticketFile, "Costo Total Pagado: $%d\n", destino.costo * infoViaje.numPersonas);
    fprintf(ticketFile, "-------------------------------------\n");
    fprintf(ticketFile, "\n¡Gracias por su compra y buen viaje!\n");

    fclose(ticketFile);
    printf("¡Compra exitosa! Se ha generado 'ticketdecompra.txt' con los detalles.\n");
}

void generarArchivoSaldoInsuficiente() {
    FILE *archivo = fopen("saldoinsuficiente.txt", "w");
    if (archivo == NULL) {
        printf("No se pudo crear el archivo de saldo insuficiente.\n");
        return;
    }
    fprintf(archivo, "La compra fue rechazada por saldo insuficiente.\n");
    fprintf(archivo, "Por favor, ingrese más fondos (Opción 3) e intente de nuevo.\n");
    fclose(archivo);
}


void obtenerHoraAleatoria(char horaAleatoria[]) {
    int hora = rand() % 24;
    int minuto = rand() % 60;
    sprintf(horaAleatoria, "%02d:%02d", hora, minuto);
}

void mostrarInformacionEmpresa() {
    printf("\n--- OrbitDreams ---\n");
    printf("Somos una empresa dedicada a hacer tus sueños de viajar por el cosmos una realidad.\n");
    printf("\n--- PROGRAMADORES DE ORBITDREAMS ---\n");
    printf("Nombre: Chimal Mosso\nCorreo: chimalmossocarlos49@gmail.com\n\n");
    printf("Nombre: Gomez Erick\nCorreo: orcinusorca2010@gmail.com\n\n");
    printf("Nombre: Hernandez Pablo\nCorreo: pablohernandezluis08@gmail.com\n\n");
    printf("Nombre: Sanchez Ivan\nCorreo: diego.sanchez09052005@gmail.com\n\n");
    printf("Nombre: Gomez Jorge\nCorreo: jorge@gmail.com\n\n");
}