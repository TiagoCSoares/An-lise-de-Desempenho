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
    double tempo_simulacao = 36000;
    double intervalo_media_chegada = 0.2;

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

    //Cálculo para descobrir o tempo médio de serviço
    // (0.2 / x) = (80 / 100)  ----> x = 0.16
    // (0.2 / x) = (90 / 100)  ----> x = 0.18
    // (0.2 / x) = (95 / 100)  ----> x = 0.19
    // (0.2 / x) = (99 / 100)  ----> x = 0.198
    double tempos_medios_servicos[4] = {0.16, 0.18, 0.19, 0.198};

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

    for (int i = 0; i < 4; i++)
    {   
        //zera as variáveis para cada cenário
        tempo_medio_servico = tempos_medios_servicos[i];
        tempo_decorrido = 0.0;
        tempo_simulacao = 36000;
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