#include <iostream>
#include <fstream>
using namespace std;

const int NumEmployees = 1000; // Máximo número de empleados
// ************************************* Comienzo Construccion Heap y sus metodos basicos *************************************************************

// Para la resolucion de este ejercicio decidimos manejar las variables relacionadas a los empleados del almacen salem con una struct
// ya que esto nos facilita la manipulacion de las variables relacionadas a empleado
// por ejemplo al momento de hacer swap en la construccion del heap, usamos la estructura y no las variables directamente
// lo que a nivel de Eficiencia de codigo no es tan notorio , pero da legitimidad al codigo y proporciona facilidad de lectura y entendimiento.
struct Employee
{
    int employeeCode;
    char firstName[50];
    char lastName[50];
    double salary;
};

// En este caso declaramos una funcion auxiliar de intercambio que intercambia los valores de dos punteros a estructuras
// esta funcion facilita el proceso al intercambiar todos los atributos del empleado de una sola vez
// al usar punteros la funcion afecta directamente a los empleados en la estructura que esta alamecada lo que permite visualizar todo de forma rapida y eficiente
void swap(Employee *a, Employee *b)
{
    Employee temp = *a;
    *a = *b;
    *b = temp;
}

// Decidimos utlizar minheap ya que anidado a heapsort esta funcion nos asegura que el orden resultante sera descedente es decir los salarios de mayor a menor
// Esta implementacion esta basada en la pagina 154 del libro introduccion a los algoritmos donde se explica el pseudocodigo de esta funcion sin embargo se modifico para trabajar con structuras
void min_heap(Employee *workers, int size, int largestnode)
{
    // usamos estas variables para calcular el arbol binario y su estructura
    // asumimos que el nodo en el que estamos es el nodo padre
    int index = largestnode;
    int leftSon = 2 * largestnode + 1;  // calculamos su hijo Izquierdo
    int rightSon = 2 * largestnode + 2; // calculamos su hijo derecho
    // determinamos usando estas verificaciones cual de los dos hijos en este caso del codigo tiene el salario mas bajo en comparacion al padre para mantener el heap
    if (leftSon < size && workers[leftSon].salary < workers[index].salary)
    {
        index = leftSon;
    }
    if (rightSon < size && workers[rightSon].salary < workers[index].salary)
    {
        index = rightSon;
    }
    // si el nodo padre es mayor que uno de sus hijos, se interacambia papa e hijo
    if (index != largestnode)
    {
        swap(&workers[largestnode], &workers[index]);
        min_heap(workers, size, index); // luego se llama nuevamente la funcion para mantener la estrucutra del monticulo
    }
}

/*****************  Funciones basicas de Heap (No utilizadas en el heapsort, pero si implementadas) ***************************************/

// Esta funcion inserta un nuevo empleado en el min-heap
void insert_heap(Employee *&workers, int &size, Employee newEmployee)
{
    size++;                          // se aumenta el tamaño del heap
    workers[size - 1] = newEmployee; // y este nuevo empleado se almacena en la ultima posicion del heap
    int i = size - 1;                // esta variable se le asigna el indice donde se ha encontrado el nuevo empleado
    while (i != 0)
    {                                                   // recorremos el heap hasta comparar el nuevo empleado con su padre y compraramos los salarios para saber que hacer
        int parent = (i - 1) / 2;                       // debemos calcular el indiice del padre si el empleado que se agrego esta en el indicie 8 por asi decirlo , el padre seria 8-1/2 haciendo floor seria 3
        if (workers[parent].salary > workers[i].salary) // entonces comparamos el valor del inidice 3 para ver si es mayor o menor que el hijo
        {
            swap(&workers[parent], &workers[i]); // si es cierto se cambia el padre con el hijo
            i = parent;
        }
        else
        {
            break; // si no se termina la fucnion
        }
    }
}

// Funcion pque busca un empleado dentro del heap
int search_in_heap(Employee *workers, int size, int employeeCode)
{
    // para facilitar la busqueda decidimos hacerlo por codigo de empleado y no por el valor que un indice puede tener
    //  se recorrre el heap
    for (int i = 0; i < size; i++)
    {
        // si el empleado tiene el mismo codigo lo devolvemos si no se devuelve -1 para saber que no se encontro
        if (workers[i].employeeCode == employeeCode)
            return i;
    }
    return -1; // Si no lo encuentra, devuelve -1
}

// Funcion que elimina un empleado del heap
void delete_in_heap(Employee *&workers, int &size, int employeeCode)
{
    // se utiliza la funcion de buscar previamente declarada y para verificar que el empleado existe
    int i = search_in_heap(workers, size, employeeCode);
    if (i == -1)
    {
        cout << "Empleado no encontrado.\n";
        return;
    } // si la funcion devuelve -1 el usuario no existe , se devuelve un cout , pero si e existe entonces
    // Intercambiamos con el ultimo nodo y reducimos el tamaño del heap para asegurarnos que no se vaya a quebrantar las propiedades del heap en nigun momento
    swap(&workers[i], &workers[size - 1]);
    size--;
    min_heap(workers, size, i); // luego de esto aplicamos para restaurar la propiedad del heap a partir del índice i
}

