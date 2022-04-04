#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <ctime>
#include <string.h>
 
using namespace std;
const int MAX = 3;
//Nesse caso vou armazenar 3 contas, mas podendo
//mudar para qualquer valor
//Poderia usar #define MAX 3, outro forma de
//definir o MAX
 
struct tm *info_tempo;
 
struct banco{
    //Registro para guardar
    //dados do usuário
    int conta_num;
    int agencia_num;
    char name[20];
    char cpf[12];
    float salario;
};
 
struct extrato{
    //Registro para usar os
    //dados do usuário na impressao
    //do arquivo html
    int conta_numero;
    int agencia_numero;
    char historico[100];
    char horario[10];
    char data[10];
    float valor;
};
 
int menu(){
    //Menu de escolha do usuario
    int op;
    cout << "*******************************\n";
    cout << "*  ARABANCO - SEMPRE COM VOCE *\n";
    cout << "*******************************\n";
    cout << "\nMenu de opcoes:\n";
    cout << "1 - Cadastrar Conta Corrente\n";
    cout << "2 - Alterar dados de uma Conta Corrente\n";
    cout << "3 - Excluir uma Conta Corrente\n";
    cout << "4 - Consultar Saldo\n";
    cout << "5 - Fazer Deposito\n";
    cout << "6 - Realizar Saque\n";
    cout << "7 - Transferir Valores\n";
    cout << "8 - Mostrar todas as contas e saldos\n";
    cout << "9 - Emitir extrato de conta\n";
    cout << "10 - Sair\n";
    cout << "\nEscolha uma opcao: ";
    cin >> op;
    return op;
}
 
