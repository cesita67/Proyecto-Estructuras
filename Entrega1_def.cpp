//* Integrantes:
//* Luis Terrones. C.I.: 31.870.499
//* Jesús Villasmil. C.I.: 31.448.764
//* Diego Rojas. C.I.: 32.336.736

#include <iostream>
#include <stdlib.h>
#include <string>
#include <windows.h>
#include <iomanip>
using namespace std;

// Función para habilitar ANSI en Windows
void habilitarANSI() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;
    GetConsoleMode(hConsole, &mode);
    SetConsoleMode(hConsole, mode | 0x0004);
}

// Códigos ANSI para colores y estilos
#define RESET   "\033[0m"
#define NEGRITA "\033[1m"
#define ROJO    "\033[31m"
#define VERDE   "\033[32m"
#define AMARILLO "\033[33m"
#define AZUL    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define BLANCO  "\033[37m"
#define FONDO_ROJO "\033[41m"
#define FONDO_VERDE "\033[42m"
#define FONDO_AMARILLO "\033[43m"

//* Estructuras
struct Bitacora{ //! Bitacora = Vuelos registrados
    string ID;
    string fecha;
    float duracion;
    int distancia_recorrida;
    string objetivo;
    Bitacora *proxBitacora;
};

struct Dron{
    string modelo;
    string num_serie;
    string tipo;
    string estado;
    Bitacora *lista_bitacoras;
    Dron *proxDron;
};

//* Funcion para crear un dron y reservar memoria
Dron *Crear_dron(string modelo, string num_serie, string tipo, string estado){
    Dron *nuevo_dron = new Dron();
    nuevo_dron->modelo = modelo;
    nuevo_dron->num_serie = num_serie;
    nuevo_dron->tipo = tipo;
    nuevo_dron->estado = estado;
    nuevo_dron->proxDron = NULL;
    nuevo_dron->lista_bitacoras = NULL;
    
    return nuevo_dron;
}

//* Funcion para crear una bitacora y reservar memoria
Bitacora *Crear_bitacora(string ID, string fecha, float duracion, int distancia_recorrida, string objetivo){ 
    Bitacora *nueva_bitacora = new Bitacora();
    nueva_bitacora->ID = ID;
    nueva_bitacora->fecha = fecha;
    nueva_bitacora->duracion = duracion;
    nueva_bitacora->distancia_recorrida = distancia_recorrida;
    nueva_bitacora->objetivo = objetivo;
    nueva_bitacora->proxBitacora = NULL;

    return nueva_bitacora;
}

//* Funcion para verificar si la lista de drones está vacia o no
bool lista_vacia_drones(Dron *principal){
    return principal == NULL;
}

//* Función para calcular el total de duracion en minutos
float calcular_total_de_duracion_minutos(Dron *actual){ 
    if(actual == NULL){
        return 0.0;
    }
    Bitacora *actual_bitacora = actual->lista_bitacoras;
    if(actual_bitacora == NULL){
        return 0.0; 
    }
    float sumatoria_duraciones = 0.0; 
    while(actual_bitacora != NULL){
        sumatoria_duraciones += actual_bitacora->duracion;
        actual_bitacora = actual_bitacora->proxBitacora;
    }
    return sumatoria_duraciones;
}

//* Función para convertir minutos a horas
float convertir_minutos_a_horas(float min){ 
    float horas = 0.0; 
    horas = min/60.0;
    return horas;
}

//* Función para calcular el total de distancias recorridas de un dron
int calcular_total_de_distancias(Dron *actual){
    if(actual == NULL){
        return 0;
    }
    Bitacora *actual_bitacora = actual->lista_bitacoras;
    int sumatoria_distancias = 0;
    while(actual_bitacora != NULL){
        sumatoria_distancias += actual_bitacora->distancia_recorrida;
        actual_bitacora = actual_bitacora->proxBitacora;
    }
    return sumatoria_distancias;
}

//* Requerimiento 1) Procedimiento para agregar un dron en la lista principal
void agregar_dron_de_ultimo(Dron *&principal, string modelo, string num_serie, string tipo, string estado){
    Dron *nuevo_dron = Crear_dron(modelo, num_serie, tipo, estado), *actual = principal;
    if(lista_vacia_drones(principal)){
        principal = nuevo_dron;
        return;
    }
    while(actual->proxDron != NULL){
        actual = actual->proxDron;
    }
    actual->proxDron = nuevo_dron;
}

