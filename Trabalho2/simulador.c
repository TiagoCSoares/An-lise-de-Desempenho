#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

//Fórmula da ocupação:
//ocupacao = soma_tempo_servico / max(tempo_decorrido,servico)

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
    double tempo_simulacao = 3600;
    double intervalo_media_chegada = 0.01;

    double tempo_decorrido = 0.0;
    double tempo_anterior = 0.0;

    double tempo_medio_servico;

    double chegada;
    double servico;
    int coleta_dados;
    double erro_little;

    double soma_tempo_servico = 0.0;

    unsigned long int fila = 0;
    unsigned int max_fila = 0;

// taxa de chegada = 0.01
// tempo de atendimento = L/R
// L1 = 550 bytes
// L2 = 40 bytes
// L3 = 1500 bytes
// taxa de transmissão média = (0.5*550 + 0.4*40 + 0.1*1500) = 441 bytes/segundo
// tamanho do link(ocupação 60%) = taxa de transmissão / ocupação = 441 / 0.6 = 735 bytes/segundo
// tamanho do link(ocupação 80%) = taxa de transmissão / ocupação = 441 / 0.8 = 551,25 bytes/segundo
// tamanho do link(ocupação 95%) = taxa de transmissão / ocupação = 441 / 0.95 = 464,21 bytes/segundo
// tamanho do link(ocupação 99%) = taxa de transmissão / ocupação = 441 / 0.99 = 445,45 bytes/segundo
// Ocupação 60% = 735 bytes/segunds
// tempo de atendimento = L/R = 550 / 735 = 0,7483 segundos
// tempo de atendimento = L/R = 40 / 735 = 0,05442 segundos
// tempo de atendimento = L/R = 1500 / 735 = 2,0408 segundos
// Ocupação 80% = 551,25 bytes/segunds
// tempo de atendimento = L/R = 550 / 551,25 = 0,9977 segundos
// tempo de atendimento = L/R = 40 / 551,25 = 0,0726 segundos
// tempo de atendimento = L/R = 1500 / 551,25 = 2,7211 segundos
// Ocupação 95% = 464,21 bytes/segunds
// tempo de atendimento = L/R = 550 / 464,21 = 1,0771 segundos
// tempo de atendimento = L/R = 40 / 464,21 = 0,0862 segundos
// tempo de atendimento = L/R = 1500 / 464,21 = 3,2313 segundos
// Ocupação 99% = 445,45 bytes/segunds
// tempo de atendimento = L/R = 550 / 445,45 = 1,2347 segundos
// tempo de atendimento = L/R = 40 / 445,45 = 0,0898 segundos
// tempo de atendimento = L/R = 1500 / 445,45 = 3,3674 segundos


    double tempos_medios_servicos[12] = {0.7483, 0.05442, 2.0408, 0.9977, 0.0726, 2.7211, 1.0771, 0.0862, 3.2313, 1.2347, 0.0898, 3.3674};
    double ocupacoes[4] = {0.6, 0.8, 0.95, 0.99};

    double e_n_final;
    double e_w_final;
    double lambda;


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


    // gerando o tempo de chegada da primeira requisição.
    chegada = (-1.0 / (1.0 / intervalo_media_chegada)) * log(aleatorio());
    for (int a = 0; a < 4; a++)
    {
        for (int i = 0; i < 12; i++)
        {   
            //zera as variáveis para cada cenário
            tempo_medio_servico = tempos_medios_servicos[i];
            tempo_decorrido = 0.0;
            tempo_simulacao = 3600;
            chegada = (-1.0 / (1.0 / intervalo_media_chegada)) * log(aleatorio());
            coleta_dados = 100;
            tempo_anterior = 0.0;
            soma_tempo_servico = 0.0;
            max_fila = 0;
            fila = 0;
            e_n_final = 0.0;
            e_w_final = 0.0;
            lambda = 0.0;
            erro_little = 0.0;
            e_n.soma_areas = 0.0;
            e_n.tempo_anterior = 0.0;
            e_n.no_eventos = 0.0;
            e_w_chegada.soma_areas = 0.0;
            e_w_saida.soma_areas = 0.0;
            e_w_chegada.tempo_anterior = 0.0;
            e_w_saida.tempo_anterior = 0.0;
            e_w_chegada.no_eventos = 0.0;
            e_w_saida.no_eventos = 0.0;
            while (tempo_decorrido <= tempo_simulacao)
            {
                //Verificar o tempo mínimo entre chegada e saída e verificar se está em um tempo múltiplo de 100
                tempo_decorrido = !fila ? minimo(chegada, coleta_dados) : minimo(minimo(coleta_dados, chegada), servico);
                // chegada
                if (tempo_decorrido == chegada)
                {
                    if (!fila)
                    {
                        //printf("tempo_decorrido: %lF\n", tempo_decorrido);
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
                    e_n.soma_areas += (tempo_decorrido - e_n.tempo_anterior) * e_n.no_eventos;
                    e_w_chegada.soma_areas += (tempo_decorrido - e_w_chegada.tempo_anterior) * e_w_chegada.no_eventos;
                    e_w_saida.soma_areas += (tempo_decorrido - e_w_saida.tempo_anterior) * e_w_saida.no_eventos;

                    e_n_final = e_n.soma_areas / tempo_anterior;
                    e_w_final = (e_w_chegada.soma_areas - e_w_saida.soma_areas) / e_w_chegada.no_eventos;
                    lambda = e_w_chegada.no_eventos / tempo_anterior;

                    erro_little = (e_n_final - lambda * e_w_final) < 0 ? (e_n_final - lambda * e_w_final) * -1 : (e_n_final - lambda * e_w_final);
                    printf("%d\t%lF\t%lF\t%lF\t%.20lF\n", coleta_dados, soma_tempo_servico / maximo(tempo_decorrido, servico), e_n_final, e_w_final, erro_little );

                    coleta_dados += 100;
                    e_n.tempo_anterior = tempo_decorrido;
                    e_w_chegada.tempo_anterior = tempo_decorrido;
                    e_w_saida.tempo_anterior = tempo_decorrido;
                }

                //Printar os resultados finais de cada iteração
                if(tempo_decorrido == tempo_simulacao)
                {
                    e_n.soma_areas += (tempo_decorrido - e_n.tempo_anterior) * e_n.no_eventos;
                    e_w_chegada.soma_areas += (tempo_decorrido - e_w_chegada.tempo_anterior) * e_w_chegada.no_eventos;
                    e_w_saida.soma_areas += (tempo_decorrido - e_w_saida.tempo_anterior) * e_w_saida.no_eventos;

                    e_n_final = e_n.soma_areas / tempo_decorrido;
                    e_w_final = (e_w_chegada.soma_areas - e_w_saida.soma_areas) / e_w_chegada.no_eventos;
                    lambda = e_w_chegada.no_eventos / tempo_decorrido;
                    erro_little = (e_n_final - lambda * e_w_final) < 0 ? (e_n_final - lambda * e_w_final) * -1 : (e_n_final - lambda * e_w_final);
                    printf("%d\t%lF\t%lF\t%lF\t%.20lF\n", coleta_dados, soma_tempo_servico / maximo(tempo_decorrido, servico), e_n_final, e_w_final, erro_little );
                }
            }
        }   
    }
    e_n.soma_areas += (tempo_decorrido - e_n.tempo_anterior) * e_n.no_eventos;
    e_w_chegada.soma_areas += (tempo_decorrido - e_w_chegada.tempo_anterior) * e_w_chegada.no_eventos;
    e_w_saida.soma_areas += (tempo_decorrido - e_w_saida.tempo_anterior) * e_w_saida.no_eventos;

    e_n_final = e_n.soma_areas / tempo_decorrido;
    e_w_final = (e_w_chegada.soma_areas - e_w_saida.soma_areas) / e_w_chegada.no_eventos;
    lambda = e_w_chegada.no_eventos / tempo_decorrido;
    
    /*printf("E[N] = %lF\n", e_n_final);
    printf("E[W] = %lF\n", e_w_final);
    printf("Lambda = %lF\n\n", lambda);

    printf("Erro de Little: %.20lF\n\n", (e_n_final - lambda * e_w_final) < 0 ? (e_n_final - lambda * e_w_final) * -1 : (e_n_final - lambda * e_w_final));

    printf("Ocupação: %lF.\n", soma_tempo_servico / maximo(tempo_decorrido, servico));
    printf("Max Fila: %d\n", max_fila);

    printf("Tempo decorrido: %lF.\n", tempo_decorrido);*/
    return 0;
}