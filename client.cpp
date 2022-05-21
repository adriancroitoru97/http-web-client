#include <bits/stdc++.h>
#include <sys/socket.h>
#include "helpers.hpp"
#include "io.hpp"
#include "requests.h"
#include "client_functions.hpp"
#include "json/single_include/nlohmann/json.hpp"

using json = nlohmann::json;
using namespace std;

int main(void)
{
	int sockfd;
	char host_ip[] = "34.241.4.235";
	int host_port = 8080;

	char arg[MAX_PAYLOAD];

	map<string, string> cookies_and_auth;
	
	while (1) {
		sockfd = open_connection(host_ip, host_port, AF_INET, SOCK_STREAM, 0);

		cin.getline(arg, MAX_PAYLOAD);

		if (!strcmp(arg, "exit")) {
			break;
		} else if (!strcmp(arg, "register")) {
            client_register(sockfd, host_ip, cookies_and_auth);
        } else if (!strcmp(arg, "login")) {
            client_login(sockfd, host_ip, cookies_and_auth);
        } else if (!strcmp(arg, "enter_library")) {
            client_enter_library(sockfd, host_ip, cookies_and_auth);
        } else if (!strcmp(arg, "get_books")) {
            client_get_books(sockfd, host_ip, cookies_and_auth);
        } else if (!strcmp(arg, "get_book")) {
            client_get_book(sockfd, host_ip, cookies_and_auth);
        } else if (!strcmp(arg, "add_book")) {
            client_add_book(sockfd, host_ip, cookies_and_auth);
        } else if (!strcmp(arg, "delete_book")) {
            client_delete_book(sockfd, host_ip, cookies_and_auth);
        } else if (!strcmp(arg, "logout")) {
            client_logout(sockfd, host_ip, cookies_and_auth);
        } else if (strcmp(arg, "")){
            cout << "\n\"Invalid command!\"\n\n";
        }
	}

	close_connection(sockfd);

	return 0;
}