//* Función para retornar un puntero a un dron segun su número de serie
Dron *dron_por_num_de_serie(Dron *principal, string num_serie_buscado){
    if(lista_vacia_drones(principal)){
        cout << ROJO << NEGRITA << "No hay drones en la base de datos" << RESET << endl;
        return NULL;
    }
    Dron *actual = principal;
    while(actual != NULL && actual->num_serie != num_serie_buscado){
        actual = actual->proxDron;
    }

    return actual;
}

//* Requerimiento 2) Procedimiento para dar de baja un dron segun su número de serie
void dar_de_baja_dron_por_num_de_serie(Dron *&principal, string num_serie){
    if(lista_vacia_drones(principal)){
        cout << ROJO << NEGRITA << "No hay drones en la base de datos..." << RESET << endl;
        return;
    }
    Dron *prev = NULL, *actual = principal;
    Bitacora *elim_bitacoras = NULL, *aux = NULL;
    bool encontrado = false;
    
    while(actual != NULL && encontrado == false){
        if(actual->num_serie == num_serie){
            encontrado = true;
        }else{
            prev = actual;
            actual = actual->proxDron;
        }
    }
    if(encontrado == false){
        cout << ROJO << NEGRITA << "No existe un dron con numero de serie " << num_serie << "..." << RESET << endl;
        return;
    }
    
    elim_bitacoras = actual->lista_bitacoras; 
    aux = elim_bitacoras;
    if(elim_bitacoras != NULL){ //* Se comienza a eliminar las bitacoras
        while(elim_bitacoras != NULL){
            aux = aux->proxBitacora;
            delete elim_bitacoras;
            elim_bitacoras = aux;
        }
    }else{ //* Si el dron a eliminar no posee algun registro de bitácoras
        cout << AMARILLO << "Dron a eliminar sin bitacoras/vuelos registrados" << RESET << endl;
    }
    
    //* Se comienza a eliminar el dron como tal
    if(principal == actual){
        principal = principal->proxDron;
        delete actual;
    }
    else{ //* Caso contrario el dron no esta al inicio
        prev->proxDron = actual->proxDron;
        delete actual;
    }
    cout << VERDE << NEGRITA << "OK Dron eliminado exitosamente" << RESET << endl;
}

//* Requerimiento 3) Procedimiento para digitar todos todos los drones de la flota con su respectiva informacion
void listar_drones(Dron *principal){
    if(lista_vacia_drones(principal)){
        cout << ROJO << NEGRITA << "No hay drones en la base de datos..." << RESET << endl;
        return;
    }
    Dron *actual = principal;
    int i = 1;
    while(actual != NULL){
        cout << CYAN << NEGRITA << "==========================" << RESET << endl;
        cout << CYAN << NEGRITA << "| Datos del Dron " << i << RESET << endl;
        cout << CYAN << "| Modelo: " << actual->modelo << RESET << endl;
        cout << CYAN << "| Numero de serie: " << actual->num_serie<< RESET << endl;
        cout << CYAN << "| Tipo de Dron: " << actual->tipo << RESET << endl;
        if(actual->estado == "Operativo" || actual->estado == "operativo"){
            cout<<VERDE<<"| Estado: "<<actual->estado<<RESET<<endl;
        }else{
            cout<<ROJO<<"| Estado "<<actual->estado<<RESET<<endl;
        }
        actual = actual->proxDron;
        i++;
    }
    cout << CYAN << NEGRITA << "==========================" << RESET << endl;
}

