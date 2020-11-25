#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <chrono>
#include <thread>

using namespace std;
using namespace chrono;

int vtcpf [10006060][16];   // Vetor matriz de CPF
int vtcnpj [10006060][16];  // Vetor matriz de CNPJ
char buffer[16]; //buffer para auxiliar na leitura dos CNPJ`s e CPF`s
int cpf = 0;  // variavel contador de CPF`s
int cnpj = 0; // variavel contador de CNPJ`s


// funcao que calcula o digito verificador do CPFs
void calculoCPF(){

  // for para thread
  for(int xi = 0; xi < cpf; xi++){

    int v1 = 0; // variavel para o calculo do primeiro digito
    int v2 = 0; // variavel para o calculo do segundo digito

    /*calculo do primeiro digito, como minha logica esta pegando a partir da terceira posicoes 
    por causa dos caracteres de espaco no comeco das numeracoes do cpf.
    */
    v1 = ((vtcpf[xi][3]*10) + (vtcpf[xi][4]*9) + (vtcpf[xi][5]*8) +
    (vtcpf[xi][6]*7) + (vtcpf[xi][7]*6) + (vtcpf[xi][8]*5)+
    (vtcpf[xi][9]*4) + (vtcpf[xi][10]*3) + (vtcpf[xi][11]*2));

    v1 = v1%11;

    v1 = 11 - v1;

    if(v1 >= 10){
    v1 = 0;
    }

    /*calculo do primeiro digito, como minha logica esta pegando a partir da terceira posicoes 
    por causa dos caracteres de espaco no comeco das numeracoes do cpf.
    */
    v2 = ((vtcpf[xi][3]*11) + (vtcpf[xi][4]*10) + (vtcpf[xi][5]*9) +
    (vtcpf[xi][6]*8) + (vtcpf[xi][7]*7) + (vtcpf[xi][8]*6) +
    (vtcpf[xi][9]*5) + (vtcpf[xi][10]*4) + (vtcpf[xi][11]*3) + (v1*2));

    v2 = v2%11;

    v2 = 11 - v2;

    if(v2 >= 10){
        v2 = 0;
    }    

    vtcpf[xi][12] = v1; //valor do primeiro digito
    vtcpf[xi][13] = v2; //valor do segundo digito
    
//    printf("\n DV %d%d \n", v1,v2);
  }
}

// funcao que calcula os digitos verificadores do CNPJs
void calculoCNPJ(){

  // for para execucao da thread
  for(int xi = 0; xi < cnpj; xi++){

    int v1 = 0; // variavel para o calculo do primeiro digito
    int v2 = 0; // variavel para o calculo do segundo digito

    //calculo do primeiro digito
    v1 = (6*vtcnpj[xi][0])+(7*vtcnpj[xi][1])+(8*vtcnpj[xi][2]) +
    (9*vtcnpj[xi][3]);

    v1 = v1 + ((2*vtcnpj[xi][4])+(3*vtcnpj[xi][5])+(4*vtcnpj[xi][6]) +(5*vtcnpj[xi][7]));

    v1 = v1 + ((6*vtcnpj[xi][8])+(7*vtcnpj[xi][9])+(8*vtcnpj[xi][10]) +
    (9*vtcnpj[xi][11]));

    v1 = v1%11;

    if(v1 >= 10){
      v1 = 0;
    }
    
    // calculo para o segundo digito
    v2 = (5*vtcnpj[xi][0])+(6*vtcnpj[xi][1])+(7*vtcnpj[xi][2])+(8*vtcnpj[xi][3]);

    v2 = v2 + ((9*vtcnpj[xi][4])+(2*vtcnpj[xi][5])+(3*vtcnpj[xi][6])+(4*vtcnpj[xi][7]));

    v2 = v2 + ((5*vtcnpj[xi][8])+(6*vtcnpj[xi][9])+(7*vtcnpj[xi][10])+(8*vtcnpj[xi][11] + (9*v1)));
    v2 = v2%11;

    if(v2 >= 10){
      v2 = 0;
    }
      
    vtcnpj[xi][12] = v1; //valor do primeiro digito
    vtcnpj[xi][13] = v2; //valor do segundo digito
    
//  printf("\n DV %d%d \n", v1,v2);
  }
}


