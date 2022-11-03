#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

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

int main()
{
    double tempo_simulacao;
    double tempo_decorrido = 0.0;

    double intervalo_media_chegada;
    double tempo_medio_servico;

    double chegada;
    double servico;

    double soma_tempo_servico = 0.0;

    unsigned long int fila = 0;
    unsigned int max_fila = 0;

    srand(time(NULL)); // 0.4 na media

    printf("Informe o tempo de simulação (segundos): ");
    scanf("%lF", &tempo_simulacao);

    printf("Informe o intervalo médio entre chegadas (segundos): ");
    scanf("%lF", &intervalo_media_chegada);

    printf("Informe o tempo médio de serviço (segundos): ");
    scanf("%lF", &tempo_medio_servico);

    // gerando o tempo de chegada da primeira requisição.
    chegada = (-1.0 / (1.0 / intervalo_media_chegada)) * log(aleatorio());

    while (tempo_decorrido <= tempo_simulacao)
    {
        tempo_decorrido = !fila ? chegada : minimo(chegada, servico);

        // chegada
        if (tempo_decorrido == chegada)
        {
            //printf("Chegada em %lF.\n", tempo_decorrido);
            if (!fila)
            {
                servico = tempo_decorrido + (-1.0 / (1.0 / intervalo_media_chegada)) * log(aleatorio());
                soma_tempo_servico += servico - tempo_decorrido;
            }
            fila++;
            max_fila = fila > max_fila ? fila : max_fila;

            chegada = tempo_decorrido + (-1.0 / (1.0 / intervalo_media_chegada)) * log(aleatorio());
        }
        // saída
        else
        {
            //printf("Saída em %lF.\n", tempo_decorrido);
            fila--;

            if (fila)
            {
                servico = tempo_decorrido + (-1.0 / (1.0 / tempo_medio_servico)) * log(aleatorio());
                soma_tempo_servico += servico - tempo_decorrido;
            }
        }
    }
    printf("Ocupação: %lF\n", soma_tempo_servico / maximo(tempo_decorrido, servico));
    printf("Max Fila: %d\n", max_fila);
    return 0;
}