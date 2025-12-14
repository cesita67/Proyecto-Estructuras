//* Integrantes:
//* Luis Terrones. C.I.: 31.870.499
//* Jesús Villasmil. C.I.: 31.448.764
//* Diego Rojas. C.I.: 32.336.736

#include <iostream>
#include <stdlib.h>
#include <string>
#include <windows.h>
#include <iomanip>
#include <fstream>
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
    if(esta_dron(principal, num_serie)){
        cout<<ROJO<<NEGRITA<<"ERROR. El n"<<char(163)<<"mero de serie ya se encuentra registrado en otro dron"<< RESET<<endl;
        return;
    }
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
        cout << ROJO << NEGRITA << "No existe un dron con n"<<char(163)<<"mero de serie " << num_serie << "..." << RESET << endl;
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
        cout << AMARILLO << "Dron a eliminar sin bit"<<char(160)<<"coras/vuelos registrados" << RESET << endl;
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
        cout << ROJO << NEGRITA << "No se encontr"<<char(162)<<" el dron con numero de serie " << num_serie_buscado << " en la base de datos" << RESET << endl;
        return;
    }
    cout << VERDE << NEGRITA << "OK Dron exitosamente encontrado en la posici"<<char(162)<<"n " << i << RESET << endl;
    cout << AZUL << NEGRITA << "====================" << RESET << endl;
    cout << AZUL << NEGRITA << "| Datos del Dron " << i << " |" << RESET << endl;
    cout << AZUL << "| Modelo: " << actual->modelo << " |" << RESET << endl;
    cout << AZUL << "| N"<<char(163)<<"mero de serie: " << actual->num_serie << " |" << RESET << endl;
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
        cout << VERDE << "--> Bit"<<char(160)<<"cora " << pos_bitacora << RESET << endl;
        cout << "  ID: " << actual_bitacora->ID << endl;
        cout << "  Fecha del vuelo: " << actual_bitacora->fecha << endl;
        cout << "  Duraci"<<char(162)<<"n: " << actual_bitacora->duracion << " minutos" << endl;
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
    Dron *Top1 = NULL, *Top2 = NULL;
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
        cout << VERDE << "- N"<<char(163)<<"mero de serie: " << dron_a_imprimir->num_serie << RESET << endl;
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
            cout << ROJO << NEGRITA << "ATENCI"<<char(224)<<"N: Se sugiere hacer mantenimiento al dron numero " << pos_dron << RESET << endl;
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
        cout << ROJO << NEGRITA << "No se pudo agregar la bit"<<char(160)<<"cora (registro de vuelo) porque no existe un dron con numero de serie " << num_serie << " en la base de datos" << RESET << endl;
        return;
    }

    Bitacora *nueva_bitacora = Crear_bitacora(ID, fecha, duracion, distancia_recorrida, objetivo);
    if(dron_para_agregar_bitacora->lista_bitacoras == NULL){
        dron_para_agregar_bitacora->lista_bitacoras = nueva_bitacora;
        return;
    }
    //* Caso contrario: El dron ya tiene bitacoras, comenzamos a recorrer la sublista bitacoras para agregar de ultimo la nueva bitacora creada recientemente
    Bitacora *actual_bitacora = dron_para_agregar_bitacora->lista_bitacoras;
    while(actual_bitacora->proxBitacora != NULL){
        actual_bitacora = actual_bitacora->proxBitacora;
    }
    actual_bitacora->proxBitacora = nueva_bitacora;
    cout << VERDE << NEGRITA << "OK Bit"<<char(160)<<"cora agregada exitosamente al dron " << num_serie << RESET << endl;
}