void cadastrar_contas(banco *ptr, int contador); //Funcao para cadastrar contas
void data(char data_local[]); //Funcao para "pegar" a data usar ou armazenala no extrato
void hora(char hora_local[]);//Funcao para "pegar" a hora usar ou armazenala no extrato
int verificando_dados(banco *a, int contador, int recebendo_conta_or_agencia); //funcao para verificar se conta e agencia existe
int numberConta_agencia(); //Funcao para informa o numero da conta e agencia em algumas opcoes
void mostrar_conta_salario(banco *p, int contador); //Funcao serve para mostrar os dados das contas armazenadas
void excluir(banco p[], int posicao, int &contador, int contador_aux, extrato pe[]);//Funcao para deletar uma conta escolhida
void mostrar_saldo(banco *p, int contador, int posicao); //Funcao para consultar o saldo
 
 
int main()
{
    ofstream fout;//fout para escrever no arquivo
    ifstream fin;//fin para ler o arquivo
    banco clientes_dados[MAX], *p;//variaveis do tipo struct banco
    extrato extrato_cliente[5000];//variaveis do tipo struct extrato
 
    int contador = 0; //esse contador serve para saber quantas contas foram digitadas
    int contador_auxiliar = 0; //esse contador serve para usar no extratos dos clientes
    int opcao; // receber opcao do usuario
 
    int conta, agencia;
    //Esses duas opcoes serve para informa a conta e agencia
    //em determinads funcoes, pois nao usei a funcao *int numberConta_agencia();*
 
    
    fin.open("arabanco.dat", ios::binary);

    //LENDO O ARQUIVO
    if(!fin){
        cout << "Erro: arquivo nao foi aberto." << endl;
        system("pause");
    }
    else{
        fin.read((char *)&contador, sizeof(contador));
        fin.read((char *)clientes_dados, sizeof(clientes_dados));
        fin.read((char *)&contador_auxiliar, sizeof(contador_auxiliar));
        fin.read((char *)extrato_cliente, sizeof(extrato_cliente));
        fin.close();
    }
    
    //iniciando o o do..while para armazenamento de
    //dados
    do{
        system("cls");
        opcao = menu();
        system("cls");
 
        switch(opcao){
            case 1:
 
                if(contador >= MAX){
                    cout << "Obrigado pelo confianca.\n";
                    cout << "Mas estamos com muitos clientes cadastrados.\n\n";
                    system("pause");
                    break;
                }
                cadastrar_contas(clientes_dados, contador);//Cadastrando contas

                //Essa parte serve para armazenar os dados da struct banco
                //para a struct extranto, usando suas variaveis, para que
                //possa usar futuramente no arquivo html
                extrato_cliente[contador_auxiliar].agencia_numero = clientes_dados[contador].agencia_num;
                //passando para o extrato o numero da agencia
                extrato_cliente[contador_auxiliar].conta_numero = clientes_dados[contador].conta_num;
                //passando para o extrato o numero da conta
                strcpy(extrato_cliente[contador_auxiliar].historico, "Saldo Inicial");
                //guardadno no extrato a opcao de saldo inicial
                extrato_cliente[contador_auxiliar].valor = clientes_dados[contador].salario;
                //guardadno no extrato o valor do salario
                hora(extrato_cliente[contador_auxiliar].horario);//guardadno no extrato da hora do saque
                data(extrato_cliente[contador_auxiliar].data);//guardadno no extrato da data do saque

                //Incrementando contadores
                contador++;
                contador_auxiliar++;
 
                cout << "Cadastrado com sucesso.\n";
                cout << "ARABANCO agradece pela confianca.\n" << endl;
                system("pause");
 
                break;
 
            case 2:
 
                int verifi_conta, verifi_agencia;
                //Essa parte serve para verificar se existe
                //as contas, chamando determinadas funcoes, que
                //ja foram explicadas
                cout << "Alterar dados da conta corrente." << endl;
                cout << "Numero da conta: ";
                verifi_conta = verificando_dados(clientes_dados, contador, numberConta_agencia());
                cout << "Numero da agencia: ";
                verifi_agencia = verificando_dados(clientes_dados, contador, numberConta_agencia());

                if(verifi_conta < 0 || verifi_agencia < 0){
                    //Se a conta nao existir mostra isso
                    cout << "Conta ou Agencia nao cadastrada.\n";
                    system("pause");
                    break;
                }
                else{
                    //caso exista faz isso, isto e, 
                    //ler outro nome e cpf
                    p = &clientes_dados[verifi_conta];
                    cout << "Informe um novo nome: ";
                    cin >> p->name;
                    cout << "Informe um novo cpf: ";
                    cin >> p->cpf;
                }
                system("pause");
                break;
 
            case 3:
                int conta_deletando, agencia_deletando;
                bool yes_or_no;
                yes_or_no = false;
                //Essa parte serve para verificar se existe
                //as contas, chamando determinadas funcoes, que
                //ja foram explicadas
                cout << "Alterar dados da conta corrente." << endl;
                cout << "Numero da conta: ";
                conta_deletando = verificando_dados(clientes_dados, contador, numberConta_agencia());
                cout << "Numero da agencia: ";
                agencia_deletando = verificando_dados(clientes_dados, contador, numberConta_agencia());
 
               
                if(conta_deletando< 0 || agencia_deletando < 0){
                    //Se a conta nao existir mostra isso
                    cout << "Conta ou Agencia nao cadastrada.\n";
                    system("pause");
                    break;
                }
                yes_or_no = true;
                //se eh true entao a conta existe
                if(yes_or_no){
                    //caso exista faz isso, ou seja, 
                    //deleta a conta que foi informada
                    excluir(clientes_dados, conta_deletando, contador, contador_auxiliar, extrato_cliente);
                    system("cls");
                    cout << "Seus dados foram deletados." << endl;
                    system("pause");
                    break;
                }
 
                system("pause");
                break;
 
            case 4:
 
                int conta_verificando, agencia_verificando;
                //Essa parte serve para verificar se existe
                //as contas, chamando determinadas funcoes, que
                //ja foram explicadas
                cout << "Verificando dados da conta corrente." << endl;
                cout << "Numero da conta: ";
                conta_verificando = verificando_dados(clientes_dados, contador, numberConta_agencia());
                cout << "Numero da agencia: ";
                agencia_verificando = verificando_dados(clientes_dados, contador, numberConta_agencia());
 
                if(conta_verificando < 0 || agencia_verificando < 0){
                    //Se a conta nao existir mostra isso
                    cout << "Conta ou Agencia nao cadastrada.\n";
                    system("pause");
                    break;
                }
                system("cls");
                //Mostrando a conta informada e seus dados
                mostrar_saldo(clientes_dados, contador, conta_verificando);
                system("pause");
                break;
 
            case 5:
 
                int verificando_conta, verificando_agencia, valor_depositado;
                //Essa parte serve para verificar se existe
                //as contas, chamando determinadas funcoes, que
                //ja foram explicadas
                cout << "Verificando dados da conta corrente." << endl;
                cout << "Numero da conta: ";
                cin >> conta;
                verificando_conta = verificando_dados(clientes_dados, contador, conta);
                cout << "Numero da agencia: ";
                cin >> agencia;
                verificando_agencia = verificando_dados(clientes_dados, contador, agencia);
               
 
                if(verificando_conta < 0 || verificando_agencia < 0){
                    //Se a conta nao existir mostra isso
                    cout << "Conta ou Agencia nao cadastrada.\n";
                    system("pause");
                    break;
                }
                system("cls");
                cout << endl;
                //Mostrando a conta informada e seus dados
                mostrar_saldo(clientes_dados, contador, verificando_conta);
                cout << "Valor a ser depositado - R$: ";
                cin >> valor_depositado;//informando o valor depositado
                
                //armazenando dados no registro extrato, para posso usar
                //no arquivo html principalmente
                clientes_dados[verificando_conta].salario += valor_depositado;//guardando valor depositado
                extrato_cliente[contador_auxiliar].agencia_numero = agencia;//passando para o extrato o numero da agencia
                extrato_cliente[contador_auxiliar].conta_numero = conta;//passando para o extrato o numero da conta
                strcpy(extrato_cliente[contador_auxiliar].historico, "Deposito");//guardadno no extrato a opcao depositado
                extrato_cliente[contador_auxiliar].valor = valor_depositado;//guardadno no extrato o valor depositado
                hora(extrato_cliente[contador_auxiliar].horario);//guardadno no extrato da hora do deposito
                data(extrato_cliente[contador_auxiliar].data);//guardadno no extrato da data do deposito

                contador_auxiliar++;//incremente esse contador
                break;
 
            case 6:
 
                int contaVerificando, agenciaVerificando, valorq_sacado;
                //Essa parte serve para verificar se existe
                //as contas, chamando determinadas funcoes, que
                //ja foram explicadas
                cout << "Verificando dados da conta corrente." << endl;
                cout << "Numero da conta: ";
                cin >> conta;
                contaVerificando = verificando_dados(clientes_dados, contador, conta);
                cout << "Numero da agencia: ";
                cin >> agencia;
                agenciaVerificando = verificando_dados(clientes_dados, contador, agencia);
 
 
                if(contaVerificando < 0 || agenciaVerificando < 0){
                    //Se a conta nao existir mostra isso
                    cout << "Conta ou Agencia nao cadastradas.\n";
                    system("pause");
                    break;
                }
                system("cls");
                cout << endl;
                mostrar_saldo(clientes_dados, contador, contaVerificando);
                cout << "Valor a ser sacado - R$: ";
                cin >> valorq_sacado;
                //guardando valor retirado que foi sacado
                clientes_dados[contaVerificando].salario -= valorq_sacado;
 
                extrato_cliente[contador_auxiliar].agencia_numero = agencia;//passando para o extrato o numero da agencia
                extrato_cliente[contador_auxiliar].conta_numero = conta;//passando para o extrato o numero da conta
                strcpy(extrato_cliente[contador_auxiliar].historico, "Saque");//guardadno no extrato a opcao de saque
                extrato_cliente[contador_auxiliar].valor = valorq_sacado * -1;//guardadno no extrato o valor sacado
                hora(extrato_cliente[contador_auxiliar].horario);//guardadno no extrato da hora do saque
                data(extrato_cliente[contador_auxiliar].data);//guardadno no extrato da data do saque
                contador_auxiliar++;//incrementando esse contador
 
                break;
 
            case 7:
 
                int acountVerifi, agencyVerifi, valor_transfe;
                int acount_destino, agency_destino;
                int conta1, agencia1;
                int conta2, agencia2;

                //Essa parte serve para verificar se existe
                //as contas, chamando determinadas funcoes, que
                //ja foram explicadas
                cout << "Verificando dados da conta origem." << endl;
                cout << "Numero da conta: ";
                cin >> conta1;
                acountVerifi = verificando_dados(clientes_dados, contador, conta1);
                cout << "Numero da agencia: ";
                cin >> agencia1;
                agencyVerifi = verificando_dados(clientes_dados, contador, agencia1);
 
                
 
                if(acountVerifi < 0 || agencyVerifi < 0){
                    //Se a conta origem nao existir mostra isso
                    cout << "\nConta ou Agencia nao cadastrada.\n";
                    system("pause");
                    break;
                }
 
                else{
                    //Essa parte serve para verificar se existe
                    //as contas, chamando determinadas funcoes, que
                    //ja foram explicadas. Usando duas vezes essa forma
                    //na opcao 7
                    cout << "Vericando dados da conta destino.\n";
                    cout << "Numero da conta: ";
                    cin >> conta2;
                    acount_destino = verificando_dados(clientes_dados, contador, conta2);
                    cout << "Numero da agencia: ";
                    cin >> agencia2;
                    agency_destino = verificando_dados(clientes_dados, contador, agencia2);
 
                    if(acount_destino < 0 || agency_destino < 0){
                        //Se a conta destino nao existir mostra isso
                        cout << "Conta ou Agencia nao cadastrada.\n";
                        system("pause");
                        break;
                    }
 
                    system("cls");
                    cout << "Conta ORIGEM:\n";
                    mostrar_saldo(clientes_dados, contador, acountVerifi);
                    cout << "\nConta DESTINO:\n";
                    mostrar_saldo(clientes_dados, contador, acount_destino);
                    cout << "\nValor a ser transferido - R$: ";
                    cin >> valor_transfe;

                    //guardando valor retirado que foi transferido
                    clientes_dados[acountVerifi].salario -= valor_transfe;

                    //guardando valor recebido que foi transferido
                    clientes_dados[acount_destino].salario += valor_transfe;
 
                    extrato_cliente[contador_auxiliar].agencia_numero = agencia1;//passando para o extrato o numero da agencia

                    extrato_cliente[contador_auxiliar].conta_numero = conta1;//passando para o extrato o numero da conta

                    //guardadno no extrato a opcao de transferenica enviada
                    strcpy(extrato_cliente[contador_auxiliar].historico, "Transferencia Enviada");

                    extrato_cliente[contador_auxiliar].valor = valor_transfe * -1;//guardadno no extrato o valor transferido

                    hora(extrato_cliente[contador_auxiliar].horario);//guardadno no extrato da hora da transferencia

                    data(extrato_cliente[contador_auxiliar].data);//guardadno no extrato da data da transferencia
                    contador_auxiliar++;
 
                    extrato_cliente[contador_auxiliar].agencia_numero = agencia2;//passando para o extrato o numero da agencia

                    extrato_cliente[contador_auxiliar].conta_numero = conta2;//passando para o extrato o numero da conta

                    //guardadno no extrato a opcao de transferenica recebida
                    strcpy(extrato_cliente[contador_auxiliar].historico, "Transferencia Recebida");
                    extrato_cliente[contador_auxiliar].valor = valor_transfe;//guardadno no extrato o valor transferido

                    hora(extrato_cliente[contador_auxiliar].horario);//guardadno no extrato da hora da transferencia

                    data(extrato_cliente[contador_auxiliar].data);//guardadno no extrato da data da transferencia
                    contador_auxiliar++;
 
                    cout << "Transferencia realizada com sucesso." << endl;
                    system("pause");
                }
 
 
                break;
 
 
            case 8:
 
                if(contador == 0){
                    //Se for zero, quer dizer que nao tem nenhuma
                    //conta cadastrada
                    cout << "Nenhum cliente cadastrado.\n";
                    system("pause");
                    break;
                }
                cout << "Lista de clientes cadastrados" << endl;
                mostrar_conta_salario(clientes_dados, contador);//Mostrando contas cadastradas
                system("pause");
 
                break;
 
            case 9:
                int escolha_extrato;
                int conta_extrato, agencia_extrato;
                char dt[10]; //data que vai "capturar"
                char hr[10]; //hora que vai "capturar"
                data(dt);//chamando funcao que pega os dados da data
                hora(hr);//chamando fucao que pega os dados da hora
 
                //Essa parte serve para verificar se existe
                //as contas, chamando determinadas funcoes, que
                //ja foram explicadas
                cout << "Informe alguns dados basicos da conta.\n";
                cout << "Numero da conta: ";
                cin >> conta;
                conta_extrato = verificando_dados(clientes_dados, contador, conta);
                cout << "Numero da agencia: ";
                cin >> agencia;
                agencia_extrato = verificando_dados(clientes_dados, contador, agencia);
 
 
                if(conta_extrato < 0 || agencia_extrato < 0){
                    cout << "Conta ou Agencia nao cadastrada.\n";
                    system("pause");
                    break;  
                }
               
                system("cls");
                //usuario informa que se quer extrato
                //em tela ou em arquivo, no caso, html
                //na case 2
                cout << "[1] - Extrato na tela.\n";
                cout << "[2] - Extrato em arquivo.";
                cout << "\nInforme sua escolha: ";
                cin >> escolha_extrato;
 
                switch (escolha_extrato){
                    case 1:
                        system("cls");
                        cout << "\n\t\t\tEXTRATO EM TELA\n\n";
                        cout << "\tNome: " << clientes_dados[conta_extrato].name << "\t\tAgencia/Conta: " << clientes_dados[conta_extrato].agencia_num << "/"
                            << clientes_dados[conta_extrato].conta_num;
                        cout << "\n\tData: " << dt << "\t\tHora: " << hr << endl << endl;
 
                        for(int i = conta_extrato; i < contador_auxiliar; i++){
                            if( (conta - extrato_cliente[i].conta_numero) == 0 && (agencia - extrato_cliente[i].agencia_numero) == 0 && strcmp(extrato_cliente[i].historico, "Total") != 0){
                               
                                cout << "Horario: " << extrato_cliente[i].horario << endl;
                                cout << "Data: " << extrato_cliente[i].data << endl;
                                cout << "Historico: " << extrato_cliente[i].historico << endl;
                                cout << "Valor - R$: " << extrato_cliente[i].valor << endl << endl;
                               
                            }
                        }
                        cout << "Saldo atual - R$: " << clientes_dados[conta_extrato].salario << endl;
                        system("pause");
                        break;
 
                    case 2:
                        extrato_cliente[contador_auxiliar].agencia_numero = agencia;//guardando o numero da agencia
                        extrato_cliente[contador_auxiliar].conta_numero = conta;//guardando o numero da conta
                        strcpy(extrato_cliente[contador_auxiliar].historico, "Total");//guardando o o dado de total no extrato
                        //guardando no extrato o salario que esta no resgitro banco, usando as determinadas variaveis
                        extrato_cliente[contador_auxiliar].valor = clientes_dados[conta_extrato].salario;
                        hora(extrato_cliente[contador_auxiliar].horario);//"pegando" os dados da hora
                        data(extrato_cliente[contador_auxiliar].data);//"pegando" os dados da data
                        contador_auxiliar++;//incrementando esse contador
 
                        system("cls");
                        //Escrevendo o arquivo HTML
                        fout.open("arabanco.html");
                        if(!fout){
                            cout << "Erro: nao foi possivel abrir o arquivo HTML." << endl;
                        }
                        else{
                            char DT[10], HR[10];
                            float new_valoes = 0;
                            float outro_valor = 0;
                            float receber_outro = 0;
                            //Esses 3 variaveis usei para armazenar determinados acoes
                            //para mostrar o saldo, acredito que deve ter outro
                            //forma, contudo conseguir fazer assim.
                            //Voce entendera no andamento da escrita do codigo, o
                            //o motivo de usar elas.
                            
                            hora(HR);
                            data(DT);
                            //Configurando o arquivo html
                            fout << "<!DOCTYPE html>";
                            fout << "<html lang='pt-br'>";
                            fout << "<head>";
                            fout << "<meta charset='UTF-8'>";
                            fout << "<meta http-equiv='X-UA-Compatible' content='IE=edge'>";
                            fout << "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
                            fout << "<title>Trabalho final</title>";
                            fout << "<link rel='shortcut icon' href='favicon.ico' type='image/x-icon'>";
                            fout << "<style>";
                            fout << "body{";
                            fout << "background-color: #f5f5dc;";
                            fout << "}";
                            fout << "p{";
                            fout << "font-size: 18px;";
                            fout << "font-family: Arial;";
                            fout << "}";
                            fout << "thead{";
                            fout << "background-color: #a9a9ac;";
                            fout << "color: #0000b0;";
                            fout << "font-family: Arial;";
                            fout << "font-size: 25px;";

                            fout << "}";
                            fout << "td{";
                            fout << "font-size: 14px;";
                            fout << "background-color: #f5dec0;";
                            fout << "}";
                            fout << "caption{";
                            fout << "font-size: 15px;";
                            fout << "}";
       
        
                            fout << "</style>";
                            fout << "</head";

                            fout << "<body>";  


                            fout << "<center>";

                            fout << "<h1><strong>EXTRATO BANCÁRIO</strong></h1>";

                            fout << "<p>";
                            
                            //mostrando os dados primordias da conta do arabanco
                            fout << "Nome: " << clientes_dados[conta_extrato].name << " &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Agencia/Conta: " << clientes_dados[conta_extrato].agencia_num << "/" << clientes_dados[conta_extrato].conta_num << "<br>" << "Data: " << DT << " &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Horario: " << HR;

                            fout << "</p>";

                            //utilizei o metedo de tabela, para mostrar as informacoes
                            fout << "<table>";
                            fout << "<caption><strong>Historico</strong></caption>";
    
                            fout << "<thead>";
    
                            fout << "<th><strong>Data</strong></th>";
                            fout << "<th><strong>Hora</strong></th>";
                            fout << "<th><strong>Lancamento</strong></th>";
                            fout << "<th><strong>Valor &lpar;R$&rpar;</strong></th>";
                            fout << "<th><strong>Saldo &lpar;R$&rpar;</strong></th>";
    
    
                            fout << "</thead>";
    
                            fout << "<tbody>";
                            
                            //aqui sao os linhas mostrando data, horario, historico
                            //e o valor
                            fout << "<tr>";
                            for(int i = conta_extrato; i < contador_auxiliar; i++){
                                
                                
                                if( (conta - extrato_cliente[i].conta_numero) == 0 && (agencia - extrato_cliente[i].agencia_numero) == 0 && strcmp(extrato_cliente[i].historico, "Total") != 0){
                                    new_valoes = extrato_cliente[i].valor;
                            
                                    fout << "<td><strong>" << extrato_cliente[i].data << "</strong></td>";
                                    fout << "<td><strong>" << extrato_cliente[i].horario << "</strong></td>";
                                    fout << "<td><strong>" << extrato_cliente[i].historico << "</strong></td>";
                                    fout << "<td><strong>" << extrato_cliente[i].valor << "</strong></td>";
                                    
                                    if(extrato_cliente[i].valor){
                                        //aqui usei as 3 variaveis citadas acima
                                        //para posso motivar o saldo, sendo ele negativo
                                        //ou nao
                                        if(outro_valor < -1){
                                            outro_valor *= (-1);
                                        }
                                        outro_valor = new_valoes;
                                        receber_outro += outro_valor;
                                        fout << "<td><strong>" << receber_outro << "</strong></td>";
                                    }
                                    
                                }
                                fout << "</tr>";
                            }
                            
                            

                            fout << "</tbody>";
    
                            fout << "</table>";

                            fout << "</center>";

                            fout << "</body>";

                            fout << "</html>";
                            //terminado o arquivo html

                        }
                        fout.close();//fechando o arquivo
                        cout << "Seu extrato feito com sucesso." << endl << endl;
                        system("pause");
                        break;
                   
               
                    default:
                        cout << "Escola opcao 1 ou 2 para extratos.\n";
                        system("pause");
                        //caso o usario informar um numero diferente,
                        //sendo um 1 o extrato em tela e 2 extrato em
                        //arquivo
                        break;
                }
 
               
 
                break;
 
            case 10:

                //Abrindo e escrevendo o arquivo binario
                //isto eh, lendo os dados informados
                fout.open("arabanco.dat", ios::binary);
                if(!fout){
                    cout << "Erro: nao foi possivel criar o arquivo.\n";
                }
                else{
                    fout.write((char *)&contador, sizeof(contador));
                    fout.write((char *)clientes_dados, sizeof(clientes_dados));
                    fout.write((char *)&contador_auxiliar, sizeof(contador_auxiliar));
                    fout.write((char *)extrato_cliente, sizeof(extrato_cliente));
                    fout.close();
                }
                cout << "GOODBYE." << endl;
                break;
 
            default:
                cout << "Digite um valor entre 1 e 10.\n";
                system("pause");
                //caso o usario informar um numero diferente,
                //sendo um 1 o extrato em tela e 2 extrato em
                //arquivo
                break;
        }
    }while(opcao != 10);
 
    return 0;
}
 
