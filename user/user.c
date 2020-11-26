#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "user.h"

void converteParaHEXA(char *string, char hexa[]);
void converteParaASCII(char *string, char ascii[]);
int lerArquivo();
int escreverArquivo();

int main ()
{
	int opc;
    int ret=0;
	
    do
    {
        printf("Digite 1 para escrever o dado criptografado no arquivo\n");
        printf("Digite 2 para ler e descriptografar o dado do arquivo\n");
        printf("Digite 3 para encerrar o programa\n");
        scanf("%i", &opc);
        switch (opc){
        case 1:
            ret = escreverArquivo();
            break;
        case 2:
	    ret = lerArquivo();
            break;
        case 3:
            break;
        
        default:
            printf("OPCAO INEXISTENTE\n");
            printf("\n");
            break;
        }
    } while (opc != 3);
    return ret;
}

int escreverArquivo()
{
    system("clear");
    int fd;
    char bufascii[128], bufhexa[256], criptografado[128];
    int ret=0, tamFile1=0;
    char *c; 

    fd = open("arquivo.txt", O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if (fd < 0) 
    	{
           perror("Falha ao abrir arquivo...");
       	   return errno;
	}

    printf("Digite o dado que deseja armazenar criptografado: ");
	
    scanf("%s", bufascii);
    converteParaHEXA(bufascii, bufhexa);
	ret = syscall(548, fd, bufhexa, (strlen(bufascii) * 2));

    if(ret== -1){
        perror("Falha ao criptografar... ");
        return ret;
    }
    sync();
	close(fd);

    fd = open("arquivo.txt", O_RDONLY);

    FILE *stream = fopen("arquivo.txt","r");
    fseek(stream,0,SEEK_END);
    tamFile1 = ftell(stream);
    fseek(stream,0,SEEK_SET);

    c = (char *) calloc(tamFile1, sizeof(char));
    read(fd, c, tamFile1);
    close(fd);
	fclose(stream);
	
    printf("\nCriptografando e escrevendo ...\n");

    printf("\n----------------------------------------------------\n");
    printf("####################################################");
    printf("\n----------------------------------------------------\n");

    printf("Dado armazenado no arquivo: ");
    printf("%s \n", c);
    
    printf("----------------------------------------------------\n");
    printf("####################################################");
    printf("\n----------------------------------------------------\n");
    printf("\n\n\n\n\n");
    return 0;
}

int lerArquivo()
{
    int fd;
    int tamFile=0, ret;
    char *c, *ascii;

	system("clear");
    
    fd = open("arquivo.txt", O_RDONLY);
	if (fd < 0) {
		perror("Falha ao abrir arquivo...");    
	}

    FILE *stream = fopen("arquivo.txt","r");
    fseek(stream,0,SEEK_END);
    tamFile = ftell(stream);
    fseek(stream,0,SEEK_SET);

    c = (char *) calloc(tamFile, sizeof(char));

    printf("\nDescriptografando ...");

    printf("\n----------------------------------------------------\n");
    printf("####################################################");
    printf("\n----------------------------------------------------\n");
    printf("Dado decriptado em hexa: ");
	ret = syscall(549, fd, c, tamFile);
 	     
     if(ret == -1){
         perror("Falha na descriptografia... ");
         return ret;
     }
    printf("%s \n",c);
	       
    printf("Dado decriptado em ASCII: ");
    ascii = malloc(tamFile / 2);
	converteParaASCII(c, ascii); 
	printf("%s \n",ascii);

    printf("----------------------------------------------------\n");
    printf("####################################################");
    printf("\n----------------------------------------------------\n");


    free(ascii);
    close(fd);
    fclose(stream);
    printf("\n\n\n\n\n");
    return 0;
}

void converteParaHEXA(char *string, char hexa[])
{
    int i;
    int tam = strlen(string);

    for(i = 0; i < tam; i++)
    {        
        sprintf(hexa+i*2,"%02x", string[i]);
    }
    sprintf(hexa+i*2+1,"%c",'\0'); 
}

void converteParaASCII(char *string, char ascii[])
{ 
    int i;    
    int cont = 0;
    int tam = strlen(string);

    char temp[2];
    
    for(i = 0; i < tam; i+=2)
    {         
        temp[0]  = string[i];
        temp[1]  = string[i+1];
        sscanf(temp, "%hhx", &ascii[cont]);
        cont++;    
   }
}