//* Requerimiento 9) Eliminar vuelo/ bitacora segun su ID
void eliminar_vuelo(Dron *&principal, string num_serie, string ID){
    if(lista_vacia_drones(principal)){
        cout << ROJO << NEGRITA << "No hay drones en la base de datos..." << RESET << endl;
        return;
    }
    Dron *dron_a_eliminar_su_bitacora = dron_por_num_de_serie(principal, num_serie);
    if(dron_a_eliminar_su_bitacora == NULL){
        cout << ROJO << NEGRITA << "No se encontr"<<char(162)<<" el dron a eliminar su vuelo o bit"<<char(160)<<"cora" << RESET << endl;
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
        cout << ROJO << NEGRITA << "No se encontr"<<char(162)<<" la bit"<<char(160)<<"cora/registro de vuelo con ID " << ID << " en el dron con numero de serie " << num_serie << RESET << endl;
        return;
    }

    if(actual_bitacora == dron_a_eliminar_su_bitacora->lista_bitacoras){
        dron_a_eliminar_su_bitacora->lista_bitacoras = dron_a_eliminar_su_bitacora->lista_bitacoras->proxBitacora;
        delete actual_bitacora;
    }else{
        prev_bitacora->proxBitacora = actual_bitacora->proxBitacora;
        delete actual_bitacora;
    }
    cout << VERDE << NEGRITA << "OK Bitacora/registro de vuelo con ID " << ID << " eliminada exitosamente del dron con n"<<char(163)<<"mero de serie " << num_serie << RESET << endl;
}

//* Requerimiento 10) Consultar vuelo mas largo
void mostrar_vuelo_mas_largo(Dron *&principal, string num_serie){
    if(lista_vacia_drones(principal)){
        cout << ROJO << NEGRITA << "No hay drones en la base de datos..." << RESET << endl;
        return;
    }
    Dron *dron_buscado = dron_por_num_de_serie(principal, num_serie);
    if(dron_buscado == NULL){
        cout << ROJO << NEGRITA << "No se encontr"<<char(162)<<" el dron solicitado en la base de datos" << RESET << endl;
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
        cout << AMARILLO << "Ninguna bit"<<char(160)<<"cora tiene duraci"<<char(162)<<"n alguna..." << RESET << endl;
        return;
    }
    cout << VERDE << NEGRITA << "VUELO MAS LARGO - Bit"<<char(160)<<"cora " << pos_bitacora_buscada << RESET << endl;
    cout << CYAN << "- ID del vuelo: " << bitacora_a_imprimir->ID << RESET << endl;
    cout << CYAN << "- Fecha del vuelo: " << bitacora_a_imprimir->fecha << RESET << endl;
    cout << CYAN << "- Duraci"<<char(162)<<"n: " << bitacora_a_imprimir->duracion << " minutos" << RESET << endl; // Duracion ahora es float
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
        cout << ROJO << NEGRITA << "No se encontr"<<char(162)<<" el dron solicitado en la base de datos" << RESET << endl;
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
        cout << ROJO << "No se encontr"<<char(162)<<" un vuelo registrado/bitacora con el objetivo (" << objetivo_buscado << ")" << RESET << endl;
    }else{
        cout << VERDE << NEGRITA << "OK se ha encontrado un vuelo registrado/bitacora con objetivo (" << objetivo_buscado << ") en la posicion " << pos_bitacora_buscada << RESET << endl;
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
        cout << ROJO << NEGRITA << "No se encontr"<<char(162)<<" el dron solicitado en la base de datos" << RESET << endl;
        return;
    }
    //* Se calcula la suma de distancias y la suma de duraciones con las funciones correspondientes
    int suma_de_distancias = calcular_total_de_distancias(dron_buscado);
    float suma_de_duraciones = calcular_total_de_duracion_minutos(dron_buscado); 
    cout << AZUL << NEGRITA << "================================" << RESET << endl;
    cout << AZUL << NEGRITA << "|   ESTAD"<<char(214)<<"STICAS DEL DRON   |" << RESET << endl;
    cout << AZUL << NEGRITA << "================================" << RESET << endl;
    cout << AZUL << "| Modelo: " << dron_buscado->modelo << string(15 - dron_buscado->modelo.length(), ' ') << "|" << RESET << endl;
    cout << AZUL << "| Tiempo total de vuelo: " << suma_de_duraciones << " min" << "|" << RESET << endl; 
    cout << AZUL << "| Distancia total: " << suma_de_distancias << " km" << string(7 - to_string(suma_de_distancias).length(), ' ') << "|" << RESET << endl;
    cout << AZUL << NEGRITA << "================================" << RESET << endl;
}


