/**************************************************************************
*                                                                         *
*           Universidade Federal do Maranhão                              *
*       Departamento de Engenharia da Computação                          *
*                                                                         *
*  Author: Prof. Dr. Thales Levi Azevedo Valente                           *
*                                                                         *
*  Description: Processamento de Imagens em C                              *
*                                                                         *
*  Date: 27-11-2023                                                       *
*                                                                         *
* Este material fornece um programa simples para processamento de imagens *
* em C, incluindo filtros como negativo, escala de cinza, blur e Sobel.    *
*                                                                         *
* Conteúdos do Material:                                                  *
*   1. Carregamento de uma imagem em C                                    *
*   2. Aplicação de filtros: negativo, escala de cinza, blur e Sobel       *
*   3. Salvamento da imagem processada                                    *
*   4. Manipulação de Dados e Ponteiros                                   *
*   5. Criação de Menu                                                   *
*                                                                         *
***************************************************************************
* -------------------------------------------------------------------------*
*   IMPORTANTE:                                                           *
*   1- NÃO modifique a assinatura das funções (tipo retorno, nome, parâm.)*
*   2- Apenas implemente o corpo das funções trocando o ; por chaves      *
*   3- NÃO modifique a main                                               *
*   4- Veja o arquivo "exemplo.c"                                         *
*   5- NOME DO ARQUIVO É "NOME_SOBRENOME1_SOBRENOME2"                     *
* -------------------------------------------------------------------------*
*                                                                         *
* Inicie o código abaixo para começar a exploração.                       *
*                                                                         *
***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

void applyNegativeFilter(unsigned char *img, int width, int height, int channels);
void applyGrayScaleFilter(unsigned char *img, int width, int height, int channels);
void applyBlurFilter(unsigned char *img, int width, int height, int channels);
void applySobelFilter(unsigned char *img, int width, int height, int channels);
void saveImage(const unsigned char *img, int width, int height, int channels, const char *outputFileName);

int main() {
    int width, height, channels;
    char inputFile[100] = "D:\\trabalho c\\input.jpg";
    char outputFileName[100];

while(1){
    // Carregar a imagem
    unsigned char *img = stbi_load(inputFile, &width, &height, &channels, 0);
    if (img == NULL) {
        fprintf(stderr, "Erro ao carregar a imagem.\n");
        return 1;
    }

    int choice;
    printf("\n Escolha o filtro a ser aplicado:\n");
    printf("1: Negativo\n");
    printf("2: Escala de Cinza\n");
    printf("3: Blur\n");
    printf("4: Sobel\n");
    printf("5: Deseja sair? Aperte 5\n");
    printf("Digite sua escolha: ");
    scanf("%d", &choice);

    switch(choice) {
        case 1:
            applyNegativeFilter(img, width, height, channels);
            strcpy(outputFileName, "negativo.jpg");
            break;
        case 2:
            applyGrayScaleFilter(img, width, height, channels);
            strcpy(outputFileName, "escala_de_cinza.jpg");
            break;
        case 3:
            applyBlurFilter(img, width, height, channels);
            strcpy(outputFileName, "blur.jpg");
            break;
        case 4:
            applyGrayScaleFilter(img, width, height, channels); // Para Sobel, primeiro converte para escala de cinza
            applySobelFilter(img, width, height, channels);
            strcpy(outputFileName, "sobel.jpg");
            break;
        case 5: // sair do loop
            printf("Obrigado por usar o programa!\n");
            stbi_image_free(img);
            return 0;
        default:
            printf("Escolha inválida!\n");
            stbi_image_free(img);
            continue;
    }

    // Salvar a imagem processada com o nome apropriado
    saveImage(img, width, height, channels, outputFileName);

    // Liberar memória
    stbi_image_free(img);
}
    return 0;
}

void applySobelFilter(unsigned char *img, int width, int height, int channels) {
    // Aqui, você precisa implementar a lógica para aplicar o filtro Sobel em X e Y
    // e, em seguida, combinar os resultados usando a distância euclidiana.
    // Esta é uma tarefa avançada que requer manipulação detalhada dos pixels e do buffer de imagem.

int soma1; // Declaração de variáveis para armazenar as somas dos produtos com os kernels Sobel
int soma2;
float kernelx[3][3]={{ -1,  0,  1 },//inicializando o kernel responsável por Sobelizar na horizontal
                  { -2,  0,  2 },
                  { -1,  0,  1 }};
float kernely[3][3]={{ -1,  -2,  -1 },//inicializando o kernel responsável por Sobelizar na vertical
                  { 0,  0,  0 },
                  { 1,  2,  1 }};
    //Mesma coisa do Blur, por enquanto, tirando q o kernel eh diferente
     unsigned char *temp = malloc(width * height * 3); //iniciando a imagem temporária, para ser aplicada no processo de soma.
     unsigned char *temp2 = malloc(width * height * 3); //iniciando a imagem temporária, para ser aplicada no processo de sobel

    for (int i = 0; i < width * height * 3; ++i) {
        temp[i] = img[i]; // torna temporário = Imagem original
    }
    for (int i = 0; i < width * height * 3; ++i) {
        temp2[i] = img[i]; // torna temporário 2 = Imagem original
    }

     for (int i = 1; i < height - 1; ++i) { //loop para percorrer os pixels da imagem, excluindo a borda
        for (int j = 1; j < width - 1; ++j) {
        for (int k = 0; k < 3; ++k) {// loop pra percorrer os canais RGB
             soma1 = 0;
                soma2 = 0;
            for (int di = -1; di <= 1; ++di) { // Aplicação dos kernels Sobel horizontal e vertical em torno do pixel atual
                for (int dj = -1; dj <= 1; ++dj) {// processo de obtenção da soma e  aplicação do Kernel horizontal na soma, aplicando a parte horizontal do Sobel
                        soma1 +=  kernelx[dj+1][di+1] * temp[((i + di)* width + (j + dj)) * 3 + k];
                        soma2 +=  kernely[dj+1][di+1] * temp[((i + di)* width + (j + dj)) * 3 + k];
        }
        }
       // Calcular a magnitude da borda
                int magnitude = (int)sqrt((soma1 * soma1) + (soma2 * soma2));

// limitar a magnitude a 255 ( valor máximo do byte)
if(magnitude>255){
    magnitude = 255;
}
                // Atribuir o valor na imagem temporaria
                temp2[(i * width + j) * 3 + k] = (unsigned char)magnitude;
img[(i * width + j ) * 3 + k] = temp2[(i * width + j) * 3 + k]; // atribui o valor guardado na imagem temporária na imagem original.
        }
     }



}
}


// Implementação das outras funções de filtro...1
// ...

void saveImage(const unsigned char *img, int width, int height, int channels, const char *outputFileName) {
    char outputPath[200];
    sprintf(outputPath, "D:\\\\trabalho c\\\\%s", outputFileName); // Constrói o caminho no diretório atual
    stbi_write_jpg(outputPath, width, height, channels, img, 100);
    printf("%s", outputPath);
}


void applyNegativeFilter(unsigned char *img, int width, int height, int channels) {
    // Laço pra percorrer a imagem byte por byte
    for (int i=0; i<height*width*channels; i++){
// Pegar o byte presente e substituir pelo byte "negativo" dele
            img[i]=255-img[i]; // aplica a operação de filtro negativo em cada píxel
        }

    // Implementação do Filtro Negativo
}

void applyGrayScaleFilter(unsigned char *img, int width, int height, int channels) {
    // Implementação do filtro de escala de cinza
    // Gray = 0.3*R + 0.59*G + 0.11*B.


//Transformar tudo em cinza
    for ( int k=0; k<width*height*channels; k+= channels){ // percorrer cada píxel, para que a fórmula de cinza receba o valor da imagem toda
    unsigned char Cinza = (unsigned char) (0.3*img[k] + 0.59*img[k+1] +  0.11*img[k+2]); ////Formula de conversão RGB para Cinza
   //Aplicar a transformação cinza nas cores RGB da imagem
    img[k]=Cinza; //Aplicando para o canal RED
    img[k+1]=Cinza;//Aplicando para o canal GREEN
    img[k+2]=Cinza;//Aplicando para o canal BLUE;
    }


   // img[k] = (img[k] + img [k+ 1] + img [k+ 2])/3; - > ESSE DEU CERTO Forma alternativa
    }


void applyBlurFilter(unsigned char *img, int width, int height, int channels) {
    // Implementação do filtro de blur


float v=1.0 / 9; // valor  do elemento do kernel, usado para a operação de borramento
float kernel[3][3]={{v,v,v},				  //inicializando o kernel responsável por borrar
		   {v,v,v},
		   {v,v,v}};

    unsigned char *temp = malloc(width * height * 3); //iniciando a imagem temporária, para ser aplicada no processo de soma.

   // Copia a imagem original para a temporária
    memcpy(temp, img, width * height * channels);

    for (int i = 1; i < height - 1; ++i) {//loop para percorrer os pixels da imagem, excluindo a borda
        for (int j = 1; j < width - 1; ++j) {
            for (int k = 0; k < 3; ++k) {// loop pra percorrer os canais RGB
                float soma = 0; //Declaração de variável para armazenar a soma  para cada pixel.
                for (int di = -1; di <= 1; ++di) {//Loop para percorrer a vizinhança do pixel atual e aplicar o kernel
                    for (int dj = -1; dj <= 1; ++dj) {// processo de obtenção da soma e  aplicação do Kernel na soma, borrando a "imagem temporaria"
                        soma +=  kernel[dj+1][di+1] * temp[((i + di) * width + (j + dj)) * 3 +k ];

                    }
                }// Criando a imagem toda borrada, atribuindo o valor da soma a imagem original.
                  img[(i * width + j ) * 3 +k ] = (unsigned char)round(soma);

            }

        }
    }
    /*for(int k = (width * channels) + channels ; k != width*height*channels; k = k + channels ){ - > Outra forma de fazer.

    	if(k >= (width * channels) + channels  && k < (width*height*channels) - 1498){
    		if(k % 1500 != 1500 - channels && k % 1500 != 0 ){
    			img[k] = (img[k - channels - 1500] + img[k - 1500] + img[k + channels - 1500] + img[k - channels ] + img[k] + img[k + channels ] + img[k - channels + 1500] + img[k + 1500] + img[k + channels  + 1500])/9;
    			img[k+1] = (img[k+1 - channels - 1500] + img[k+1 - 1500] + img[k+1 + channels - 1500] + img[k+1 - channels ] + img[k+1] + img[k+1 + channels ] + img[k+1 - channels + 1500] + img[k+1 + 1500] + img[k+1 + channels  + 1500])/9;
    			img[k+2] = (img[k+2 - channels - 1500] + img[k+2 - 1500] + img[k+2 + channels - 1500] + img[k+2 - channels ] + img[k+2] + img[k+2 + channels ] + img[k+2 - channels + 1500] + img[k+2 + 1500] + img[k+2 + channels  + 1500])/9;
			}
		}
	}*/

}