//* Requerimiento 4) Buscamos un dron por su numero de serie
void buscar_dron_por_num_de_serie(Dron *principal, string num_serie_buscado){
    if(lista_vacia_drones(principal)){
        cout << ROJO << NEGRITA << "No hay drones en la base de datos" << RESET << endl;
        return;
    }
    Dron *actual = principal;
    int i = 1;
    while(actual != NULL && actual->num_serie != num_serie_buscado){
        actual = actual->proxDron;
        i++;
    }
    if(actual == NULL){
        cout << ROJO << NEGRITA << "No se encontro el dron con numero de serie " << num_serie_buscado << " en la base de datos" << RESET << endl;
        return;
    }
    cout << VERDE << NEGRITA << "OK Dron exitosamente encontrado en la posicion " << i << RESET << endl;
    cout << AZUL << NEGRITA << "====================" << RESET << endl;
    cout << AZUL << NEGRITA << "| Datos del Dron " << i << " |" << RESET << endl;
    cout << AZUL << "| Modelo: " << actual->modelo << " |" << RESET << endl;
    cout << AZUL << "| Numero de serie: " << actual->num_serie << " |" << RESET << endl;
    cout << AZUL << "| Tipo de Dron: " << actual->tipo << " |" << RESET << endl;
    cout << (actual->estado == "Operativo" ? VERDE : ROJO) << "| Estado: " << actual->estado << " |" << RESET << endl;
    cout << AZUL << "====================" << RESET << endl;
    
    Bitacora *actual_bitacora = actual->lista_bitacoras;
    int pos_bitacora = 1;
    if(actual_bitacora == NULL){
        cout << AMARILLO << "El dron no posee bitacoras registradas" << RESET << endl;
        return;
    }
    cout << CYAN << NEGRITA << "Digitando bitacoras del dron registrado" << RESET << endl;
    system("pause");
    cout << endl;
    while(actual_bitacora != NULL){
        cout << VERDE << "--> Bitacora " << pos_bitacora << RESET << endl;
        cout << "  ID: " << actual_bitacora->ID << endl;
        cout << "  Fecha del vuelo: " << actual_bitacora->fecha << endl;
        cout << "  Duracion: " << actual_bitacora->duracion << " minutos" << endl;
        cout << "  Distancia recorrida: " << actual_bitacora->distancia_recorrida << " km" << endl;
        cout << "  Objetivo: " << actual_bitacora->objetivo << endl;
        cout << VERDE << "----------------------------------------" << RESET << endl;
        actual_bitacora = actual_bitacora->proxBitacora;
        cout << endl;
        pos_bitacora++;
    }
}

//* Funcion para contar la cantidad de drones en la lista
int cantidad_drones(Dron *principal){
    if(lista_vacia_drones(principal)){
        return 0;
    }
    Dron *actual_dron = principal;
    int cantidad = 0;
    while(actual_dron != NULL){
        cantidad++;
        actual_dron = actual_dron->proxDron;
    }
    return cantidad;
}

//* Funcion para verificar si un dron existe en la lista por su numero de serie 
bool esta_dron(Dron *principal, string num_serie_buscado){
    if(lista_vacia_drones(principal)){
        return false;
    }
    Dron *actual_dron = principal;
    while(actual_dron != NULL){
        if(actual_dron->num_serie == num_serie_buscado){
            return true;
        }
        actual_dron = actual_dron->proxDron;
    }
    return false;
}

void insertar_dron_ordenadamente(Dron *&principal_de_ordenados, Dron *&dron_a_insertar){
    if(lista_vacia_drones(principal_de_ordenados)){
        principal_de_ordenados = dron_a_insertar;
        return;
    }
    if(dron_a_insertar->modelo < principal_de_ordenados->modelo){
        dron_a_insertar->proxDron = principal_de_ordenados;
        principal_de_ordenados = dron_a_insertar;
        return;
    }
    Dron *actual_dron = principal_de_ordenados;
    while(actual_dron->proxDron != NULL && actual_dron->proxDron->modelo < dron_a_insertar->modelo){
        actual_dron = actual_dron->proxDron;
    }
    dron_a_insertar->proxDron = actual_dron->proxDron;
    actual_dron->proxDron = dron_a_insertar;
}

//* Requerimiento 5) Ordenar los drones alfabeticamente por modelo
void ordenar_drones_alfabeticamente(Dron *&principal){
    if(lista_vacia_drones(principal)){
        cout << ROJO << NEGRITA << "No hay drones en la base de datos" << RESET << endl;
        return;
    }
    Dron *lista_drones_ordenados = NULL;
    while(principal != NULL){
        Dron *sig = principal->proxDron; //* Guarda un puntero que apunte al siguiente de actual
        principal->proxDron = NULL; //* Se desconecta el nodo
        insertar_dron_ordenadamente(lista_drones_ordenados, principal);
        principal = sig;
    }
    principal = lista_drones_ordenados;

    cout << VERDE << NEGRITA << "OK Los drones han sido ordenados segun su modelo exitosamente" << RESET << endl;
}