//*Requerimiento 13 (extra) de la defensa del proyecto: 
//* Reporte de eficiencia, por cada dron buscar la eficiencia, calcular la suma total de distancia entre la suma total de duracion de sus bitacoras
void reporte_de_eficiencia(Dron *&principal){
    if(lista_vacia_drones(principal)){
        cout << ROJO << NEGRITA << "No hay drones en la base de datos" << RESET << endl;
        return;
    }
    Dron *actual_Dron = principal;
    int pos = 1, cantidad_distancia= 0;
    float cantidad_minutos = 0.0, efectividad_duracion_horas = 0.0;
    while (actual_Dron != NULL){

        cantidad_distancia = calcular_total_de_distancias(actual_Dron);
        cantidad_minutos = calcular_total_de_duracion_minutos(actual_Dron);
        if (cantidad_minutos > 0.0)
        {
            efectividad_duracion_horas = cantidad_distancia / cantidad_minutos;
            cout<<"El dron con la posicion "<<pos<<" tiene un indice de eficiencia de: "<< efectividad_duracion_horas<<endl;
        }
        else{
        cout << ROJO << NEGRITA << "No es posible calcular el indice de eficiencia, el dron en la posicion " <<pos<< " no tiene minutos registrados en sus bitacoras" << RESET << endl;
        }
        actual_Dron = actual_Dron->proxDron;
        pos++;
        
    }
}

//* Requerimiento 14) Guardar en un archivo txt
void guardar_en_archivo_txt(Dron *principal) {
    ofstream archivo_salida("reportedrones.txt");
    if (!archivo_salida.is_open()) {
        cout << ROJO << "ERROR: No se pudo crear el archivo de reporte." << RESET << endl;
        return;
    }
    
    cout << BLANCO << NEGRITA << "Guardando datos de drones en el archivo 'reportedrones.txt'..." << RESET << endl;
    
    if (lista_vacia_drones(principal)) {
        archivo_salida << "No hay drones registrados en el sistema.\n";
    } else {
        Dron *actual = principal;
        int contador_dron = 1;
        
        while (actual != NULL) {
            archivo_salida << "DRON #" << contador_dron << "\n";
            archivo_salida << "Modelo: " << actual->modelo << "\n";
            archivo_salida << "N"<<char(163)<<"mero de serie: " << actual->num_serie << "\n";
            archivo_salida << "Tipo: " << actual->tipo << "\n";
            archivo_salida << "Estado: " << actual->estado << "\n";
            archivo_salida << "VUELOS REGISTRADOS\n";
            
            // Listar bitácoras (vuelos)
            Bitacora *actual_bitacora = actual->lista_bitacoras;
            int contador_vuelo = 1;
            
            if (actual_bitacora == NULL) {
                archivo_salida << "No hay vuelos registrados para este dron.\n";
            } else {
                while (actual_bitacora != NULL) {
                    archivo_salida << "VUELO #" << contador_vuelo << "\n";
                    archivo_salida << "  ID: " << actual_bitacora->ID << "\n";
                    archivo_salida << "  Fecha: " << actual_bitacora->fecha << "\n";
                    archivo_salida << "  Duraci"<<char(162)<<"n: " << actual_bitacora->duracion << " minutos\n";
                    archivo_salida << "  Distancia: " << actual_bitacora->distancia_recorrida << " km\n";
                    archivo_salida << "  Objetivo: " << actual_bitacora->objetivo << "\n";
                    
                    actual_bitacora = actual_bitacora->proxBitacora;
                    contador_vuelo++;
                }
            }
            
            archivo_salida << "\n"; // Separador entre drones
            
            actual = actual->proxDron;
            contador_dron++;
        }
    }
    
    archivo_salida.close();
    cout << VERDE << NEGRITA << "OK Reporte guardado exitosamente en 'reportedrones.txt'" << RESET << endl;
}

