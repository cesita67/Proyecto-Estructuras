#include <iostream>
#include <stdlib.h>
#include <string>
using namespace std;


struct Bitacora{ //! Bitacora = Vuelos registrados (son sinonimos)
    string ID;
    string fecha;
    int duracion;
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
int calcular_total_de_duracion_minutos(Dron);
int convertir_minutos_a_horas(int);

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

Bitacora *Crear_bitacora(string ID, string fecha, int duracion, int distancia_recorrida, string objetivo){
    Bitacora *nueva_bitacora = new Bitacora();
    nueva_bitacora->ID = ID;
    nueva_bitacora->fecha = fecha;
    nueva_bitacora->duracion = duracion;
    nueva_bitacora->distancia_recorrida = distancia_recorrida;
    nueva_bitacora->objetivo = objetivo;
    nueva_bitacora->proxBitacora = NULL;

    return nueva_bitacora;
}

bool lista_vacia_drones(Dron *principal){
    return principal == NULL;
}


//* Funcion para calcular el total de duracion en minutos
int calcular_total_de_duracion_minutos(Dron *actual){
    if(actual == NULL){
        return 0;
    }
    Bitacora *actual_bitacora = actual->lista_bitacoras;
    if(actual_bitacora == NULL){
        return 0;
    }
    int sumatoria_duraciones = 0;
    while(actual_bitacora != NULL){
        sumatoria_duraciones += actual_bitacora->duracion;
        actual_bitacora = actual_bitacora->proxBitacora;
    }
    return sumatoria_duraciones;
}

//* Funcion para convertir minutos a horas
int convertir_minutos_a_horas(int min){
    int horas = 0;
    horas = min/60;
    return horas;
}

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
    // Pedir_datos_dron(&modelo, &num_serie, &tipo, &estado);
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

//* Funcion para retornar un puntero a un dron segun su número de serie
Dron *dron_por_num_de_serie(Dron *principal, string num_serie_buscado){
    if(lista_vacia_drones(principal)){
        cout<<"No hay drones en la base de datos"<<endl;
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
        cout<<"No hay drones en la base de datos..."<<endl;
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
        cout<<"No existe un dron con numero de serie "<<num_serie<<"..."<<endl;
        return;
    }
    
    elim_bitacoras = actual->lista_bitacoras; 
    aux = elim_bitacoras; //* Esto no es necesario pero bueno...
    if(elim_bitacoras != NULL){ //* Comenzamos a eliminar las bitacoras
        while(elim_bitacoras != NULL){
            aux = aux->proxBitacora;
            delete elim_bitacoras;
            elim_bitacoras = aux;
        }
    }else{ //* Si el dron a eliminar no posee algun registro de bitácoras
        cout<<"Dron a eliminar sin bitácoras"<<endl;
    }
    
    //* Comenzamos a eliminar el Dron como tal
    if(principal == actual){ //* Caso borde: El dron a eliminar se enceuntra en el inicio de la lista de drones
        principal = principal->proxDron;
        delete actual;
    }
    else{ //* Caso contrario el dron no esta al inicio
        prev->proxDron = actual->proxDron;
        delete actual;
    }
}

//* Requerimiento 3) Procedimiento para digitar todos todos los drones de la flota con su respectiva informacion
void listar_drones(Dron *principal){
    if(lista_vacia_drones(principal)){
        cout<<"No hay drones en la base de datos..."<<endl;
        return;
    }
    Dron *actual = principal;
    int i = 1;
    while(actual != NULL){
        cout<<"------------------"<<endl;
        cout<<"| Datos del Dron "<<i<<" | "<<endl;
        cout<<"| Modelo: "<<actual->modelo<<" | "<<endl;
        cout<<"| Numero de serie: "<<actual->num_serie<<" | "<<endl;
        cout<<"| Tipo de Dron: "<<actual->tipo<<" | "<<endl;
        cout<<"| Estado: "<<actual->estado<<" | "<<endl;
        actual = actual->proxDron;
        i++;
        cout<<endl; //* Salto de linea
    }
    cout<<"------------------"<<endl;
    
}

//* Requerimiento 4) Buscamos un dron por su numero de serie
void buscar_dron_por_num_de_serie(Dron *principal, string num_serie_buscado){
    if(lista_vacia_drones(principal)){
        cout<<"No hay drones en la base de datos"<<endl;
        return;
    }
    Dron *actual = principal;
    int i = 1;
    while(actual != NULL && actual->num_serie != num_serie_buscado){
        actual = actual->proxDron;
        i++;
    }
    if(actual == NULL){
        cout<<"No se encontro el dron con numero de serie "<<num_serie_buscado<<" en la base de datos"<<endl;
        return;
    }
    cout<<"Dron exitosamente encontrado en la posicion "<<i<<endl;
    cout<<"------------------"<<endl;
    cout<<"| Datos del Dron "<<i<<" | "<<endl;
    cout<<"| Modelo: "<<actual->modelo<<" | "<<endl;
    cout<<"| Numero de serie: "<<actual->num_serie<<" | "<<endl;
    cout<<"| Tipo de Dron: "<<actual->tipo<<" | "<<endl;
    cout<<"| Estado: "<<actual->estado<<" | "<<endl;
    
    //* Digitando los datos de las bitacoras del dron buscado
    Bitacora *actual_bitacora = actual->lista_bitacoras;
    int pos_bitacora = 1;
    if(actual_bitacora == NULL){
        cout<<"El dron no posee bitacoras registradas"<<endl;
        return;
    }
    cout<<"Digitando biracoras del dron registrado"<<endl;
    system("pause");
    
    while(actual_bitacora != NULL){
        cout<<"Bitacora "<<pos_bitacora<<endl;
        cout<<"ID: "<<actual_bitacora->ID<<endl;
        cout<<"Fecha del vuelo "<<actual_bitacora->fecha<<endl;
        cout<<"Duracion: "<<actual_bitacora->duracion<<endl;
        cout<<"Distancia recorrida (km): "<<actual_bitacora->distancia_recorrida<<endl;
        cout<<"Objetivo: "<<actual_bitacora->objetivo<<endl;
        actual_bitacora = actual_bitacora->proxBitacora;
        cout<<endl;
        pos_bitacora++;
    }
}

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

//* Procedimiento para insertar un dron. A diferencia del procedimmiento para insertar un dron al final
//* este procedimiento no necesita los datos de un dron solo el puntero del dron a insertar
void insertar_dron(Dron *&principal, Dron *&dron_a_insertar){
    if(lista_vacia_drones(principal)){
        principal = dron_a_insertar;
        return;
    }
    Dron *actual = principal;
    while(actual->proxDron != NULL){
        actual = actual->proxDron;
    }
    actual->proxDron = dron_a_insertar;
}

void insertar_dron_ordenadamente(Dron *&principal, Dron *&dron_a_insertar){
    if(lista_vacia_drones(principal)){
        principal = dron_a_insertar;
        return;
    }
    if(dron_a_insertar->modelo < principal->modelo){
        dron_a_insertar->proxDron = principal;
        principal = dron_a_insertar;
        return;
    }
    Dron *actual_dron = principal;
    while(actual_dron->proxDron != NULL && actual_dron->proxDron->modelo < dron_a_insertar->modelo){
        actual_dron = actual_dron->proxDron;
    }
    dron_a_insertar->proxDron = actual_dron->proxDron;
    actual_dron->proxDron = dron_a_insertar;
}

//* Requerimiento 5) Ordenar los drones alfabeticamente por modelo
void ordenar_drones_alfabeticamente(Dron *&principal){
    if(lista_vacia_drones(principal)){
        cout<<"No hay drones en la base de datos"<<endl;
        return;
    }
    Dron *lista_drones_ordenados = NULL;
    int insertaciones = 0;
    while(principal != NULL){
        Dron *sig = principal->proxDron; //* Guardo un puntero que apunte al siguiente de actual
        principal->proxDron = NULL; //* Desconecto el puntero principal
        insertar_dron_ordenadamente(lista_drones_ordenados, principal); //* Inserto el nodo principal segun corresponda en la lista ordenada de drones
        principal = sig;
    }
    principal = lista_drones_ordenados; //* Ahora el nodo cabeza (principal) apunta a la nueva lista ordenada de drones

    cout<<"Los drones han sido ordenados segun su modelo exitosamente"<<endl;
}

// while(i != NULL){
//     while(actual_dron->proxDron != NULL){
//         if(actual_dron->proxDron->modelo > actual_dron->modelo && !esta_dron(lista_drones_ordenados, actual_dron->num_serie)){
//             dron_a_insertar = actual_dron;
//         }
//         actual_dron = actual_dron->proxDron;
//     }
//     i = i->proxDron;
//     actual_dron = i;
//     insertar_dron(lista_drones_ordenados, dron_a_insertar);
// }
// principal = lista_drones_ordenados; //* Se actualiza la lista original de drones

// void ordenar_drones_alfabeticamente(Dron *&principal){
//     if(lista_vacia_drones(principal)){
//         cout<<"No hay drones en la base de datos"<<endl;
//         return;
//     }
//     Dron *Tope = principal;
//     while(Tope->proxDron != NULL){
//         Tope = Tope->proxDron;
//     }
//     Dron *actual_dron = principal, *dron_a_insertar = NULL;
//     string modelo_menor = actual_dron->modelo; //* Se asume el modelo menor como el primero

//     while(actual_dron->proxDron != NULL){
//         if(actual_dron->modelo <= actual_dron->proxDron->modelo){
//             dron_a_insertar = actual_dron;
//         }
//         actual_dron = actual_dron->proxDron;
//     }

// }

//* Requerimiento 6) Top 3 drones por hora de vuelo
void mostrar_top_3_drones_por_hora_de_vuelo(Dron *principal){
    if(lista_vacia_drones(principal)){
        cout<<"No hay drones en la base de datos"<<endl;
        return;
    }
    if(cantidad_drones(principal) < 3){
        cout<<"Imposible realizar la solicitud, existen menos de 3 drones"<<endl;
        return;
    }
    Dron *actual_dron = principal;
    Dron *dron_a_imprimir = NULL, *dron_1 = NULL, *dron_2 = NULL;
    int pos = 1; //* Este contador aumentara cada vez que encuentre un dron con mayor horas de vuelo
    int tiempo_de_vuelo_actual = 0, tiempo_de_vuelo_mayor = 0; //* EL tiempo de vuelo mayor estara en horas

    while(pos <= 3){
        while(actual_dron != NULL){
            tiempo_de_vuelo_actual = calcular_total_de_duracion_minutos(actual_dron);
            tiempo_de_vuelo_actual = convertir_minutos_a_horas(tiempo_de_vuelo_actual);
            if(tiempo_de_vuelo_actual > tiempo_de_vuelo_mayor && actual_dron != dron_1 && actual_dron != dron_2){
                tiempo_de_vuelo_mayor = tiempo_de_vuelo_actual; //* Actualizo la variable de tiempo de vuelo mayor
                dron_a_imprimir = actual_dron;
            }
            actual_dron = actual_dron->proxDron;
        }
        cout<<"Top numero "<<pos<<endl;
        cout<<"Modelo: "<<dron_a_imprimir->modelo<<endl;
        cout<<"Numero de serie: "<<dron_a_imprimir->num_serie<<endl;
        cout<<"Tipo de dron: "<<dron_a_imprimir->tipo<<endl;
        cout<<"Estado: "<<dron_a_imprimir->estado<<endl;
        if(pos == 1){
            dron_1 = dron_a_imprimir;
        }
        if(pos == 2){
            dron_2 = dron_a_imprimir;
        }
        actual_dron = principal;
        tiempo_de_vuelo_mayor = 0;
        pos++;
    }
}

//* Requerimiento 7) Reporte de Mantenimiento
void reporte_de_mantenimiento(Dron *principal, int horas_de_vuelo){
    if(lista_vacia_drones(principal)){
        cout<<"No hay drones en la base de datos"<<endl;
        return;
    }
    Dron *actual_dron = principal;
    int duraciones_de_dron = 0, pos_dron = 1;
    bool se_requiere_mantenimiento = false;
    while(actual_dron != NULL){
        duraciones_de_dron = calcular_total_de_duracion_minutos(actual_dron);
        duraciones_de_dron = convertir_minutos_a_horas(duraciones_de_dron);
        if((actual_dron->estado == "Operativo" || actual_dron->estado == "operativo") && duraciones_de_dron > horas_de_vuelo){
            se_requiere_mantenimiento = true;
            cout<<"-> Se sugiere hacer mantenimiento al dron numero "<<pos_dron<<endl;
        }
        pos_dron++;
        actual_dron = actual_dron->proxDron;
    }
    if(se_requiere_mantenimiento == false){
        cout<<"No se requiere mantenimiento a ningun un dron"<<endl;
    }
}

//* Requerimiento 8) Agregar una nueva bitácora de vuelo a un dron específico por su numero de serie
void agregar_bitacora_de_ultimo(Dron *&principal, string num_serie, string ID, string fecha, int duracion, int distancia_recorrida, string objetivo){
    //* Primero buscamos el dron segun su numero de serie para despues para agregar la bitacora 
    Dron *dron_para_agregar_bitacora = dron_por_num_de_serie(principal, num_serie);
    if(dron_para_agregar_bitacora == NULL){
        cout<<"No se pudo agregar la bitacora (registro de vuelo) porque no existe una dron con numero de serie"<<num_serie<<" en la base de datos"<<endl;
        return;
    }
    Bitacora *nueva_bitacora = Crear_bitacora(ID, fecha, duracion, distancia_recorrida, objetivo);
    if(dron_para_agregar_bitacora->lista_bitacoras == NULL){ //* Si no hay bitacoras en el dron buscado
        dron_para_agregar_bitacora->lista_bitacoras = nueva_bitacora; //* Ahora insertamos como primera Bitacora la bitacora creada recientemente
        return;
    }
    //* Caso contrario: El dron ya tiene bitacoras, comenzamos a recorrer la sublista bitacoras para agregar de ultimo la nueva bitacora creada recientemente
    Bitacora *actual_bitacora = dron_para_agregar_bitacora->lista_bitacoras;
    while(actual_bitacora->proxBitacora != NULL){
        actual_bitacora = actual_bitacora->proxBitacora;
    }
    actual_bitacora->proxBitacora = nueva_bitacora;
}

//* Requerimiento 9) Eliminar vuelo/ bitacora segun su ID
void eliminar_vuelo(Dron *&principal, string num_serie, string ID){
    if(lista_vacia_drones(principal)){
        cout<<"No hay drones en la base de datos..."<<endl;
        return;
    }
    Dron *dron_a_eliminar_su_bitacora = dron_por_num_de_serie(principal, num_serie);
    if(dron_a_eliminar_su_bitacora == NULL){
        cout<<"No se encontro el dron a eliminar su vuelo o bitacora"<<endl;
        return;
    }
    Bitacora *actual_bitacora = dron_a_eliminar_su_bitacora->lista_bitacoras, *prev_bitacora = NULL;
    while(actual_bitacora != NULL && actual_bitacora->ID != ID){
        prev_bitacora = actual_bitacora;
        actual_bitacora = actual_bitacora->proxBitacora;
    }
    if(actual_bitacora == dron_a_eliminar_su_bitacora->lista_bitacoras){
        dron_a_eliminar_su_bitacora->lista_bitacoras = dron_a_eliminar_su_bitacora->lista_bitacoras->proxBitacora;
        delete actual_bitacora;
    }else{
        prev_bitacora->proxBitacora = actual_bitacora->proxBitacora;
        delete actual_bitacora;
    }
}

//* Requerimiento 10) Consultar vuelo mas largo
void mostrar_vuelo_mas_largo(Dron *&principal, string num_serie){
    if(lista_vacia_drones(principal)){
        cout<<"No hay drones en la base de datos..."<<endl;
        return;
    }
    Dron *dron_buscado = dron_por_num_de_serie(principal, num_serie);
    if(dron_buscado == NULL){
        cout<<"No se encontro el dron solicitado en la base de datos"<<endl;
        return;
    }
    Bitacora *actual_bitacora = dron_buscado->lista_bitacoras, *bitacora_a_imprimir = NULL; //* Primero creamos un puntero bitacora auxiliar para recorrer toda la lista de bitacoras y determinar cual es la bitacora con mayor duracion 
    //* Despues creamos un puntero bitacora_a_imprimir que apuntara a la bitacora que tenga mas duracion
    int duracion_mayor = 0, pos_bitacora = 1, pos_bitacora_buscada = 1;
    while(actual_bitacora != NULL){
        if(actual_bitacora->duracion >= duracion_mayor){
            duracion_mayor = actual_bitacora->duracion;
            bitacora_a_imprimir = actual_bitacora; //* Apuntamos la bitacora_a_imprimir como la bitacora actual
            pos_bitacora_buscada = pos_bitacora;
        }
        actual_bitacora = actual_bitacora->proxBitacora;
        pos_bitacora++; 
    }
    if(duracion_mayor <= 0){ //* Si todas las bitacoras no tienen duracion alguna de los vuelos
        cout<<"Ninguna bitacora tiene duracion alguna..."<<endl;
        return;
    }
    cout<<"Digitando la bitacora "<<pos_bitacora_buscada<<" que contiene la mayor duracion de vuelo"<<endl;
    cout<<"-> ID del vuelo: "<<bitacora_a_imprimir->ID<<endl;
    cout<<"-> Fecha del vuelo: "<<bitacora_a_imprimir->fecha<<endl;
    cout<<"-> Duracion: "<<bitacora_a_imprimir->duracion<<" minutos"<<endl;
    cout<<"-> Distancia recorrida: "<<bitacora_a_imprimir->distancia_recorrida<<" km"<<endl;
    cout<<"-> Objetivo: "<<bitacora_a_imprimir->objetivo<<endl;
}

//* Requerimiento 11) Buscar si en un dron dado posee una bitacora/vuelo registrado con un objetivo en especifico
void buscar_si_existe_bitacora_con_un_objetivo(Dron *principal, string num_serie, string objetivo_buscado){
    if(lista_vacia_drones(principal)){
        cout<<"No hay drones en la base de datos"<<endl;
        return;
    }
    Dron *dron_buscado = dron_por_num_de_serie(principal, num_serie);
    if(dron_buscado == NULL){
        cout<<"No se encontro el dron solicitado en la base de datos"<<endl;
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
        cout<<"No se encontro un vuelo registrado/bitacora con el objetivo ("<<objetivo_buscado<<")"<<endl;
    }else{
        cout<<"Se ha encontrado un vuelo registrado/bitacora con objetivo ("<<objetivo_buscado<<") en la posicion "<<pos_bitacora_buscada<<endl;
    }
}

//* Requerimiento 12) Calcular totales de un dron en especifico
void calcular_totales_de_un_dron(Dron *&principal, string num_serie){
    if(lista_vacia_drones(principal)){
        cout<<"No hay drones en la base de datos..."<<endl;
        return;
    }
    Dron *dron_buscado = dron_por_num_de_serie(principal, num_serie);
    if(dron_buscado == NULL){
        cout<<"No se encontro el dron solicitado en la base de datos"<<endl;
        return;
    }
    //* Se calcula la suma de distancias y la suma de duraciones con las funciones correspondientes
    int suma_de_distancias = calcular_total_de_distancias(dron_buscado), suma_de_duraciones = calcular_total_de_duracion_minutos(dron_buscado);
    cout<<"------------------------"<<endl;
    cout<<"Datos del dron de modelo "<<dron_buscado->modelo<<endl;
    cout<<"Suma de duraciones (tiempo total de vuelo): "<<suma_de_duraciones<<endl;
    cout<<"Suma de distancias recorridas (distancia total recorrida): "<<suma_de_distancias<<endl;
}

void llenar_datos_dron(Dron *&principal){
    int i = 0, cantidad = 0;
    string modelo, num_serie, tipo, estado;
    cout<<"Digite la cantidad de drones: "<<endl;
    cin>>cantidad;
    while(i != cantidad){
        cout<<"-- Digite datos del dron numero "<<i+1<<"--"<<endl;
        cout<<"Modelo: ";
        cin>>modelo;
        cout<<"Numero de serie: ";
        cin>>num_serie;
        cout<<"Tipo de dron: ";
        cin>>tipo;
        cout<<"Estado: ";
        cin>>estado;
        agregar_dron_de_ultimo(principal, modelo, num_serie, tipo, estado);
        i++;

    }
}

int main(){
    Dron *principal = NULL; 
    string num_serie_dron;
    string ID, fecha, objetivo;
    int duracion, distancia_recorrida;
    int opc = -1, num_horas = 0;
    bool opcion_valida = false;
    do{
        cout<<"\t..-MENU PARA GESTIONAR DRONES-.."<<endl;
        cout<<"1- Agregar dron/es (al final del registro de la base datos)"<<endl;
        cout<<"2- Dar de baja un dron "<<endl;
        cout<<"3- Listar drones"<<endl;
        cout<<"4- Buscar un dron "<<endl;
        cout<<"5- Ordenar drones"<<endl;
        cout<<"6- Mostrar top 3 drones por horas de vuelo"<<endl;
        cout<<"7- Mostrar reporte de mantenimiento"<<endl;
        cout<<"8- Registrar vuelo"<<endl;
        cout<<"9- Eliminar vuelo registrado/bitacora"<<endl;
        cout<<"10- Consultar vuelo/bitacora mas largo"<<endl;
        cout<<"11- Buscar vuelo/bitacora por objetivo"<<endl;
        cout<<"12- Calcular el tiempo y distancia total recorrida de un dron"<<endl;
        cout<<"0- Salir"<<endl;
        while(opcion_valida == false){
            cout<<"Digite una opcion: ";
            if(!(cin>>opc)){
                cout<<"ERROR. Debe digitar un numero de 1-12"<<endl;
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
            cout<<"Digite el numero de serie del dron que desea dar de baja: "; 
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
            cout<<"Digite el numero de serie del dron a buscar: ";
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
            cout<<"Digite las horas de vuelo a considerar: ";
            cin>>num_horas; cout<<endl;
            reporte_de_mantenimiento(principal, num_horas);
            system("pause"); system("cls");
        }
        else if(opc == 8){
            cin.ignore();
            cout<<"Digite el numero de serie del dron a agregar la bitacora/registro de vuelo: ";
            getline(cin, num_serie_dron); cout<<endl;
            if(esta_dron(principal, num_serie_dron) == false){
                cout<<"No se encontro el dron con numero de serie: "<<num_serie_dron<<endl;
            }else{
                cin.ignore();
                cout<<"--Digite datos de la bitacora/registro de vuelo-- "<<endl;
                cout<<"ID de vuelo: ";
                cin>>ID;
                cout<<"Fecha del vuelo: ";
                cin>>fecha;
                cout<<"Duracion: ";
                cin>>duracion;
                cout<<"Distancia recorrida: ";
                cin>>distancia_recorrida;
                cout<<"Objetivo: ";
                cin>>objetivo;
                agregar_bitacora_de_ultimo(principal, num_serie_dron, ID, fecha, duracion, distancia_recorrida, objetivo);
            }
            system("pause"); system("cls");
        }
        else if(opc == 9){
            cin.ignore();
            cout<<"Digite el numero de serie del dron a eliminar una bitacora de la base de datos: ";
            cin>>num_serie_dron; cout<<endl;
            if(esta_dron(principal, num_serie_dron) == false){
                cout<<"No se encontro el dron con numero de serie: "<<num_serie_dron<<endl;
            }
            else{
                cout<<"Digite el ID de la bitacora/registro de vuelo a eliminar: ";
                cin>>ID;
                eliminar_vuelo(principal, num_serie_dron, ID);
            }
            system("pause"); system("cls");
        }
        else if(opc == 10){
            cin.ignore();
            cout<<"Digite el numero de serie del dron: ";
            cin>>num_serie_dron; cout<<endl;
            mostrar_vuelo_mas_largo(principal, num_serie_dron);
            system("pause"); system("cls");
        }
        else if(opc == 11){
            cin.ignore();
            cout<<"Digite el numero de serie del dron: ";
            cin>>num_serie_dron; cout<<endl;
            cout<<"Digite el objetivo de la bitacora/registro de vuelo a buscar: ";
            cin>>objetivo; cout<<endl;
            buscar_si_existe_bitacora_con_un_objetivo(principal, num_serie_dron, objetivo);
            system("pause"); system("cls");
        }
        else if(opc == 12){
            cin.ignore();
            cout<<"Digite el numero de serie del dron a calcular: ";
            cin>>num_serie_dron; cout<<endl;
            calcular_totales_de_un_dron(principal, num_serie_dron);
            system("pause"); system("cls");
        }
        else if(opc == 0){
            cout<<"¡Parece que te vas! Hasta luego..."<<endl;
            system("pause"); system("cls");
        }else{
            cout<<"ERROR. Opcion incorrecta"<<endl;
            system("pause"); system("cls");
        }
        cout<<endl;
    }while(opc != 0);

    Dron *elim = principal;
    if(principal != NULL){
        Bitacora *elim_bitacora = principal->lista_bitacoras;        
        while(elim != NULL){
            while(elim_bitacora !=NULL){
                principal->lista_bitacoras = principal->lista_bitacoras->proxBitacora;
                delete elim_bitacora;
                elim_bitacora = principal->lista_bitacoras;
            }
            principal = principal->proxDron;
            delete elim;
            elim = principal;
            elim_bitacora = principal->lista_bitacoras;
        }
        cout<<"Memoria liberada..."<<endl;
    }

    system("pause");
    return 0;
}