//* Requerimiento 6) Top 3 drones por hora de vuelo
void mostrar_top_3_drones_por_hora_de_vuelo(Dron *principal){
    if(lista_vacia_drones(principal)){
        cout << ROJO << NEGRITA << "No hay drones en la base de datos" << RESET << endl;
        return;
    }
    if(cantidad_drones(principal) < 3){
        cout << AMARILLO << NEGRITA << "ATENCION: Imposible realizar la solicitud, existen menos de 3 drones" << RESET << endl;
        return;
    }
    Dron *Top1 = NULL, *Top2 = NULL, *Top3 = NULL;
    int pos = 1; //* Este contador aumentara cada vez que encuentre un dron con mayor horas de vuelo

    while(pos <= 3){
        Dron *actual_dron = principal;
        Dron *dron_a_imprimir = NULL;
        float tiempo_de_vuelo_mayor = -1.0;

        while(actual_dron != NULL){
            float tiempo_de_vuelo_actual = calcular_total_de_duracion_minutos(actual_dron);
            tiempo_de_vuelo_actual = convertir_minutos_a_horas(tiempo_de_vuelo_actual);
            if(actual_dron != Top1 && actual_dron != Top2 && tiempo_de_vuelo_actual > tiempo_de_vuelo_mayor){
                tiempo_de_vuelo_mayor = tiempo_de_vuelo_actual;
                dron_a_imprimir = actual_dron;
            }
            actual_dron = actual_dron->proxDron;
        }
 
        if(dron_a_imprimir == NULL || tiempo_de_vuelo_mayor <= 0.0){
            if(pos == 1){
                cout << AMARILLO << "Ningun dron tiene vuelos registrados." << RESET << endl;
            }
            break;
        }

        cout << FONDO_AMARILLO << BLANCO << NEGRITA << "TOP " << pos << RESET << endl;
        cout << VERDE << "- Modelo: " << dron_a_imprimir->modelo << RESET << endl;
        cout << VERDE << "- Numero de serie: " << dron_a_imprimir->num_serie << RESET << endl;
        cout << VERDE << "- Tipo de dron: " << dron_a_imprimir->tipo << RESET << endl;
        cout << VERDE << "- Estado: " << dron_a_imprimir->estado << RESET << endl;
        cout << VERDE << "- Horas de vuelo: " <<fixed<<setprecision(2)<< tiempo_de_vuelo_mayor << " horas" << RESET << endl;
        cout << endl;

        if(pos == 1){
            Top1 = dron_a_imprimir;
        }
        else if(pos == 2){
            Top2 = dron_a_imprimir;
        }
        else if(pos == 3){
            Top3 = dron_a_imprimir;
        }
        pos++;
    }
}

//* Requerimiento 7) Reporte de Mantenimiento
void reporte_de_mantenimiento(Dron *principal, float horas_de_vuelo){
    if(lista_vacia_drones(principal)){
        cout << ROJO << NEGRITA << "No hay drones en la base de datos" << RESET << endl;
        return;
    }
    Dron *actual_dron = principal;
    float duraciones_de_dron = 0.0; 
    int pos_dron = 1;
    bool se_requiere_mantenimiento = false;
    while(actual_dron != NULL){
        duraciones_de_dron = calcular_total_de_duracion_minutos(actual_dron);
        duraciones_de_dron = convertir_minutos_a_horas(duraciones_de_dron);
        if((actual_dron->estado == "Operativo" || actual_dron->estado == "operativo") && duraciones_de_dron > horas_de_vuelo){
            se_requiere_mantenimiento = true;
            cout << ROJO << NEGRITA << "ATENCION: Se sugiere hacer mantenimiento al dron numero " << pos_dron << RESET << endl;
            cout << "   Modelo: " << actual_dron->modelo << " | Horas de vuelo: " << duraciones_de_dron << endl;
        }
        pos_dron++;
        actual_dron = actual_dron->proxDron;
    }
    if(se_requiere_mantenimiento == false){
        cout << VERDE << NEGRITA << "OK No se requiere mantenimiento a ningun un dron" << RESET << endl;
    }
}