void cadastrar_contas(banco *ptr, int n){
    /*Função vai serve para que o usuário
    insira os dados pertinentes a sua conta.
    n = contador*/
    cout << "Cadastrando suas informacoes.\n\n";
    cout << "Informe os dados seguintes:\n";
    cout << "Numero da conta: ";
    cin >> ptr[n].conta_num;
    cout << "Numero da agencia: ";
    cin >> ptr[n].agencia_num;
    cout << "Nome: ";
    cin >> ptr[n].name;
    cout << "CPF: ";
    cin >> ptr[n].cpf;
    cout << "Saldo inicial - R$: ";
    cin >> ptr[n].salario;
    if(ptr[n].salario < 0){
        cout << "Salario inicial negativo.\n";
        cout << "Informe outro salario.\nR$: ";
        cin >> ptr[n].salario;
    }
    system("cls");
}
 
void data(char data_local[]){
    //funcao para guarda a informacao
    //da data local
    time_t rawtime;
    time (&rawtime);
    info_tempo = localtime (&rawtime);
    strftime(data_local,80,"%d/%m/%y",info_tempo);
}
 
void hora(char hora_local[]){
    //funcao para guarda a informacao
    //da hora local
    time_t rawtime;
    time (&rawtime);
    info_tempo = localtime (&rawtime);
    strftime(hora_local,80,"%H:%M %p",info_tempo);
}
 
