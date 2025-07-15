# OrbitDreams - Sistema de Agencia de Viajes Espaciales 🚀

## Descripción del Proyecto

**OrbitDreams** es un sistema de gestión básico para una agencia de viajes espaciales ficticia, desarrollado íntegramente en C. Este proyecto funciona como una aplicación de consola (CLI) que simula las operaciones de administración y compra de viajes, utilizando archivos de texto plano para la persistencia de datos.

-----

## 🛸 Características Principales

El sistema se divide en dos roles con diferentes capacidades:

### 👨‍💼 Rol de Administrador

  * **Gestión de Clientes**: Permite agregar, modificar y eliminar las cuentas de los clientes.
  * **Gestión de Destinos**: Ofrece control total para añadir, editar o quitar los destinos espaciales y sus precios.
  * **Acceso Seguro**: El panel de administrador está protegido por un usuario y contraseña fijos.

### 🧑‍🚀 Rol de Cliente

  * **Inicio de Sesión**: Los clientes deben autenticarse con credenciales creadas previamente por un administrador.
  * **Catálogo de Viajes**: Pueden visualizar una lista de todos los destinos disponibles y sus costos.
  * **Simulación de Compra**: Permite elegir un viaje, aplicar cupones de descuento y realizar un pago virtual.
  * **Generación de Tickets**: Al completar una compra, se genera un archivo `ticketdecompra.txt` con los detalles.

-----

## 🛠️ Tecnologías Utilizadas

  * **Lenguaje**: C (Estándar C99)
  * **Librerías Principales**: `stdio.h`, `stdlib.h`, `string.h`, `time.h`

-----

## ⚙️ Compilación y Ejecución

Para poder compilar y correr este proyecto, necesitarás un compilador de C, como **GCC**.

1.  **Clona o descarga el repositorio en tu máquina local.**

2.  **Abre una terminal** en el directorio donde se encuentran los archivos.

3.  **Compila el proyecto** usando el siguiente comando:

    ```bash
    gcc OrbitDreams.c -o OrbitDreams
    ```

4.  **Ejecuta el programa** desde la misma terminal:

      * En Windows:
        ```bash
        OrbitDreams.exe
        ```
      * En Linux o macOS:
        ```bash
        ./OrbitDreams
        ```

-----

## 🎮 Cómo Usar el Programa

Una vez que el programa esté en ejecución, podrás navegar por los menús usando números.

  * **Acceso de Administrador**:

      * **Usuario**: `orbitdreams`
      * **Contraseña**: `dorito`

  * **Acceso de Cliente**:

      * Primero, el administrador debe crear una cuenta de cliente.
      * Luego, puedes iniciar sesión con las credenciales que hayas creado.