//* Requerimiento 8) Agregar una nueva bitácora de vuelo a un dron específico por su numero de serie
void agregar_bitacora_de_ultimo(Dron *&principal, string num_serie, string ID, string fecha, float duracion, int distancia_recorrida, string objetivo){ 
    if(lista_vacia_drones(principal)){
        cout << ROJO << NEGRITA << "No hay drones en la base de datos" << RESET << endl;
        return;
    }
    //* Primero buscamos el dron segun su numero de serie para despues para agregar la bitacora 
    Dron *dron_para_agregar_bitacora = dron_por_num_de_serie(principal, num_serie);
    if(dron_para_agregar_bitacora == NULL){
        cout << ROJO << NEGRITA << "No se pudo agregar la bitacora (registro de vuelo) porque no existe un dron con numero de serie " << num_serie << " en la base de datos" << RESET << endl;
        return;
    }

    Bitacora *nueva_bitacora = Crear_bitacora(ID, fecha, duracion, distancia_recorrida, objetivo);
    if(dron_para_agregar_bitacora->lista_bitacoras == NULL){
        dron_para_agregar_bitacora->lista_bitacoras = nueva_bitacora;
    } else {
        //* Caso contrario: El dron ya tiene bitacoras, comenzamos a recorrer la sublista bitacoras para agregar de ultimo la nueva bitacora creada recientemente
        Bitacora *actual_bitacora = dron_para_agregar_bitacora->lista_bitacoras;
        while(actual_bitacora->proxBitacora != NULL){
            actual_bitacora = actual_bitacora->proxBitacora;
        }
        actual_bitacora->proxBitacora = nueva_bitacora;
    }
    cout << VERDE << NEGRITA << "OK Bitacora agregada exitosamente al dron " << num_serie << RESET << endl;
}

//* Requerimiento 9) Eliminar vuelo/ bitacora segun su ID
void eliminar_vuelo(Dron *&principal, string num_serie, string ID){
    if(lista_vacia_drones(principal)){
        cout << ROJO << NEGRITA << "No hay drones en la base de datos..." << RESET << endl;
        return;
    }
    Dron *dron_a_eliminar_su_bitacora = dron_por_num_de_serie(principal, num_serie);
    if(dron_a_eliminar_su_bitacora == NULL){
        cout << ROJO << NEGRITA << "No se encontro el dron a eliminar su vuelo o bitacora" << RESET << endl;
        return;
    }
    Bitacora *actual_bitacora = dron_a_eliminar_su_bitacora->lista_bitacoras, *prev_bitacora = NULL;
    bool encontrado = false;
    while(actual_bitacora != NULL && encontrado == false){
        if(actual_bitacora->ID == ID){
            encontrado = true;
        }
        else{
            prev_bitacora = actual_bitacora;
            actual_bitacora = actual_bitacora->proxBitacora;
        }
    }
    if(encontrado == false){
        cout << ROJO << NEGRITA << "No se encontro la bitacora/registro de vuelo con ID " << ID << " en el dron con numero de serie " << num_serie << RESET << endl;
        return;
    }

    if(actual_bitacora == dron_a_eliminar_su_bitacora->lista_bitacoras){
        dron_a_eliminar_su_bitacora->lista_bitacoras = dron_a_eliminar_su_bitacora->lista_bitacoras->proxBitacora;
        delete actual_bitacora;
    }else{
        prev_bitacora->proxBitacora = actual_bitacora->proxBitacora;
        delete actual_bitacora;
    }
    cout << VERDE << NEGRITA << "OK Bitacora/registro de vuelo con ID " << ID << " eliminada exitosamente del dron con numero de serie " << num_serie << RESET << endl;
}

//* Requerimiento 10) Consultar vuelo mas largo
void mostrar_vuelo_mas_largo(Dron *&principal, string num_serie){
    if(lista_vacia_drones(principal)){
        cout << ROJO << NEGRITA << "No hay drones en la base de datos..." << RESET << endl;
        return;
    }
    Dron *dron_buscado = dron_por_num_de_serie(principal, num_serie);
    if(dron_buscado == NULL){
        cout << ROJO << NEGRITA << "No se encontro el dron solicitado en la base de datos" << RESET << endl;
        return;
    }
    Bitacora *actual_bitacora = dron_buscado->lista_bitacoras, *bitacora_a_imprimir = NULL; //* Primero creamos un puntero bitacora auxiliar para recorrer toda la lista de bitacoras y determinar cual es la bitacora con mayor duracion
    float duracion_mayor = 0.0;
    int pos_bitacora = 1, pos_bitacora_buscada = 1;
    while(actual_bitacora != NULL){
        if(actual_bitacora->duracion >= duracion_mayor){
            duracion_mayor = actual_bitacora->duracion;
            bitacora_a_imprimir = actual_bitacora; //* bitacora_a_imprimir apunta a la bitacora actual
            pos_bitacora_buscada = pos_bitacora;
        }
        actual_bitacora = actual_bitacora->proxBitacora;
        pos_bitacora++; 
    } 
    if(duracion_mayor <= 0.0){
        cout << AMARILLO << "Ninguna bitacora tiene duracion alguna..." << RESET << endl;
        return;
    }
    cout << VERDE << NEGRITA << "VUELO MAS LARGO - Bitacora " << pos_bitacora_buscada << RESET << endl;
    cout << CYAN << "- ID del vuelo: " << bitacora_a_imprimir->ID << RESET << endl;
    cout << CYAN << "- Fecha del vuelo: " << bitacora_a_imprimir->fecha << RESET << endl;
    cout << CYAN << "- Duracion: " << bitacora_a_imprimir->duracion << " minutos" << RESET << endl; // Duracion ahora es float
    cout << CYAN << "- Distancia recorrida: " << bitacora_a_imprimir->distancia_recorrida << " km" << RESET << endl;
    cout << CYAN << "- Objetivo: " << bitacora_a_imprimir->objetivo << RESET << endl;
}

