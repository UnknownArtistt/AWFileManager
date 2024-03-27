#include <iostream>
#include <string>
#include <dirent.h>
#include <unistd.h>
#include <fstream>
#include <limits>
#include <string.h>

using namespace std;

/*
Esta funcion recibe como parametro el path del directorio y lista todos los archivos que contiene el directorio.
Recibe el path como string referenciandolo y especificandolo como constante para que no se pueda modificar.
*/
void listarArchivosEnDirectorio(const std::string& path) {
    DIR *dir;
    struct dirent *entry;

    // Si el path no es nulo va leyendo las entradas del contenido del path
    if ((dir = opendir(path.c_str())) != NULL) {
        std::cout << "Contenido de '" << path << "':" << std::endl; // cabecera del tree
        while ((entry = readdir(dir)) != NULL) { // mientras que haya archivos
            std::cout << "- " << entry->d_name << std::endl; // escribe el nombre del archivo/directorio
        }
        closedir(dir); // cierra el directorio
    } else { // handler del error
        std::cerr << "Error al abrir el directorio: " << strerror(errno) << std::endl;
    }
}

/*
Esta funcion crea un archivo en el directorio establecido. La funcion recibe dos referencias de parametros de tipo string
(uno para el directorio y otro para el contenido). 
*/
void escribirArchivo(const std::string& path) {
    std::ofstream file(path, std::ios::out); // crea un flujo de salida en modo escritura para escribir el contenido 
    if (file.is_open()) { // si el archivo se creado de manera correcta
        std::string contenido; // variable para el contenido 
        std::string linea; // variable para controlar las entradas por linea

        std::cout << "Ingresa el contenido para el archivo. Ingresa '#' en una línea nueva para finalizar:\n";

        while (true) {
            std::getline(std::cin, linea); // lee la linea
            if (linea == "#") { // si detecta un # en una nueva linea hace el break y sale de la entrada
                break;
            }
            contenido += linea + "\n"; // Agregar salto de línea al final de cada línea
        }

        file << contenido; // escribe el contenido en el archivo
        file.close(); // cierra el archivo
        std::cout << "Archivo '" << path << "' creado exitosamente." << std::endl;
    } else {
        std::cerr << "Error al crear el archivo: " << strerror(errno) << std::endl;
    }
}

/*
Esta funcion recibe como parametro una referencia constante del path del archivo que se desea eliminar.
Se verifica si la operacion de remove del path que se a especificado es correcta, si es correcta se elimina
el archivo y se notifica que se ha borrado exitosamente sino se indica que ha sucedido un error.
*/
void borrarArchivo(const std::string& path) {
    if (remove(path.c_str()) == 0) { // se verifica si se ha eliminado correctamente  
        std::cout << "Archivo '" << path << "' eliminado exitosamente." << std::endl; // se printea que se ha eliminado el archivo
    } else {
        std::cerr << "Error al eliminar el archivo: " << strerror(errno) << std::endl; // se printea el handler del error
    }
}

/*
Esta funcion recibe como parametros las referencias constantes del path del archivo que se quiere copiar y
el path de destino donde se quiere copiar el archivo. Se crean dos flujos, uno de entrada con ifstream (modo lectura) y otro de
salida (modo escritura) para copiar el contenido de uno a otro. Verifica si los dos archivos se han abierto de forma correcta, y si es asi,
copia el contenido de uno a otro con la funcion rdbuf(). Una vez hecho esto se cierran los archivos y se printea que se ha copiado
el archivo de forma correcta. En el caso opuesto se printea que ha habido un error.
*/
void copiarArchivo(const std::string& pathOrigen, const std::string& pathDestino) {
    std::ifstream fileOrigen(pathOrigen, std::ios::binary); // crea un flujo de entrada en modo lectura para copiar el contenido
    std::ofstream fileDestino(pathDestino, std::ios::binary); // crea un flujo de salida en modo escritura para escribir el contenido
    if (fileOrigen.is_open() && fileDestino.is_open()) { // verifica si se han abierto los archivos de forma correcta
        fileDestino << fileOrigen.rdbuf(); // copia el contenido del archivo de origen en el de destino
        fileOrigen.close(); // se cierra el archivo
        fileDestino.close(); // se cierra el archivo
        std::cout << "Archivo '" << pathOrigen << "' copiado a '" << pathDestino << "' exitosamente." << std::endl; // Printea que se ha realizado de forma exitosa la operacion
    } else {
        std::cerr << "Error al copiar el archivo: " << strerror(errno) << std::endl; // Handler del error
    }
}