//* Requerimiento 15) Cargar desde archivo txt
void cargar_desde_archivo_txt(Dron *&principal) {
    ifstream archivo_entrada("reportedrones.txt");
    if (!archivo_entrada.is_open()) {
        cout << AMARILLO << "No se encontr"<<char(162)<<" archivo de datos. Se iniciar"<<char(160)<<" con lista vac"<<char(161)<<"a." << RESET << endl;
        return;
    }

    string linea;
    Dron *dron_actual = NULL;
    Bitacora *ultima_bitacora = NULL;
    bool leyendo_bitacoras = false;

    while (getline(archivo_entrada, linea)) {
        //* Aqui se eliminan espacios al inicio y final
        while (!linea.empty() && (linea[0] == ' ' || linea[0] == '\t')) {
            linea = linea.substr(1);
        }
        while (!linea.empty() && (linea.back() == ' ' || linea.back() == '\t' || linea.back() == '\r')) {
            linea.pop_back();
        }

        //* Se saltan líneas vacías
        if (linea.empty()) {
            continue;
        }

        //* Se detecta el inicio de nuevo dron
        if (linea.find("DRON #") != string::npos) {
            // Crear nuevo dron
            dron_actual = new Dron();
            dron_actual->proxDron = NULL;
            dron_actual->lista_bitacoras = NULL;
            leyendo_bitacoras = false;
            ultima_bitacora = NULL;

            // Agregar a la lista principal
            if (principal == NULL) {
                principal = dron_actual;
            } else {
                Dron *temp = principal;
                while (temp->proxDron != NULL) {
                    temp = temp->proxDron;
                }
                temp->proxDron = dron_actual;
            }
            continue;
        }

        //* Leer datos del dron (solo si no estamos leyendo bitácoras)
        if (dron_actual != NULL && !leyendo_bitacoras) {
            if (linea.find("Modelo: ") != string::npos) {
                dron_actual->modelo = linea.substr(8);
            } 
            else if (linea.find("Numero de serie: ") != string::npos) {
                dron_actual->num_serie = linea.substr(17);
            }
            else if (linea.find("Tipo: ") != string::npos) {
                dron_actual->tipo = linea.substr(6);
            }
            else if (linea.find("Estado: ") != string::npos) {
                dron_actual->estado = linea.substr(8);
            }
            else if (linea.find("VUELOS REGISTRADOS") != string::npos) {
                leyendo_bitacoras = true;
            }
            continue;
        }

        if (leyendo_bitacoras && dron_actual != NULL) {
            if (linea.find("No hay vuelos registrados") != string::npos) {
                leyendo_bitacoras = false;
                dron_actual = NULL;
                continue;
            }
            
            //* En el caso si encontramos una línea vacía después de bitácoras, podría ser el fin de este dron
            if (linea.empty() && ultima_bitacora != NULL) {
                // Podría ser el separador entre drones
                leyendo_bitacoras = false;
                dron_actual = NULL;
                continue;
            }

            //* Se detecta un nuevo vuelo
            if (linea.find("VUELO #") != string::npos) {
                // Crear nueva bitácora
                Bitacora *nueva_bitacora = new Bitacora();
                nueva_bitacora->proxBitacora = NULL;
                
                //* Agregar a la lista del dron
                if (dron_actual->lista_bitacoras == NULL) {
                    dron_actual->lista_bitacoras = nueva_bitacora;
                } else {
                    //* Encontrar el último nodo
                    Bitacora *temp = dron_actual->lista_bitacoras;
                    while (temp->proxBitacora != NULL) {
                        temp = temp->proxBitacora;
                    }
                    temp->proxBitacora = nueva_bitacora;
                }
                ultima_bitacora = nueva_bitacora;
            }
            //* Leemos datos de la bitácora actual
            else if (ultima_bitacora != NULL) {
                if (linea.find("  ID: ") != string::npos) {
                    ultima_bitacora->ID = linea.substr(6);
                }
                else if (linea.find("  Fecha: ") != string::npos) {
                    ultima_bitacora->fecha = linea.substr(9);
                }
                else if (linea.find("  Duracion: ") != string::npos) {
                    string duracion_str = linea.substr(12);
                    size_t pos = duracion_str.find(" ");
                    if (pos != string::npos) {
                        duracion_str = duracion_str.substr(0, pos);
                    }
                    try {
                        ultima_bitacora->duracion = stof(duracion_str);
                    } catch (...) {
                        ultima_bitacora->duracion = 0.0;
                    }
                }
                else if (linea.find("  Distancia: ") != string::npos) {
                    string dist_str = linea.substr(13);
                    size_t pos = dist_str.find(" ");
                    if (pos != string::npos) {
                        dist_str = dist_str.substr(0, pos);
                    }
                    try {
                        ultima_bitacora->distancia_recorrida = stoi(dist_str);
                    } catch (...) {
                        ultima_bitacora->distancia_recorrida = 0;
                    }
                }
                else if (linea.find("  Objetivo: ") != string::npos) {
                    ultima_bitacora->objetivo = linea.substr(12);
                }
            }
        }
    }

    archivo_entrada.close();
    cout << VERDE << NEGRITA << "OK Datos cargados desde archivo 'reportedrones.txt'" << RESET << endl;
}

