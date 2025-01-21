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
    char inputFile[100] = "C:\\Users\\nickf\\Downloads\\FinalLab\\input.jpg";
    char outputFileName[100];

    // Carregar a imagem
    unsigned char *img = stbi_load(inputFile, &width, &height, &channels, 0);
    if (img == NULL) {
        fprintf(stderr, "Erro ao carregar a imagem.\n");
        return 1;
    }

    int choice;
    printf("Escolha o filtro a ser aplicado:\n");
    printf("1: Negativo\n");
    printf("2: Escala de Cinza\n");
    printf("3: Blur\n");
    printf("4: Sobel\n");
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
            applyGrayScaleFilter(img, width, height, channels); 
          // Para Sobel, primeiro converte para escala de cinza
            applySobelFilter(img, width, height, channels);
            strcpy(outputFileName, "sobel.jpg");
            break;
        default:
            printf("Escolha inválida!\n");
            stbi_image_free(img);
            return 1;
    }

    // Salvar a imagem processada com o nome apropriado
    saveImage(img, width, height, channels, outputFileName);

    // Liberar memória
    stbi_image_free(img);
    return 0;
}

void applySobelFilter(unsigned char *img, int width, int height, int channels) {
    // Aqui, você precisa implementar a lógica para aplicar o filtro Sobel em X e Y
    // e, em seguida, combinar os resultados usando a distância euclidiana.
    // Esta é uma tarefa avançada que requer manipulação detalhada dos pixels e do buffer de imagem.0

  // Matrizes que serão aplicadas
  int kernelH[3][3] = {
      {-1, 0, 1},
      {-2, 0, 2},
      {-1, 0, 1}
  };

  int kernelV[3][3] = {
      {-1, -2, -1},
      {0, 0, 0},
      {1, 2, 1}
  };

  // Criando imagem temporária
  unsigned char *tempImg = (unsigned char *)malloc(width * height * channels);
  if (tempImg == NULL) {
      return;
  }

  // Lendo a imagem ignorando as bordas
  for (int y = 1; y < height - 1; y++) {
      for (int x = 1; x < width - 1; x++) {
          // Percorrendo cada canal da imagem
          for (int c = 0; c < channels; c++) {
              // Inicializando variáveis que irão conter valor final
              int cnvX = 0, cnvY = 0;
              // Percorrendo uma quadra de pixels da imagem
              for (int i = -1; i <= 1; i++) {
                  for (int j = -1; j <= 1; j++) {
                      // Aplicando a máscara x na componente x da imagem
                      cnvX += img[((y + i) * width + (x + j)) * channels + c] * kernelH[i + 1][j + 1];
                      // Aplicando a máscara y na componente y da imagem
                      cnvY += img[((y + i) * width + (x + j)) * channels + c] * kernelV[i + 1][j + 1];
                  }
              }
              // Calculando a magnitude do gradiente combinando as componentes
              int magnitude = (int)sqrt(pow(cnvX, 2) + pow(cnvY, 2));

              // Aplicando a magnitude na imagem
              tempImg[(y * width + x) * channels + c] = (unsigned char)magnitude;
          }
      }
  }

  // Transferindo o resultado da imagem temporária para a imagem original
  memcpy(img, tempImg, width * height * channels);

  // Liberando a memória
  free(tempImg);

  }

// Implementação das outras funções de filtro...1
// ...

void saveImage(const unsigned char *img, int width, int height, int channels, const char *outputFileName) {
    char outputPath[200];
    sprintf(outputPath, "C:\\Users\\nickf\\Downloads\\FinalLab\\%s", outputFileName);
  // Constrói o caminho no diretório atual
    stbi_write_jpg(outputPath, width, height, channels, img, 100);
    printf("%s", outputPath);
}


void applyNegativeFilter(unsigned char *img, int width, int height, int channels) {
// Implementação do Filtro Negativo

// Laço para passar por toda a imagem, byte por byte
for (int i = 0; i < width * height * channels; i++) {
  // Pegando o valor do byte e subtraindo de 255
  img[i] = 255 - img[i];
}
}

void applyGrayScaleFilter(unsigned char *img, int width, int height, int channels) {
    // Implementação do filtro de escala de cinza

// Laço que percorre cada pixel da imagem
for (int i = 0; i < height; i++) {
  for (int j = 0; j < width; j++) {
    // passando pela imagem horizontalmente
    int index = (i * width + j) * channels;
    // Botando os parâmetros para escala de cinza
    unsigned char grayscale = (unsigned char)(0.3 * img[index] + 0.59 * img[index + 1] + 0.11 * img[index + 2]);
    // Botando a escala de cinza em cada canal
    img[index] = grayscale;
    img[index + 1] = grayscale;
    img[index + 2] = grayscale;
  }
}
}

void applyBlurFilter(unsigned char *img, int width, int height, int channels){
    // Implementação do filtro de blur

  // Criando uma imagem temporária para conter os pixels originais
  unsigned char *tempImg = (unsigned char *)malloc(width * height * channels);
  memcpy(tempImg, img, width * height * channels);

  // Parâmetro de desfoque
  int blur = 7;

  // Percorrendo a imagem
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      // Variáveis para acumular os valores ponderados de cada canal (RGB)
      float sumR = 0, sumG = 0, sumB = 0, cnt = 0;

      // Percorre uma vizinhança quadrada ao redor do pixel com base no fator de blur
      for (int dy = -blur; dy <= blur; dy++) {
        for (int dx = -blur; dx <= blur; dx++) {
          // Calcula o pixel da vizinhança com base na coordenada do pixel atual
          int nx = x + dx;
          int ny = y + dy;

          // Verifica se o pixel é válido
          if (ny >= 0 && nx >= 0 && ny < height && nx < width) {
            // Percorrendo horizontalmente os pixels da imagem temporária
            int index = (ny * width + nx) * channels;
            // Fazendo a soma do quadrante de um pixel
            sumR += tempImg[index];
            sumG += tempImg[index + 1];
            sumB += tempImg[index + 2];
            cnt++;
          }
        }
      }
      int index = (y * width + x) * channels;
      // Aplicando a média ponderada em cada canal
      img[index] = sumR / cnt;
      img[index + 1] = sumG / cnt;
      img[index + 2] = sumB / cnt;
    }
  }

  // Não se esqueça de liberar a memória da imagem temporária
  free(tempImg);

}
}
