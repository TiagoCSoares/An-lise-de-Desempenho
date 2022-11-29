#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

//Fórmula da ocupação:
// ocupacao = chegada/capacidade do sistema

typedef struct little_{
    unsigned long int no_eventos;
    double tempo_anterior;
    double soma_areas;
} little;

double aleatorio()
{
    double u = rand() / ((double)RAND_MAX + 1);
    // limitando entre (0,1]
    u = 1.0 - u;

    return (u);
}

double minimo(double num1, double num2)
{
    if (num1 < num2)
    {
        return num1;
    }
    return num2;
}

double maximo(double num1, double num2)
{
    if (num1 > num2)
    {
        return num1;
    }
    return num2;
}

void inicia_little(little *l)
{
    l->no_eventos = 0;
    l->tempo_anterior = 0.0;
    l->soma_areas = 0.0;
}
    
int main()
{
    double tempo_simulacao;
    double tempo_decorrido = 0.0;
    double tempo_anterior = 0.0;

    double intervalo_media_chegada;
    double tempo_medio_servico;

    double chegada;
    double servico;
    double coleta_dados;

    double soma_tempo_servico = 0.0;

    unsigned long int fila = 0;
    unsigned int max_fila = 0;


    /*
    Little
    */

    little e_n;
    little e_w_chegada;
    little e_w_saida;

    inicia_little(&e_n);
    inicia_little(&e_w_chegada);
    inicia_little(&e_w_saida);

    little e_n_cd;
    little e_w_chegada_cd;
    little e_w_saida_cd;

    inicia_little(&e_n_cd);
    inicia_little(&e_w_chegada_cd);
    inicia_little(&e_w_saida_cd);
    /*
    Little - fim
    */

    srand(time(NULL)); // 0.4 na media

    //printf("Informe o tempo de simulação (segundos): ");
    //scanf("%lF", &tempo_simulacao);
    tempo_simulacao = 36000;

    //printf("Informe o intervalo médio entre chegadas (segundos): ");
    //scanf("%lF", &intervalo_media_chegada);
    intervalo_media_chegada = 0.2;

    printf("Informe o tempo médio de serviço (segundos):");
    scanf("%lF", &tempo_medio_servico);

    // gerando o tempo de chegada da primeira requisição.
    chegada = (-1.0 / (1.0 / intervalo_media_chegada)) * log(aleatorio());
    coleta_dados = 100.0;

    while (tempo_decorrido <= tempo_simulacao)
    {
        tempo_decorrido = !fila ? chegada : minimo(minimo(chegada, servico), minimo(coleta_dados, servico));

        
        // chegada
        if (tempo_decorrido == chegada)
        {
            if (!fila)
            {
                servico = tempo_decorrido + (-1.0 / (1.0 / intervalo_media_chegada)) * log(aleatorio());
                soma_tempo_servico += servico - tempo_decorrido;
            }
            fila++;
            max_fila = fila > max_fila ? fila : max_fila;

            chegada = tempo_decorrido + (-1.0 / (1.0 / intervalo_media_chegada)) * log(aleatorio());

            //little
            e_n.soma_areas += (tempo_decorrido - e_n.tempo_anterior) * e_n.no_eventos;
            e_n.tempo_anterior = tempo_decorrido;
            e_n.no_eventos++;

            e_w_chegada.soma_areas += (tempo_decorrido - e_w_chegada.tempo_anterior) * e_w_chegada.no_eventos;
            e_w_chegada.tempo_anterior = tempo_decorrido;
            e_w_chegada.no_eventos++;
            tempo_anterior = tempo_decorrido;
        }
        // saída
        else if (tempo_decorrido == servico)
        {
            //printf("Saída em %lF.\n", tempo_decorrido);
            fila--;

            if (fila)
            {
                servico = tempo_decorrido + (-1.0 / (1.0 / tempo_medio_servico)) * log(aleatorio());
                soma_tempo_servico += servico - tempo_decorrido;
            }

            //little
            e_n.soma_areas += (tempo_decorrido - e_n.tempo_anterior) * e_n.no_eventos;
            e_n.tempo_anterior = tempo_decorrido;
            e_n.no_eventos--;

            e_w_saida.soma_areas += (tempo_decorrido - e_w_saida.tempo_anterior) * e_w_saida.no_eventos;
            e_w_saida.tempo_anterior = tempo_decorrido;
            e_w_saida.no_eventos++;
            tempo_anterior = tempo_decorrido;
        }
    
        else if(tempo_decorrido == coleta_dados)
        {   

            double e_n_final = e_n.soma_areas / tempo_anterior;
            double e_w_final = (e_w_chegada.soma_areas - e_w_saida.soma_areas) / e_w_chegada.no_eventos;
            double lambda = e_w_chegada.no_eventos / tempo_anterior;

            printf("Tempo decorrido: %lF \n", coleta_dados);
            printf("Ocupação: %lF.\n", soma_tempo_servico / maximo(tempo_decorrido, servico));
            printf("E[N] = %lF\n", e_n_final);
            printf("E[W] = %lF\n", e_w_final);
            printf("Erro de Little: %.20lF\n\n", e_n_final - lambda * e_w_final);
            coleta_dados += 100;
        }
    }
    e_n.soma_areas += (tempo_decorrido - e_n.tempo_anterior) * e_n.no_eventos;
    e_w_chegada.soma_areas += (tempo_decorrido - e_w_chegada.tempo_anterior) * e_w_chegada.no_eventos;
    e_w_saida.soma_areas += (tempo_decorrido - e_w_saida.tempo_anterior) * e_w_saida.no_eventos;

    double e_n_final = e_n.soma_areas / tempo_decorrido;
    double e_w_final = (e_w_chegada.soma_areas - e_w_saida.soma_areas) / e_w_chegada.no_eventos;
    double lambda = e_w_chegada.no_eventos / tempo_decorrido;
    
    printf("E[N] = %lF\n", e_n_final);
    printf("E[W] = %lF\n", e_w_final);
    printf("Lambda = %lF\n\n", lambda);

    printf("Erro de Little: %.20lF\n\n", e_n_final - lambda * e_w_final); 

    printf("Ocupação: %lF.\n", soma_tempo_servico / maximo(tempo_decorrido, servico));
    printf("Max Fila: %d\n", max_fila);

    printf("Tempo decorrido: %lF.\n", tempo_decorrido);
    return 0;
}