//* Requerimiento 11) Buscar si en un dron dado posee una bitacora/vuelo registrado con un objetivo en especifico
void buscar_si_existe_bitacora_con_un_objetivo(Dron *principal, string num_serie, string objetivo_buscado){
    if(lista_vacia_drones(principal)){
        cout << ROJO << NEGRITA << "No hay drones en la base de datos" << RESET << endl;
        return;
    }
    Dron *dron_buscado = dron_por_num_de_serie(principal, num_serie);
    if(dron_buscado == NULL){
        cout << ROJO << NEGRITA << "No se encontro el dron solicitado en la base de datos" << RESET << endl;
        return;
    }
    Bitacora *actual_bitacora = dron_buscado->lista_bitacoras;
    bool encontrado = false;
    int pos_bitacora_buscada = 1;
    while(actual_bitacora != NULL && encontrado == false){
        if(actual_bitacora->objetivo == objetivo_buscado){
            encontrado = true;
        }else{
            actual_bitacora = actual_bitacora->proxBitacora;
            pos_bitacora_buscada++;
        }
    }
    if(encontrado == false){
        cout << ROJO << "No se encontro un vuelo registrado/bitacora con el objetivo (" << objetivo_buscado << ")" << RESET << endl;
    }else{
        cout << VERDE << NEGRITA << "OK Se ha encontrado un vuelo registrado/bitacora con objetivo (" << objetivo_buscado << ") en la posicion " << pos_bitacora_buscada << RESET << endl;
    }
}

//* Requerimiento 12) Calcular totales de un dron en especifico
void calcular_totales_de_un_dron(Dron *&principal, string num_serie){
    if(lista_vacia_drones(principal)){
        cout << ROJO << NEGRITA << "No hay drones en la base de datos..." << RESET << endl;
        return;
    }
    Dron *dron_buscado = dron_por_num_de_serie(principal, num_serie);
    if(dron_buscado == NULL){
        cout << ROJO << NEGRITA << "No se encontro el dron solicitado en la base de datos" << RESET << endl;
        return;
    }
    //* Se calcula la suma de distancias y la suma de duraciones con las funciones correspondientes
    int suma_de_distancias = calcular_total_de_distancias(dron_buscado);
    float suma_de_duraciones = calcular_total_de_duracion_minutos(dron_buscado); 
    cout << AZUL << NEGRITA << "================================" << RESET << endl;
    cout << AZUL << NEGRITA << "|   ESTADISTICAS DEL DRON   |" << RESET << endl;
    cout << AZUL << NEGRITA << "================================" << RESET << endl;
    cout << AZUL << "| Modelo: " << dron_buscado->modelo << string(15 - dron_buscado->modelo.length(), ' ') << "|" << RESET << endl;
    cout << AZUL << "| Tiempo total de vuelo: " << suma_de_duraciones << " min" << "|" << RESET << endl; 
    cout << AZUL << "| Distancia total: " << suma_de_distancias << " km" << string(7 - to_string(suma_de_distancias).length(), ' ') << "|" << RESET << endl;
    cout << AZUL << NEGRITA << "================================" << RESET << endl;
}

//* Procedimientro para llenar datos de "n" drones
void llenar_datos_dron(Dron *&principal){
    int i = 0, cantidad = 0;
    string modelo, num_serie, tipo, estado;
    cout << CYAN << "Digite la cantidad de drones: " << RESET;
    cin>>cantidad;
    cin.ignore();
    while(i != cantidad){
        cout << VERDE << "\n-- Datos del dron " << i+1 << RESET << endl;
        cout << "| Modelo: ";
        getline(cin, modelo);
        cout << "| Numero de serie: ";
        getline(cin, num_serie);
        cout << "| Tipo de dron: ";
        getline(cin, tipo);
        cout << "| Estado: ";
        getline(cin, estado);
        cout << VERDE << "----------------------------------------" << RESET << endl;
        agregar_dron_de_ultimo(principal, modelo, num_serie, tipo, estado);
        i++;
    }
    cout << VERDE << NEGRITA << "OK Drones agregados exitosamente" << RESET << endl;
}