/*
Esta funcion recibe como parametro el path del archivo que se quiere modificar. Es una referencia constante de la misma.
Primero crea un flujo de entrada en modo lectura para extraer el contenido existente del archivo. Despues verifica si
se ha abierto de forma correcta el archivo, si es asi se lee linea por linea el contenido y se guarda en la variable
contenidoExistente. Despues se pasa a pedir el nuevo contenido para sumarlo al archivo. Ira leyendo las entradas hasta que
se detecte un # en una nueva linea. Entonces se concatena el nuevo contenido con el existente y se escribe en el archivo.
*/
void escribirEnArchivo(const std::string& path) {
    std::ifstream file(path);
    std::string contenidoExistente;
    std::string linea;

    if (file.is_open()) {
        // Leer el contenido existente del archivo
        while (std::getline(file, linea)) {
            contenidoExistente += linea + "\n"; // Agregar un salto de linea después de cada linea
        }
        file.close();

        // Permitir al usuario ingresar el nuevo contenido
        std::string nuevoContenido;
        std::cout << "Ingresa el nuevo contenido para el archivo. Ingresa '#' en una linea nueva para finalizar:\n";

        while (true) {
            std::getline(std::cin, linea);
            if (linea == "#") {
                break;
            }
            nuevoContenido += linea + "\n"; // Agregar un salto de linea despues de cada linea
        }

        // Concatenar el nuevo contenido al contenido existente
        contenidoExistente += nuevoContenido;

        // Escribir el contenido modificado de vuelta en el archivo
        std::ofstream outFile(path, std::ios::trunc);
        if (outFile.is_open()) {
            outFile << contenidoExistente;
            outFile.close();
            std::cout << "Archivo '" << path << "' modificado exitosamente." << std::endl;
        } else {
            std::cerr << "Error al modificar el archivo: " << strerror(errno) << std::endl;
        }
    } else {
        std::cerr << "Error al abrir el archivo para modificar: " << strerror(errno) << std::endl;   
    }
}

int main() {

    std::string path; // path del directorio
    std::string contenido; // contenido del archivo en caso de querer crear uno y insertar algo
    int opcion; // opcion del menu
    bool repeat = true; // variable de control para el menu
    std::string pathDestino; // variable para la ruta de destino al copiar un archivo
    std::string nuevoContenido;  // variable para el contenido nuevo en el caso de querer escribir un archivo

    while (repeat) {

        /* Printeamos las opciones del menu */
        std::cout << "------------------------------------------------------------------------------------------------------------ \n";
        std::string asciiArt =

        "                       _____ .__ .__                                                                         \n"
        " _____   __  _  __   _/ ____\\|__||  |    ____       _____  _____     ____  _____      ____    ____  _______  \n"
        " \\__  \\  \\ \\/ \\/ /   \\   __\\ |  ||  |  _/ __ \\     /     \\ \\__  \\   /    \\ \\__  \\    / ___\\ _/ __ \\ \\_  __ \\ \n"
        "  / __ \\_ \\     /     |  |   |  ||  |__\\  ___/    |  Y Y  \\ / __ \\_|   |  \\ / __ \\_ / /_/  >\\  ___/  |  | \\/ \n"
        " (____  /  \\/\\_/      |__|   |__||____/ \\___  >   |__|_|  /(____  /|___|  /(____  / \\___  /  \\___  > |__|    \n"
        "      \\/                                    \\/          \\/      \\/      \\/      \\/ /_____/       \\/          \n\n";
        std::cout << asciiArt;
        std::cout << "Terminal Based File Manager                                                                  By AbyssWatcher\n";
        std::cout << "------------------------------------------------------------------------------------------------------------\n";
        std::cout << "\nMENU DEL EXPLORADOR DE ARCHIVOS:" << std::endl;
        std::cout << "------------------------------------\n";
        std::cout << "\n1. Listar archivos en un directorio" << std::endl;
        std::cout << "2. Escribir un archivo" << std::endl;
        std::cout << "3. Borrar un archivo" << std::endl;
        std::cout << "4. Copiar un archivo" << std::endl;
        std::cout << "5. Modificar el contenido de un archivo" << std::endl;
        std::cout << "6. Salir\n" << std::endl;
        std::cout << "-----------------------------------\n";
        std::cout << "Ingresa tu opcion: ";
        std::cin >> opcion; // leemos la opcion del usuario
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // limpiar el búfer de entrada después de leer un valor numérico con std::cin, ignore el contenido despues del salto de linea \n

        switch (opcion) {
            
            case 1:
                std::cout << "Ingresa la ruta del directorio: "; // pedimos el path al directorio deseado
                std::getline(std::cin, path); // mediante cin lo almacenamos en la variable path
                listarArchivosEnDirectorio(path); // llamamos a la funcion y pasamos el path como parametro
                break;

            case 2:
                std::cout << "Ingresa la ruta para el archivo: ";
                std::getline(std::cin, path); // IMPORTANTE mejor usar getline, cin solo lee hasta el primer espacio en blanco o salto de linea
                //std::cout << "Ingresa el contenido del archivo: ";
                //std::getline(std::cin, contenido); // almacenamos el contenido del archivo
                escribirArchivo(path); // llamamos a la funcion pasandole el path donde se quiere crear el archivo y el contenido
                break;

            case 3:
                std::cout << "Ingresa la ruta del archivo que deseas eliminar: ";
                std::getline(std::cin, path); // ruta del archivo que se desea eliminar
                borrarArchivo(path); // llamada a la funcion
                break;

            case 4:
                std::cout << "Ingresa la ruta del archivo que deseas copiar: ";
                std::getline(std::cin, path); // ruta del archivo que se quiere copiar
                std::cout << "Ingresa la ruta donde deseas copiar el archivo deseado: ";
                std::getline(std::cin, pathDestino); // ruta donde se quiere copiar el archivo
                copiarArchivo(path, pathDestino); // llamada a la funcion
                break;
            
            case 5:
                std::cout << "Ingresa la ruta del archivo donde quieres escribir: "; // ruta del archivo donde se desea escribir
                std::getline(std::cin, path);
                escribirEnArchivo(path); // llamada a la funcion
                break;

            case 6:
                std::cout << "Gracias por usar AWFileManager! Hasta luego!" << std::endl; // mensaje de adios
                repeat = false; // romper bucle while para acabar la ejecucion
                return 0;

            default:
                break;
            }
    }
}