#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef FMACROS // garante que macros.h não seja reincluída
   #include "macros.h"
#endif
///
#ifndef FTYPES // garante que types.h não seja reincluída
  #include "types.h"
#endif

#include "misc.h"
#include "dictionary.h"

// INICIALIZACAO DO BUFFER
tp_buffer * initbuffer(){

    tp_buffer *bp = (tp_buffer*)malloc(sizeof(tp_buffer)*PAGES);
    memset(bp, 0, sizeof(tp_buffer)*PAGES);

    int i;
    tp_buffer *temp = bp;

    if(bp == NULL)
        return ALLOCATION_ERROR;
    for (i = 0;i < PAGES; i++){
        temp->db=0;
        temp->pc=0;
        temp->nrec=0;
        temp++;
    }

    return bp;
}
//// imprime os dados no buffer (deprecated?)
int printbufferpoll(tp_buffer *buffpoll, tp_table *s,struct fs_objects objeto, int num_page){

    int aux, i, num_reg = objeto.qtdCampos;


    if(buffpoll[num_page].nrec == 0){
        return PRINT_ERROR;
    }

    i = aux = 0;

    aux = header(s, num_reg);


    while(i < buffpoll[num_page].nrec){ // Enquanto i < numero de registros * tamanho de uma instancia da tabela
        drawline(buffpoll, s, objeto, i, num_page);
        i++;
    }
    return SUCCESS;
}

// RETORNA PAGINA DO BUFFER
column * getPage(tp_buffer *buffer, tp_table *campos, struct fs_objects objeto, int page){

    if(page >= PAGES)
        return INVALID_PAGE;

    if(buffer[page].nrec == 0) //Essa página não possui registros
        return PARAMETER_ERROR_2;

    column *colunas = (column *)malloc(sizeof(column)*objeto.qtdCampos*(buffer[page].nrec)); //Aloca a quantidade de campos necessária

    if(!colunas)
        return ALLOCATION_ERROR;

    memset(colunas, 0, sizeof(column)*objeto.qtdCampos*(buffer[page].nrec));

    int i=0, j=0, t=0, h=0;

    if (!buffer[page].position)
        return colunas;

    while(i < buffer[page].position){
        t=0;
        if(j >= objeto.qtdCampos)
            j=0;

        colunas[h].valorCampo = (char *)malloc(sizeof(char)*campos[j].tam+1);
        memset(colunas[h].valorCampo, '\0', campos[j].tam+1);
        colunas[h].tipoCampo = campos[j].tipo;  //Guarda tipo do campo

        strcpy(colunas[h].nomeCampo, campos[j].nome); //Guarda nome do campo

        while(t < campos[j].tam){
            colunas[h].valorCampo[t] = buffer[page].data[i]; //Copia os dados
            t++;
            i++;
        }
        colunas[h].valorCampo[t] = '\0';

        h++;
        j++;
    }

    return colunas; //Retorna a 'page' do buffer
}
// EXCLUIR TUPLA BUFFER
column * excluirTuplaBuffer(tp_buffer *buffer, tp_table *campos, struct fs_objects objeto, int page, int nTupla){
    column *colunas = (column *)malloc(sizeof(column)*objeto.qtdCampos);

    if(colunas == NULL)
        return ALLOCATION_ERROR;

    if(buffer[page].nrec == 0) //Essa página não possui registros
        return PARAMETER_ERROR_2;

    int i, tamTpl = tupleSize(campos, objeto), j=0, t=0;
    i = tamTpl*nTupla; //Calcula onde começa o registro

    while(i < tamTpl*nTupla+tamTpl){
        t=0;

        colunas[j].valorCampo = (char *)malloc(sizeof(char)*campos[j].tam); //Aloca a quantidade necessária para cada campo
        colunas[j].tipoCampo = campos[j].tipo;  // Guarda o tipo do campo
        strcpylower(colunas[j].nomeCampo, campos[j].nome);   //Guarda o nome do campo

        while(t < campos[j].tam){
            colunas[j].valorCampo[t] = buffer[page].data[i];    //Copia os dados
            t++;
            i++;
        }
        j++;
    }
    j = i;
    i = tamTpl*nTupla;
    for(; i < buffer[page].position; i++, j++) //Desloca os bytes do buffer sobre a tupla excluida
        buffer[page].data[i] = buffer[page].data[j];

    buffer[page].position -= tamTpl;
    buffer[page].nrec--;

    return colunas; //Retorna a tupla excluida do buffer
}
// INSERE UMA TUPLA NO BUFFER!
char *getTupla(tp_table *campos,struct fs_objects objeto, int from){ //Pega uma tupla do disco a partir do valor de from

    int tamTpl = tupleSize(campos, objeto);
    char *linha=(char *)malloc(sizeof(char)*tamTpl);
    memset(linha, '\0', tamTpl);

    FILE *dados;

    from = from * tamTpl;

	char directory[LEN_DB_NAME*2];
    strcpy(directory, connected.db_directory);
    strcat(directory, objeto.nArquivo);

    dados = fopen(directory, "r");

    if (dados == NULL) {
        free(linha);
        return READING_ERROR;
    }

    fseek(dados, from, 1);
    if(fgetc (dados) != EOF){
        fseek(dados, -1, 1);
        fread(linha, sizeof(char), tamTpl, dados); //Traz a tupla inteira do arquivo
    } else {       //Caso em que o from possui uma valor inválido para o arquivo de dados
        fclose(dados);
        free(linha);
        return READING_ERROR;
    }

    fclose(dados);
    return linha;
}
/////
void setTupla(tp_buffer *buffer,char *tupla, int tam, int pos) { //Coloca uma tupla de tamanho "tam" no buffer e na página "pos"
    int i = buffer[pos].position;

    for (; i < buffer[pos].position + tam; i++)
        buffer[pos].data[i] = *(tupla++);
}
//// insere uma tupla no buffer
int colocaTuplaBuffer(tp_buffer *buffer, int from, tp_table *campos, struct fs_objects objeto){//Define a página que será incluida uma nova tupla

    char *tupla = getTupla(campos,objeto,from);

    if(tupla == READING_ERROR) {
        free(tupla);
        return READING_DATA_ERROR;
    }

    int i=0, found=0;
    while (!found && i < PAGES) {//Procura pagina com espaço para a tupla.

        if(SIZE - buffer[i].position > tupleSize(campos, objeto)) {// Se na pagina i do buffer tiver espaço para a tupla, coloca tupla.
            setTupla(buffer, tupla, tupleSize(campos, objeto), i);
            found = 1;
            buffer[i].position += tupleSize(campos, objeto); // Atualiza proxima posição vaga dentro da pagina.
            buffer[i].nrec += 1;
            break;
        }
        i++;// Se não, passa pra proxima página do buffer.
    }

    if (!found) {
        free(tupla);
        return BUFFER_ALREADY_FULL;
    }


    free(tupla);
    return SUCCESS;
}
////////

void createField(int tam, int header, char *val, int x) {
    int i;
    char aux[30];

    if(header) {
        for(i = 0; i <= 30 && val[i] != '\0'; i++){
            aux[i] = val[i];
        }
        for(;i<30;i++)
            aux[i] = ' ';
        aux[i] ='\0';
        printf("%s", aux);

    } else {
        for(i = 0; i < x; i++)
            printf(" ");
    }
}
