/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
// MODELO DE ISING                                                             //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

//BIBLIOTECAS
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "gsl_rng.h" //Librería que me va a permitir generar números aleatorios
//CONSTANTES DEL PROGRAMA
#define d 20 //Dimensión de la matriz
#define P 400  //Número de pasos MonteCarlo
//DEFINICIÓN DEL PUNTERO COMO UNA VARIABLE EXTERNA: me permite utilizarlo también en las funciones
gsl_rng *tau;
//DECLARACIÓN DE VARIABLES EXTERNAS
double T; //Temperatura
int n,m;
int R[dim+2][dim+2]; //MATRIZ RED (Añados dos dimensiones para poder meter las condiciones de contorno de la red)
FILE *f1; //Fichero
//DECLARACIÓN DE FUNCIONES
int Ordenar();
int Desordenar();
int Cambiar();

int main()
{
    extern gsl_rng *tau;  //Puntero al estado del número aleatorio
    int semilla=278349;  //Semilla del generador de números aleatorios
    int q;  //Para elegir si el fichero estará ordenado o desordenado

    //Doy un valor inicial a la temperatura
    T=1;

    //Abro el fichero para guardar los datos
    f1=fopen("ising2.txt","w");

    tau=gsl_rng_alloc(gsl_rng_taus); //Inicializar el puntero
    gsl_rng_set(tau,semilla); //Inicializar la semilla

    //Pregunto si queremos empezr con la red ordenada o desordenada
    printf("Pulsa 0 si ordenado o 1 si desordenado:\n");
    scanf("%i",&q);

    if(q==0) Ordenar();
    else if(q==1) Desordenar();

    fclose(f1);

    return 0;
}

//DEFINICIÓN DE LAS FUNCIONES QUE USO EN EL PROGRAMA...............................................................................
int Ordenar()
{
    //Dar a todos las posiciones de la red el valor 1
     for(int i=1;i<dim;i++)
    {
        for(int j=1;j<dim;j++)
        {
            R[i][j]=1;
        }
        printf("\n");
    }
    //Imprimo en el fichero el valor de la matriz para cada paso Montecarlo
    for(int i=0;i<P;i++)
    {
        for(int j=1;j<dim;j++)
        {
            for(int k=1;k<dim;k++)
            {                    
                fprintf(f1,"%i\t%i\t%i\t",j,k,(R[j][k]+2));
            }
        }
        fprintf(f1,"\n");

        //Pasos montecarlo
        for(int l=0;l<pow(dim,2);l++)
        {
            Cambio();
        }
    }
    return 0;
}

int Desordenar()
{
    int k; //Para guardar un número aleatorio entre 0 y 1

    //Inicializar la matriz aleatoriamente con los números 1 y -1
    for(int i=1;i<dim;i++)
    {
        for(int j=1;j<dim;j++)
        {
            k=gsl_rng_uniform_int(tau,2);
            if(k!=1) //Cuando el número aleatorio me salga 0 lo cambio por el -1 y así consigo los valores que busco
            {
                k=-1;
            }
            R[i][j]=k;
        }
        printf("\n");
    }

    //Imprimo en el fichero el valor de la matriz para cada paso Montecarlo
    for(int i=0;i<P;i++)
    {
        for(int j=1;j<dim;j++)
        {
            for(int k=1;k<dim;k++)
            {                    
                fprintf(f1,"%i\t%i\t%i\t",j,k,(red[j][k]+2));
            }
        }
        fprintf(f1,"\n");

        //Pasos montecarlo
        for(int l=0;l<pow(dim,2);l++)
        {
            Cambio();
        }
    }
    return 0;
}

int Cambiar() //Hace un cambio del valor de spin entre dos posiciones aleatorioas de la matriz red
{
    //Genero dos números aleatorios entre 0 y la demensión de la red: cada número representa una posición en la matriz
    n=gsl_rng_uniform_int(tau,dim); 
    m=gsl_rng_uniform_int(tau,dim);

    //En el siguiente paso se deicide si se realiza o no el cambio:
    // Para ver si el punto en el que estamos cambia, generaremos un número aleatorio entre 0 y 1, al que llamo r
    // si r<p ==> red(n,m)=-red(n,m)
    // donde p=min(1,exp-[dE/T]) con T la temperatura
    // dE=2s(n,m)[s(n+1,m)+s(n-1,m)+s(n,m+1)+s(n,m-1)]

    //DECLARACIÓN DE VARIABLES que necesito en esta función
    double E, p, r, l; //l es un cálculo adicional para obtener p.

    //CÁLCULO DE LA ENERGÍA
    //Impongo las condiciones de contorno 
    R[0][m]=R[dim][m];
    R[dim+1][m]=R[1][m];
    R[n][0]=R[n][dim];
    R[n][dim+1]=R[n][1];
    //Cálculo de E
    E=2*(R[n][m])*(R[n+1][m]+R[n-1][m]+rR[n][m+1]+R[n][m-1]);

    //CÁLCULO DE p
    l=exp(-(E/T));

    if(l>=1) p=1.0;
    else p=l;
    //Genero un número real aleatorio r entre [0:1]
    r=gsl_rng_uniform(tau); 

    //COMPARAR p CON r
    if(r<p) R[n][m]=-(R[n][m]);

    return R[n][m];
}