int verificando_dados(banco *a, int contador, int recebendo_conta_or_agencia){
    /*Função para verificar se existe a
    conta e a agencia*/
    int x;
    for(x=0;x<contador;x++){
        if(a[x].conta_num == recebendo_conta_or_agencia){
            return x;
        }
        if(a[x].agencia_num == recebendo_conta_or_agencia){
            return x;
        }
    }
    return -1;
}
 
int numberConta_agencia(){
    //Função que ler a numero da
    //da conta e agencia
    int a;
    cin >> a;
    return a;
}
 
void mostrar_conta_salario(banco *p, int contador){
    /*Função que serve para mostrar as contas
    cadastradas no banco.
    n = contador*/
    int x;
    for(x=0;x<contador;x++){
        cout << "\nConta " << x+1 << endl;
        cout << "Numero da conta: " << p[x].conta_num;
        cout << "\nNumero da agencia: " << p[x].agencia_num;
        cout << "\nNome: " << p[x].name;
        cout << "\nCPF do titular: " << p[x].cpf;
        cout << "\nSaldo atual - R$: " << p[x].salario << endl;
        cout << endl;
        // Tenho que mudar esse SALDO ATUAl
        // para mostrar o verdadeiro valor.
       
    }
   
}
 
void excluir(banco p[], int posicao, int &contador, int contador_aux, extrato pe[]){
    //Funcao que puxar a conta anterior para "cima"
    //para poder excluir a conta desejada
    for(int i = 0; i < contador_aux; i++){
        if( (p[posicao].conta_num - pe[i].conta_numero) == 0){
            pe[i].conta_numero = NULL;
        }
    }
    for(int i = posicao; i < contador-1; i++){
        p[i] = p[i+1];
    }
 
    contador--;
}
 
void mostrar_saldo(banco *p, int contador, int posicao){
    //Função serve para mostrar o saldo
    //e os outros dados da conta
    for(int i = 0; i < contador; i++){
        if(i == posicao){
            cout << "Conta " << i+1 << endl;
            cout << "Numero da conta: " << p[i].conta_num;
            cout << "\nNumero da agencia: " << p[i].agencia_num;
            cout << "\nNome: " << p[i].name;
            cout << "\nCPF do titular: " << p[i].cpf;
            cout << "\nSaldo atual - R$: " << p[i].salario << endl;
            cout << endl;
            break;
        }
    }
}