int main(){
    habilitarANSI(); // Habilitar códigos ANSI
    
    Dron *principal = NULL; 
    string num_serie_dron;
    string ID, fecha, objetivo;
    float duracion = 0.0;
    int distancia_recorrida = 0;
    int opc = -1;
    float num_horas = 0.0;
    bool opcion_valida = false;
    
    system("start musica_ascensor.wav");
    
    do{
        cout << MAGENTA << NEGRITA << "========================================" << RESET << endl;
        cout << MAGENTA << NEGRITA << "|     SISTEMA DE GESTION DE DRONES    |" << RESET << endl;
        cout << MAGENTA << NEGRITA << "========================================" << RESET << endl;
        cout << CYAN << "1" << RESET << "- Agregar dron/es (al final del registro)" << endl;
        cout << CYAN << "2" << RESET << "- Dar de baja un dron " << endl;
        cout << CYAN << "3" << RESET << "- Listar drones" << endl;
        cout << CYAN << "4" << RESET << "- Buscar un dron " << endl;
        cout << CYAN << "5" << RESET << "- Ordenar drones" << endl;
        cout << CYAN << "6" << RESET << "- Mostrar top 3 drones por horas de vuelo" << endl;
        cout << CYAN << "7" << RESET << "- Mostrar reporte de mantenimiento" << endl;
        cout << CYAN << "8" << RESET << "- Registrar vuelo/bitacora" << endl;
        cout << CYAN << "9" << RESET << "- Eliminar vuelo registrado/bitacora" << endl;
        cout << CYAN << "10" << RESET << "- Consultar vuelo registrado/bitacora con mayor duracion" << endl;
        cout << CYAN << "11" << RESET << "- Buscar vuelo/bitacora por objetivo" << endl;
        cout << CYAN << "12" << RESET << "- Calcular el tiempo y distancia total de un dron" << endl;
        cout << ROJO << "0" << RESET << "- Salir" << endl;
        
        while(opcion_valida == false){
            cout << AMARILLO << NEGRITA << "\n>> Digite una opcion: " << RESET;
            if(!(cin>>opc)){
                cout << ROJO << "ERROR: Debe digitar un numero de 0-12" << RESET << endl;
                cin.clear();
                cin.ignore(1000, '\n');
            }
            else{
                opcion_valida = true;
            }
        }
        opcion_valida = false;
        
        if(opc == 1){
            llenar_datos_dron(principal);
            system("pause"); system("cls");
        }
        else if(opc == 2){
            cin.ignore();
            cout << AMARILLO << "Digite el numero de serie del dron que desea dar de baja: " << RESET;
            getline(cin, num_serie_dron); cout<<endl;
            dar_de_baja_dron_por_num_de_serie(principal, num_serie_dron);
            system("pause"); system("cls");
        }
        else if(opc == 3){
            listar_drones(principal);
            system("pause"); system("cls");
        }
        else if(opc == 4){
            cin.ignore();
            cout << AMARILLO << "Digite el numero de serie del dron a buscar: " << RESET;
            getline(cin, num_serie_dron);
            buscar_dron_por_num_de_serie(principal, num_serie_dron);
            system("pause"); system("cls");
        }
        else if(opc == 5){
            ordenar_drones_alfabeticamente(principal);
            system("pause"); system("cls");
        }
        else if(opc == 6){
            mostrar_top_3_drones_por_hora_de_vuelo(principal);
            system("pause"); system("cls");
        }
        else if(opc == 7){
            cout << AMARILLO << "Digite las horas de vuelo a considerar: " << RESET;
            if(!(cin>>num_horas)){
                cout << ROJO << "ERROR: Entrada no válida para horas." << RESET << endl;
                num_horas = 0.0;
                cin.clear();
                cin.ignore(1000, '\n');
            }
            cout<<endl;
            reporte_de_mantenimiento(principal, num_horas);
            system("pause"); system("cls");
        }
        else if(opc == 8){
            cin.ignore();
            cout << AMARILLO << "Digite el numero de serie del dron a agregar la bitacora: " << RESET;
            getline(cin, num_serie_dron); cout<<endl;
            cout<<"Presione una tecla para continuar..."<<endl;
            if(esta_dron(principal, num_serie_dron) == false){
                cout << ROJO << "ERROR: No se encontro el dron con numero de serie: " << num_serie_dron << RESET << endl;
            }else{
                cin.ignore();
                cout << VERDE << "-- Datos de la bitacora --" << RESET << endl;
                cout << "| ID de vuelo: ";
                getline(cin, ID);
                cout << "| Fecha del vuelo: ";
                getline(cin, fecha);
                while(opcion_valida == false){
                    cout<< "| Duracion (minutos): ";
                    if(!(cin>>duracion) || duracion < 0.0){ //* Validacion de la duracion 
                        cout << ROJO << "ERROR: Debe digitar un numero mayor o igual a 0" << RESET << endl;
                        cin.clear();
                        cin.ignore(1000, '\n');
                    }
                    else{
                        opcion_valida = true;
                    }
                }
                opcion_valida = false;
                while(opcion_valida == false){
                    cout<< "| Distancia recorrida (km): ";
                    if(!(cin>>distancia_recorrida) || distancia_recorrida < 0){  //* Validacion de la distancia
                        cout << ROJO << "ERROR: Debe digitar un numero mayor o igual a 0" << RESET << endl;
                        cin.clear();
                        cin.ignore(1000, '\n');
                    }
                    else{
                        opcion_valida = true;
                    }
                }
                opcion_valida = false;
                cin.ignore();
                cout << "| Objetivo: ";
                getline(cin, objetivo);
                cout << VERDE << "----------------------------------------" << RESET << endl;
                agregar_bitacora_de_ultimo(principal, num_serie_dron, ID, fecha, duracion, distancia_recorrida, objetivo);
            }
            system("pause"); system("cls");
        }
        else if(opc == 9){
            cin.ignore();
            cout << AMARILLO << "Digite el numero de serie del dron: " << RESET;
            getline(cin, num_serie_dron); cout<<endl;
            if(esta_dron(principal, num_serie_dron) == false){
                cout << ROJO << "ERROR: No se encontro el dron con numero de serie: " << num_serie_dron << RESET << endl;
            }
            else{
                cout << AMARILLO << "Digite el ID de la bitacora a eliminar: " << RESET;
                getline(cin, ID);
                eliminar_vuelo(principal, num_serie_dron, ID);
            }
            system("pause"); system("cls");
        }
        else if(opc == 10){
            cin.ignore();
            cout << AMARILLO << "Digite el numero de serie del dron: " << RESET;
            getline(cin, num_serie_dron); cout<<endl;
            mostrar_vuelo_mas_largo(principal, num_serie_dron);
            system("pause"); system("cls");
        }
        else if(opc == 11){
            cin.ignore();
            cout << AMARILLO << "Digite el numero de serie del dron: " << RESET;
            getline(cin, num_serie_dron); cout<<endl;
            cout << AMARILLO << "Digite el objetivo a buscar: " << RESET;
            getline(cin, objetivo); cout<<endl;
            buscar_si_existe_bitacora_con_un_objetivo(principal, num_serie_dron, objetivo);
            system("pause"); system("cls");
        }
        else if(opc == 12){
            cin.ignore();
            cout << AMARILLO << "Digite el numero de serie del dron: " << RESET;
            getline(cin, num_serie_dron); cout<<endl;
            calcular_totales_de_un_dron(principal, num_serie_dron);
            system("pause"); system("cls");
        }
        else if(opc == 0){
            cout << MAGENTA << NEGRITA << "¡Gracias por usar el sistema! Hasta luego..." << RESET << endl;
            system("pause"); system("cls");
        }else{
            cout << ROJO << "ERROR: Opcion incorrecta" << RESET << endl;
            system("pause"); system("cls");
        }
        cout<<endl;
    }while(opc != 0);

    //* Liberar memoria
    Dron *elim = principal;
    if(principal != NULL){
        while(elim != NULL){
            Bitacora *elim_bitacora = elim->lista_bitacoras;
            while(elim_bitacora != NULL){
                Bitacora *next_bit = elim_bitacora->proxBitacora;
                delete elim_bitacora;
                elim_bitacora = next_bit;
            }
            Dron *next_dron = elim->proxDron;
            delete elim;
            elim = next_dron;
        }
        principal = NULL;
        cout << VERDE << "OK Memoria liberada correctamente..." << RESET << endl;
    }

    system("pause");
    return 0;
}