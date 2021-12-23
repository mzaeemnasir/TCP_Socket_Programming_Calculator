// TCP Server using C Libraries

// Name: Muhammad Zaeem Nasir
// Roll: 19F-0355
// Section: B

// Flow Chart

// 1. Create a socket  & Attach it to a port
// 2. Bind the socket to a port
// 3. Listen for connections
// 4. Accept the connection
// 5. Receive data
// 6. Send data

#include <iostream>
#include <string>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
using namespace std;

int Calculating(int &number1, int &number2, int &operation)
{
    cout << "Number 1: " << number1 << endl;
    cout << "Number 2: " << number2 << endl;
    cout << "Operation: " << operation << endl;
    switch (operation)
    {
    case 1:
        return number1 + number2;
    case 2:
        return number1 - number2;
    case 3:
        return number1 * number2;
    case 4:
        return number1 / number2;
    default:
        break;
    }
    return 0;
}

int Get_inputs(int &Socket)
{
    string s = "Please Enter Operation: \n1. Addition \n2. Subtraction \n3. Multiplication \n4. Division \n";

    char Buffer[1024] = {0};
    int number1 = 0;
    int number2 = 0;
    int operation = 0;

    cout << "Sending Data from Server to Client" << endl;

    // Getting Input num 1
    send(Socket, "Please Enter Number 1: ", strlen("Please Enter Number 1: "), 0);
    read(Socket, Buffer, sizeof(Buffer));
    number1 = atoi(Buffer);

    // Getting Input num 2
    send(Socket, "Please Enter Number 2: ", strlen("Please Enter Number 2: "), 0);
    read(Socket, Buffer, sizeof(Buffer));
    number2 = atoi(Buffer);

    send(Socket, s.c_str(), strlen(s.c_str()), 0);
    memset(Buffer, 0, sizeof(Buffer));
    read(Socket, Buffer, sizeof(Buffer));

    if (Buffer[0] < '1' || Buffer[0] > '4')
    {
        send(Socket, "Invalid Input. Please Enter Again: \n", strlen("Invalid Input. Please Enter Again: \n"), 0);
        exit;
    }

    operation = atoi(Buffer);
    // Calculating the result
    int result = Calculating(number1, number2, operation); // This function will return the result
    // Sending the result to the client
    send(Socket, "Result: ", strlen("Result: "), 0);
    cout << "The Result is : " << result << endl;
    send(Socket, to_string(result).c_str(), strlen(to_string(result).c_str()), 0);
    return 0;
}
int main()
{
    // Creating Socket
    // AF_INET - IPv4
    // SOCK_STREAM - TCP
    // 0 - Protocol Value
    int Socket = socket(AF_INET, SOCK_STREAM, 0); // Here AF_INET is the address family, which is IPv4
    int Set_Sock_opt;
    int Port = 3000;
    int opt = 1;
    char Buffer[1024] = {0};
    if (Socket == 0)
    {
        cout << "Socket Creation Failed" << endl;
        exit;
    }
    // Now our Socket is created, Attatching Socket to the port
    // SOL_SOCKET - Socket Level
    // SO_REUSEADDR - Socket Option
    Set_Sock_opt = setsockopt(Socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

    // Attaching Socket to the port
    if (Set_Sock_opt < 0)
    {
        cout << "Socket Attachment Failed" << endl;
        exit;
    }

    // Binding Socket to the port
    struct sockaddr_in Server_Addr;
    // sockaddr_in is a structure that contains an internet address
    Server_Addr.sin_family = AF_INET;         // Address Family - Ip Address Type - IPv4
    Server_Addr.sin_port = htons(Port);       // Port Number
    Server_Addr.sin_addr.s_addr = INADDR_ANY; // INADDR_ANY - Any IP Address

    // Binding Socket to the port
    int Bind = bind(Socket, (struct sockaddr *)&Server_Addr, sizeof(Server_Addr));

    // if the Binind is not successful
    if (Bind < 0)
    {
        cout << "Socket Binding Failed" << endl;
        exit;
    }

    // Listening for connections
    int Listen = listen(Socket, 3);
    if (Listen < 0)
    {
        cout << "Socket Listening Failed" << endl;
        exit;
    }
    else
    {
        cout << "Server is Listening" << endl;
    }
    // Accepting the connection
    int addrlen = sizeof(Server_Addr);
    int Client_Socket = accept(Socket, (struct sockaddr *)&Server_Addr, (socklen_t *)&addrlen);
    if (Client_Socket < 0)
    {
        cout << "Socket Accepting Failed" << endl;
        exit;
    }
    else
    {
        cout << "Getting Inputs" << endl;
        Get_inputs(Client_Socket);
    }
    close(Socket);
    return 0;
}