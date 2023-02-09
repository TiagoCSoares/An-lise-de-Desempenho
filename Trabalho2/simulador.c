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
    double u = rand() / ((double)RAND_MAX + 3);
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
    double num_aleatorio = 0.0;
    double lakaka = 0.0;
    int pacote550 = 0;
    int pacote40 = 0;
    int pacote1500 = 0;
// taxa de chegada = 0.01
// tempo de atendimento = L/R
// L1 = 550 bytes
// L2 = 40 bytes
// L3 = 1500 bytes
// taxa de transmissão média = (0.5*550 + 0.4*40 + 0.1*1500) = 44100 bytes/segundo
// tamanho do link(ocupação 60%) = taxa de transmissão / ocupação = 44100 / 0.6 = 73500 bytes/segundo
// tamanho do link(ocupação 80%) = taxa de transmissão / ocupação = 44100 / 0.8 = 55125 bytes/segundo
// tamanho do link(ocupação 95%) = taxa de transmissão / ocupação = 44100 / 0.95 = 46421 bytes/segundo
// tamanho do link(ocupação 99%) = taxa de transmissão / ocupação = 44100 / 0.99 = 44545 bytes/segundo
// Ocupação 60% = 735 bytes/segunds
// ocupação = taxa media de chegada / tempo medio de serviço
// 60/100 = 73500 / tempo medio de srviço -> 122500
// tempo de atendimento = L/R = 550 / 122500 = 0,007483 segundos
// tempo de atendimento = L/R = 40 / 122500 = 0,0005442 segundos
// tempo de atendimento = L/R = 1500 / 122500 = 0,020408 segundos
// Ocupação 80% = 55125 bytes/segunds
// ocupação = taxa media de chegada / tempo medio de serviço
// 80/100 = 55125 / tempo medio de srviço -> 68906,25
// tempo de atendimento = L/R = 550 / 68906,25 = 0,009977 segundos
// tempo de atendimento = L/R = 40 / 68906,25 = 0,000726 segundos
// tempo de atendimento = L/R = 1500 / 68906,25 = 0,027211 segundos
// Ocupação 95% = 46421 bytes/segunds
// ocupação = taxa media de chegada / tempo medio de serviço
// 95/100 = 46421 / tempo medio de srviço -> 48864,21
// tempo de atendimento = L/R = 550 / 464,21 = 0,010771 segundos
// tempo de atendimento = L/R = 40 / 464,21 = 0,000862 segundos
// tempo de atendimento = L/R = 1500 / 464,21 = 0,032313 segundos
// Ocupação 99% = 445,45 bytes/segunds
// tempo de atendimento = L/R = 550 / 445,45 = 0,012347 segundos
// tempo de atendimento = L/R = 40 / 445,45 = 0,000898 segundos
// tempo de atendimento = L/R = 1500 / 445,45 = 0,033674 segundos


    double tempos_medios_servicos[4][3] = {{550.0/73500.0, 40.0/73500.0, 1500.0/73500.0}, {550.0/55125.0, 40.0/55125.0, 1500.0/55125.0}, {550.0/46421.0, 40.0/46421.0, 1500.0/46421.0}, {550.0/44545.0, 40.0/44545.0, 1500.0/44545.0}};
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
        tempo_decorrido = 0.0;
        tempo_simulacao = 36000;
        num_aleatorio = aleatorio();
        chegada = (-1.0 / (1.0 / intervalo_media_chegada)) *(log(num_aleatorio));
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
                    num_aleatorio = aleatorio();
                    if (num_aleatorio < 0.5)
                    {
                        tempo_medio_servico = tempos_medios_servicos[i][0];
                    }
                    else if (num_aleatorio >= 0.5 && num_aleatorio < 0.9)
                    {
                        tempo_medio_servico = tempos_medios_servicos[i][1];
                    }
                    else if (num_aleatorio >= 0.9)
                    {
                        tempo_medio_servico = tempos_medios_servicos[i][2];
                    }
                    servico = tempo_decorrido + tempo_medio_servico;
                    soma_tempo_servico += servico - tempo_decorrido;
                }
                fila++;
                max_fila = fila > max_fila ? fila : max_fila;
                num_aleatorio = aleatorio();
                /*if (num_aleatorio < 0.5)
                {
                    tempo_medio_servico = tempos_medios_servicos[i][0];
                }
                else if (num_aleatorio >= 0.5 && num_aleatorio < 0.9)
                {
                    tempo_medio_servico = tempos_medios_servicos[i][1];
                }
                else if (num_aleatorio >= 0.9)
                {
                    tempo_medio_servico = tempos_medios_servicos[i][2];
                }*/
                chegada = tempo_decorrido + (-1.0 / (1.0 / intervalo_media_chegada)) * (log(num_aleatorio));
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
                    num_aleatorio = aleatorio();
                    if (num_aleatorio < 0.5)
                    {
                        tempo_medio_servico = tempos_medios_servicos[i][0];
                        pacote550++;
                    }
                    else if (num_aleatorio >= 0.5 && num_aleatorio < 0.9)
                    {
                        tempo_medio_servico = tempos_medios_servicos[i][1];
                        pacote40++;
                    }
                    else if (num_aleatorio >= 0.9)
                    {
                        tempo_medio_servico = tempos_medios_servicos[i][2];
                        pacote1500++;
                    }
                    servico = tempo_decorrido + tempo_medio_servico;
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
    
    /*printf("pacote550 %d\n", pacote550);
    printf("pacote40 %d\n", pacote40);
    printf("pacote1500 %d\n", pacote1500);*/
    return 0;
}