/*
 conforme solicitado, funcao do orquestrador que fara as threads do meu programa e printara os resultados como 
 a quantidade e o tempo gasto de calculo do CPF, CNPJ em milisegundos e o Total de documentos verificados.
 */
void orquestrador(){

  // thread para o calculo do CPF em millisegundos
  auto start = std::chrono::steady_clock::now();
  std::thread ThreadCPF(calculoCPF);
  ThreadCPF.join();
  auto end = steady_clock::now();

  milliseconds tempoCPF = duration_cast<milliseconds>
  (end - start);    

  //prints dos resultados
  printf("\n\nQuantidade de CPF's lidos: %d\n",cpf);
  cout<<"Tempo de calculo do CPF em milisegundos: " <<tempoCPF.count() << " ms";
  puts("\n");

  // thread para o calculo do CNPJ em millisegundos
  start = std::chrono::steady_clock::now();
  std::thread ThreadCNPJ(calculoCNPJ);
  ThreadCNPJ.join();
  end = steady_clock::now();

  milliseconds tempoCNPJ = duration_cast<milliseconds>(end - start); 

  //prints dos resultados
  printf("Quantidade de CNPJ's lidos: %d\n",cnpj);
  cout<< "Tempo de calculo do CNPJ em milisegundos: "<< tempoCNPJ.count() << " ms";
  puts("\n");

  // print da soma da quantidade de CPFs e CNPJs
  printf("Quantidade total de documentos calculados: %d\n\n",cpf+cnpj);

}

//funcao arquivo, que lera a BASE de dados dos CPFs e CNPJs
void arquivo(){

  // arquivo
  FILE* arquivo = fopen("BASE.txt", "r");

  // while que fara a leitura do arquivo
  while(!feof(arquivo)) {
      fgets(buffer, 16, arquivo);
      
      /* se no comeco da leitura das linhas for ' '(espaco), ele identificara que eh CPF, 
      sendo que o espaco ocupa ate 3 posicoes do vetor, por isso minhas logicas sempre iniciam 
      apos a terceira posicao do vetor.
      */
      if(buffer[0] == ' '){
        for(int j = 3;j<16;j++){
          vtcpf[cpf][j] = buffer[j] - '0';  // conversao de string para inteiro
        }
       cpf++; // variavel contador de CPF
      }
      
      // senao tiver espaco no inicio da linha, entao sao CNPJs
      else{
        for(int j = 0;j<16;j++){
          vtcnpj[cnpj][j] = buffer[j] - '0'; // conversao de string para inteiro
        }
       cnpj++; // variavel contador de CNPJ
      }
  } 

  fclose(arquivo);

}

//funcao de print dos vetores que armazenara em um arquivo
void printaVetores(){

  // arquivo
  FILE* printsArquivos = fopen("BASE_SAIDA.txt", "w"); 

  fprintf(printsArquivos,"\n\t\t CPF's\n\n");
  
  // for para print do CPF juntamente com seus digitos verificadores
  for(int i = 0; i<cpf;i++){
    fprintf(printsArquivos,"CPF : ");
    for(int j = 3; j<14;j++){
      fprintf(printsArquivos,"%d", vtcpf[i][j]);
    }
    fprintf(printsArquivos, "\n");
  }

  fprintf(printsArquivos,"\n\n\t\t CNPJ's\n\n");

  // for para print do CNPJ juntamente com seus digitos verificadores 
  for(int i = 0; i<cnpj;i++){
    fprintf(printsArquivos,"CNPJ: ");
    for(int j = 0; j <=13;j++){
      fprintf(printsArquivos,"%d", vtcnpj[i][j]);
    }
    fprintf(printsArquivos, "\n");
  }

  fclose(printsArquivos);

}

// funcao main
int main(){
  
  arquivo();
  orquestrador();
  printaVetores();
  
  return 0;
}