//* Requerimiento Adicional) Leer archivo txt desde el sistema
//* Función para leer el archivo txt y mostrar su contenido en consola
int leer_archivo_txt(){
    ifstream archivo_entrada("reportedrones.txt");
    if (archivo_entrada.is_open()) { //* Si la apertura del archivo fue exitosa
        string linea;
        cout <<FONDO_VERDE<<BLANCO<<NEGRITA<< "Contenido del archivo:" <<RESET<<endl;
        cout <<VERDE<< "-----------------------------------" <<RESET<<endl;
        while (getline(archivo_entrada, linea)) {
            cout << linea <<endl;
        }
        archivo_entrada.close();
        cout<<VERDE << "-----------------------------------" << RESET<< endl;
    } 
    else {
        cout<< ROJO<< "ERROR! No se pudo abrir el archivo..." << RESET<< endl;
        return 1; 
    }
    return 0;
}

//* Procedimientro para llenar datos de "n" drones
void llenar_datos_dron(Dron *&principal){
    int i = 0, cantidad = 0;
    string modelo, num_serie, tipo, estado;
    bool entrada_valida = false, repetido = true;

    //* Validacion de la entrada para la cantidad de drones
    while(!entrada_valida){ 
        cout << CYAN << "Digite la cantidad de drones: " << RESET;
        if(!(cin >> cantidad) || cantidad < 0){
            cout << ROJO << "ERROR: Debe digitar un numero entero mayor o igual a 0" << RESET << endl;
            cin.clear();
            cin.ignore(1000, '\n');
        } else {
            entrada_valida = true;
        }
    }
    cin.ignore();
    while(i != cantidad){
        cout << VERDE << "\n-- Datos del dron " << i+1 << RESET << endl;
        cout << "| Modelo: ";
        getline(cin, modelo);
        cout << "| Numero de serie: ";
        getline(cin, num_serie);
        if(esta_dron(principal, num_serie)){
            while(esta_dron(principal, num_serie)){
                cout<<ROJO<<NEGRITA<<" ERROR. Ya existe un numero de serie registrado en la base de datos"<<RESET<<endl;
                cout<<VERDE<<"Digite un numero de serie valido: "<<RESET;
                getline(cin, num_serie);
            }
        }
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
    string mensaje = "Bienvenidos al sistema de gestion de drones. Por favor, seleccione una opcion del menu.";
    string comando;
    int resultado = 0; // Inicializamos resultado

    #ifdef __APPLE__
        comando = "say \"" + mensaje + "\"";
        resultado = system(comando.c_str());
    #elif __linux__
        comando = "espeak-ng -v es \"" + mensaje + "\"";
        resultado = system(comando.c_str());
    #elif _WIN32
        comando = "powershell -Command \"Add-Type -AssemblyName System.speech; $synth = New-Object System.Speech.Synthesis.SpeechSynthesizer; $synth.Speak('" + mensaje + "')\"";
        resultado = system(comando.c_str());
    #else
        // Sistema no reconocido
        cout << "Sistema no reconocido. Mostrando texto en consola." <<endl;
        cout << mensaje <<endl;
    #endif

    Dron *principal = NULL; 

    // Cargar datos desde archivo al iniciar
    cargar_desde_archivo_txt(principal);
    
    string num_serie_dron;
    string ID, fecha, objetivo;
    float duracion = 0.0;
    int distancia_recorrida = 0;
    int opc = -1;
    float num_horas = 0.0;
    bool opcion_valida = false;
    do{
        
        cout << MAGENTA << NEGRITA << "========================================" << RESET << endl;
        cout << MAGENTA << NEGRITA << "|     SISTEMA DE GESTI"<<char(224)<<"N DE DRONES    |" << RESET << endl;
        cout << MAGENTA << NEGRITA << "========================================" << RESET << endl;
        cout << CYAN << "1" << RESET << "- Agregar dron/es (al final del registro)" << endl;
        cout << CYAN << "2" << RESET << "- Dar de baja un dron " << endl;
        cout << CYAN << "3" << RESET << "- Listar drones" << endl;
        cout << CYAN << "4" << RESET << "- Buscar un dron " << endl;
        cout << CYAN << "5" << RESET << "- Ordenar drones" << endl;
        cout << CYAN << "6" << RESET << "- Mostrar top 3 drones por horas de vuelo" << endl;
        cout << CYAN << "7" << RESET << "- Mostrar reporte de mantenimiento" << endl;
        cout << CYAN << "8" << RESET << "- Registrar vuelo/bit"<<char(160)<<"cora" << endl;
        cout << CYAN << "9" << RESET << "- Eliminar vuelo registrado/bit"<<char(160)<<"cora" << endl;
        cout << CYAN << "10" << RESET << "- Consultar vuelo registrado/bitacora con mayor duraci"<<char(162)<<"n" << endl;
        cout << CYAN << "11" << RESET << "- Buscar vuelo/bitacora por objetivo" << endl;
        cout << CYAN << "12" << RESET << "- Calcular el tiempo y distancia total de un dron" << endl;
        cout << CYAN << "13" << RESET << "- Calcular el "<<char(161)<<"ndice de eficiencia de los drones" << endl;
        cout << CYAN << "14" << RESET << "- Leer archivo txt" << endl;

        cout << ROJO << "0" << RESET << "- Salir y guardar informaci"<<char(162)<<"n en un archivo txt" << endl;
        
        while(opcion_valida == false){
            cout << AMARILLO << NEGRITA << "\n>> Digite una opci"<<char(162)<<"n: " << RESET;
            if(!(cin>>opc)){
                cout << ROJO << "ERROR: Debe digitar un n"<<char(163)<<"mero de 0-13" << RESET << endl;
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
            cout << AMARILLO << "Digite el n"<<char(163)<<"mero de serie del dron que desea dar de baja: " << RESET;
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
            cout << AMARILLO << "Digite el n"<<char(163)<<"mero de serie del dron a buscar: " << RESET;
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
                cout << ROJO << "ERROR: Entrada no valida para horas." << RESET << endl;
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
            cout << AMARILLO << "Digite el n"<<char(163)<<"mero de serie del dron a agregar la bitacora: " << RESET;
            getline(cin, num_serie_dron); cout<<endl;
            if(esta_dron(principal, num_serie_dron) == false){
                cout << ROJO << "ERROR: No se encontr"<<char(162)<<" el dron con n"<<char(163)<<"mero de serie: " << num_serie_dron << RESET << endl;
            }else{
                cout << VERDE << "-- Datos de la bitacora --" << RESET << endl;
                cout << "| ID de vuelo: ";
                getline(cin, ID);
                cout << "| Fecha del vuelo: ";
                getline(cin, fecha);
                while(opcion_valida == false){
                    cout<< "| Duraci"<<char(162)<<"n (minutos): ";
                    if(!(cin>>duracion) || duracion < 0.0){ //* Validacion de la duracion 
                        cout << ROJO << "ERROR: Debe digitar un n"<<char(163)<<"mero mayor o igual a 0" << RESET << endl;
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
                        cout << ROJO << "ERROR: Debe digitar un n"<<char(163)<<"mero mayor o igual a 0" << RESET << endl;
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
            cout << AMARILLO << "Digite el n"<<char(163)<<"mero de serie del dron: " << RESET;
            getline(cin, num_serie_dron); cout<<endl;
            if(esta_dron(principal, num_serie_dron) == false){
                cout << ROJO << "ERROR: No se encontr"<<char(162)<<" el dron con numero de serie: " << num_serie_dron << RESET << endl;
            }
            else{
                cout << AMARILLO << "Digite el ID de la bit"<<char(160)<<"cora a eliminar: " << RESET;
                getline(cin, ID);
                eliminar_vuelo(principal, num_serie_dron, ID);
            }
            system("pause"); system("cls");
        }
        else if(opc == 10){
            cin.ignore();
            cout << AMARILLO << "Digite el n"<<char(163)<<"mero de serie del dron: " << RESET;
            getline(cin, num_serie_dron); cout<<endl;
            mostrar_vuelo_mas_largo(principal, num_serie_dron);
            system("pause"); system("cls");
        }
        else if(opc == 11){
            cin.ignore();
            cout << AMARILLO << "Digite el n"<<char(163)<<"mero de serie del dron: " << RESET;
            getline(cin, num_serie_dron); cout<<endl;
            cout << AMARILLO << "Digite el objetivo a buscar: " << RESET;
            getline(cin, objetivo); cout<<endl;
            buscar_si_existe_bitacora_con_un_objetivo(principal, num_serie_dron, objetivo);
            system("pause"); system("cls");
        }
        else if(opc == 12){
            cin.ignore();
            cout << AMARILLO << "Digite el n"<<char(163)<<"mero de serie del dron: " << RESET;
            getline(cin, num_serie_dron); cout<<endl;
            calcular_totales_de_un_dron(principal, num_serie_dron);
            system("pause"); system("cls");
        }
        else if(opc==13){
            cin.ignore();
            reporte_de_eficiencia(principal);
            system("pause"); system("cls");
        }
        else if(opc == 14){
            leer_archivo_txt();
            system("pause"); system("cls");
        }

        else if(opc == 0){
            guardar_en_archivo_txt(principal);
            system("pause"); system("cls");
            cout << MAGENTA << NEGRITA << ""<<char(173)<<"Gracias por usar el sistema! Hasta luego..." << RESET << endl;
            string mensaje = "¡Gracias por usar el sistema! Hasta luego...";
            string comando;
            int resultado = 0; // Inicializamos resultado
            #ifdef __APPLE__
            comando = "say \"" + mensaje + "\"";
            resultado = system(comando.c_str());
            #elif __linux__
            comando = "espeak-ng -v es \"" + mensaje + "\"";
            resultado = system(comando.c_str());
            #elif _WIN32
            comando = "powershell -Command \"Add-Type -AssemblyName System.speech; $synth = New-Object System.Speech.Synthesis.SpeechSynthesizer; $synth.Speak('" + mensaje + "')\"";
            resultado = system(comando.c_str());
            #else
            // Sistema no reconocido
            cout << "Sistema no reconocido. Mostrando texto en consola." <<endl;
            cout << mensaje <<endl;
            system("pause"); system("cls");
        #endif
        cout<<endl;
        }else{
            cout << ROJO << "ERROR: Opci"<<char(162)<<"n incorrecta" << RESET << endl;
            string mensaje = "ERROR: Opcion incorrecta, asegurese que la opcion digitada sea correcta.";
            string comando;
            int resultado = 0; // Inicializamos resultado
    
            #ifdef __APPLE__
                comando = "say \"" + mensaje + "\"";
                resultado = system(comando.c_str());
            #elif __linux__
                comando = "espeak-ng -v es \"" + mensaje + "\"";
                resultado = system(comando.c_str());
            #elif _WIN32
                comando = "powershell -Command \"Add-Type -AssemblyName System.speech; $synth = New-Object System.Speech.Synthesis.SpeechSynthesizer; $synth.Speak('" + mensaje + "')\"";
                resultado = system(comando.c_str());
            #else
                // Sistema no reconocido
                cout << "Sistema no reconocido. Mostrando texto en consola." <<endl;
                cout << mensaje <<endl;
            #endif
            cout<<endl;
            system("pause"); system("cls");
        }
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