// Funcion para modidifiar un empleado del heap
void modify_heap(Employee *&workers, int &size, int employeeCode, double newSalary)
{
    // se utiliza la funcion de buscar previamente declarada y para verificar que el empleado existe
    int index = search_in_heap(workers, size, employeeCode);
    if (index == -1)
    {
        cout << "Empleado no encontrado.\n";
        return;
    } // si la funcion devuelve -1 el usuario no existe , se devuelve un cout , pero si e existe entonces
    // asiganmos el nuevo salario que recibimos en la variable newSalary , para cuando esta funcion fuere invocada
    workers[index].salary = newSalary;
    // luego de esto aplicamos para restaurar la propiedad del heap a partir del índice i
    min_heap(workers, size, index);
}

// ************************************* Fin Construccion Heap y sus metodos basicos *************************************************************

// ************************************* Comienzo Logica de Ordenamiento por Heaps (Heapsort) *************************************************************

// ACLARACION : Entendemos que build min heap es parte de las funciones basicas del heap por lo que se explicarar a continuacion, sin embargo se decidio poner en esta seccion de comentarios
// por que consideramos que es fundamental para el heapsort , pero sabemos que esta funcion es utilizada independientemente si el heap sufre de ordenamiento ordena o no.

// a partir de la estructura de empleados , construimos un heap
void build_min_heap(Employee *workers, int size)
{
    // En un arreglo que representa un arbol binario, todos los nodos a partir de este indice son nodos padres. Cualquier nodo que este mas alla de este punto (en size / 2 hasta size - 1)
    //  no tiene hijos y por lo tanto, no necesita ser ajustado
    for (int i = size / 2 - 1; i >= 0; i--)
    {
        min_heap(workers, size, i);
    }
}

// Funcion que se encarga de la logica del programa
void heap_sort(Employee *workers, int size)
{
    build_min_heap(workers, size); // basicamente  Convertimos el arreglo de empleados en un Min-Heap
    // El bucle realiza la ordenaciin intercambiando la raiz del heap
    for (int i = size - 1; i > 0; i--)
    {
        swap(&workers[0], &workers[i]); // intercambiamos posiciones
        min_heap(workers, i, 0);        // se ajusta el heap para que vuelva a cumplir la propiedad del Min-Heap
    }
}

// ************************************* Fin Logica de Ordenamiento por Heaps (Heapsort) *************************************************************

/******** Funciones que para lectura de Datos *******/
// Funcion Auxiliar para la lectura de datos
int read_employees(Employee *heapTree)
{
    ifstream inputFile("salarios.txt");
    if (!inputFile)
    {
        cout << "Hubo un error abriendo el archivo vuelve a intentarlo" << endl;
        return -1;
    }

    int size = 0;
    while (size < NumEmployees && inputFile >> heapTree[size].employeeCode >> heapTree[size].firstName >> heapTree[size].lastName >> heapTree[size].salary)
    {
        if (heapTree[size].employeeCode <= 0 || heapTree[size].salary < 0)
        {
            cout << "Error: datos invalidos en la posicion " << size + 1 << endl;
            return -1;
        }
        size++;
    }
    inputFile.close();

    return size;
}

// funcion para inicialisar los valores de la struct,para evitar lo mas que se pueda errores basura
void initialize_employees(Employee *heapTree, int maxEmployees)
{
    // Inicializamos manualmente los valores de cada empleado en el arreglo
    for (int i = 0; i < maxEmployees; ++i)
    {
        heapTree[i].employeeCode = 0;
        heapTree[i].firstName[0] = '\0'; // Cadenas vacías
        heapTree[i].lastName[0] = '\0';  // Cadenas vacías
        heapTree[i].salary = 0.0;
    }
}

// ************************************* Logica principal del programa *************************************************************
int business_logic()
{
    Employee heapTree[NumEmployees];

    // Inicializamos los empleados manualmente
    initialize_employees(heapTree, NumEmployees);

    int size = read_employees(heapTree); // Llama a la función para leer los empleados

    if (size <= 0)
    {
        cout << "Archivo abierto pero no se encontraron Datos" << endl;
        return -1;
    }

    heap_sort(heapTree, size); // Ordena los empleados por salario
    cout << "\n******Bienvenido al Sistema de Ordenamiento de Almacenes Salem *****";
    cout << "\nEmpleados ordenados por salario de menor a mayor:\n"
         << endl;

    // Imprimir los resultados
    cout << "Código | Nombre Completo          | Salario" << endl;
    cout << "----------------------------------------------" << endl;

    int cont = 0;
    for (int i = 0; i < size; i++)
    {
        cont++;
        cout << heapTree[i].employeeCode << "     | "
             << heapTree[i].firstName << " " << heapTree[i].lastName
             << "     | $ " << heapTree[i].salary << "\n";
    }

    cout << "\n.............Se Ordenaron: " << cont << " empleados" << endl;
    cout << ".............Hasta Luego:     " << endl;

    return 0;
}

int main()
{
    business_logic();
    return 0;
}
