#include<stdio.h>
#include<stdlib.h>
#include<math.h>


double aleatorio() {
	double u = rand() / ((double) RAND_MAX + 1);
	//limitando entre (0,1]
	u = 1.0 - u;

	return (u);
}

int minimo(int num1, int num2){
    if(num1 < num2){
        return num1;
    }
    return num2;
}


int main() {
    double tempo_simulacao;
    double tempo_decorrido = 0.0;

    double intervalo_media_chegada;
    double tempo_medio_servico;

    double chegada;
    double servico;

    unsigned long int fila = 0;

    printf("Informe o tempo de simulação (segundos): ");
    scanf("%lF", &tempo_simulacao);

    printf("Informe o intervalo médio entre chegadas (segundos): ");
    scanf("%lF", &intervalo_media_chegada);

    printf("Informe o tempo médio de serviço (segundos): ");
    scanf("%lF", &tempo_medio_servico);

    //gerando o tempo de chegada da primeira requisição.
    chegada = (-1.0 / (1.0/intervalo_media_chegada)) * log(aleatorio());

    while(tempo_decorrido <= tempo_simulacao){
        tempo_decorrido = !fila ? chegada : minimo(chegada, servico);

        if(tempo_decorrido == chegada){
            printf("Chegada em %lF.\n", tempo_decorrido);
            if(!fila){
                servico = tempo_decorrido + (-1.0 / (1.0/intervalo_media_chegada)) * log(aleatorio());
            }
            fila++;
            chegada = tempo_decorrido + (-1.0 / (1.0/intervalo_media_chegada)) * log(aleatorio());
            }
        else{
            printf("Saída em %lF.\n", tempo_decorrido);
            fila--;

            if(fila){
                servico = tempo_decorrido + (-1.0 / (1.0/intervalo_media_chegada)) * log(aleatorio());
            }
        }
    }
    return 0;
}
