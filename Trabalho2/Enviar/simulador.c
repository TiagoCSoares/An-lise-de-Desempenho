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
    // taxa de transmissão média = (0.5*550 + 0.4*40 + 0.1*1500)*100 = 44100 bytes/segundo
    // tamanho do link(ocupação 60%) = taxa de transmissão / ocupação = 44100 / 0.6 = 73500 bytes/segundo
    // tamanho do link(ocupação 80%) = taxa de transmissão / ocupação = 44100 / 0.8 = 55125 bytes/segundo
    // tamanho do link(ocupação 95%) = taxa de transmissão / ocupação = 44100 / 0.95 = 46421 bytes/segundo
    // tamanho do link(ocupação 99%) = taxa de transmissão / ocupação = 44100 / 0.99 = 44545 bytes/segundo
    // Ocupação 60% = 73500 bytes/segunds
    // tempo de atendimento = L/R = 550 / 73500 =  0.007482993197278911 segundos
    // tempo de atendimento = L/R = 40 / 73500 =  0.0005442176870748299 segundos
    // tempo de atendimento = L/R = 1500 / 73500 = 0.02040816326530612  segundos
    // Ocupação 80% = 55125 bytes/segunds
    // tempo de atendimento = L/R = 550 / 55125 = 0.009977324263038548  segundos
    // tempo de atendimento = L/R = 40 / 55125 = 0.0007256235827664399 segundos
    // tempo de atendimento = L/R = 1500 / 55125 = 0.027210884353741496 segundos
    // Ocupação 95% = 46421 bytes/segunds
    // tempo de atendimento = L/R = 550 / 46421 = 0.011848085995562353 segundos
    // tempo de atendimento = L/R = 40 / 46421 = 0.0008616789814954439 segundos
    // tempo de atendimento = L/R = 1500 / 46421 = 0.03231296180607914 segundos
    // Ocupação 99% = 44545 bytes/segunds
    // tempo de atendimento = L/R = 550 / 44545 = 0.012347064765967 segundos
    // tempo de atendimento = L/R = 40 / 44545 = 0.012347064765967 segundos
    // tempo de atendimento = L/R = 1500 / 44545 = 0.033673812998091815 segundos


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
                    // Verifica qual o tamanho do pacote e atribui o tempo médio de serviço correspondente
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
                fila--;
                if (fila)
                {
                    // Verifica qual o tamanho do pacote e atribui o tempo médio de serviço correspondente
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
                //Para exibir os resultados finais retirar o comentário da linha abaixo
                //printf("%d\t%lF\t%lF\t%lF\t%.20lF\n", coleta_dados, soma_tempo_servico / maximo(tempo_decorrido, servico), e_n_final, e_w_final, erro_little );
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
    
    // Exibe a porcentagem de pacotes de cada tamanho
    /*float total_pacotes = pacote550 + pacote40 + pacote1500;
    printf("A porcentam de pacotes de tamanho 550 Bytes foi igual a: = %lF\n", pacote550 / total_pacotes);
    printf("A porcentam de pacotes de tamanho 40 Bytes foi igual a: = %lF\n", pacote40 / total_pacotes);
    printf("A porcentam de pacotes de tamanho 1500 Bytes foi igual a: = %lF\n", pacote1500 / total_pacotes);*/
    return 0;
}