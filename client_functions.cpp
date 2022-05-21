#include "client_functions.hpp"

using namespace std;
using json = nlohmann::json;

void client_register(int &sockfd, char *host_ip,
                     map<string, string> &cookies_and_auth)
{
	if (cookies_and_auth.count(LOGIN)) {
		cout << "\n\"Already logged in! Please log out firstly!\"\n\n";
		return;
	}

	/* I/O and json payload build */
	char *payload = (char *)malloc(sizeof(char) * MAX_PAYLOAD);
	if(!io_username_password(payload)) {
		cout << "\"Username and password cannot contain spaces!\"\n\n";
		return;
	}

	char *message;
	string response;
	char content_type[] = "application/json";
	char url[] = "/api/v1/tema/auth/register";
	message = compute_post_request(host_ip, url, content_type,
								   payload, NULL, 0, NULL);

	send_to_server(sockfd, message);
    response = receive_from_server(sockfd);

	if (response.find("201 Created") != string::npos) {
		cout << "\"Account created successfully!\"" << endl;
	} else {
		char temp[MAX_STRLEN];
		strcpy(temp, response.c_str());
		char *only_data = get_payload(temp);

		if (response.find("429 Too Many Requests") != string::npos) {
			cout << only_data + 1 << endl;
		} else if (response.find("400 Bad Request") != string::npos) {
			json resp = json::parse(only_data);
			cout << resp["error"] << endl;
		}
	}

	cout << endl;

	free(payload);
	free(message);
}

void client_login(int &sockfd, char *host_ip,
                  map<string, string> &cookies_and_auth)
{
	if (cookies_and_auth.count(LOGIN)) {
		cout << "\n\"Already logged in! Please log out firstly!\"\n\n";
		return;
	}

	/* I/O and json payload build */
	char *payload = (char *)malloc(sizeof(char) * MAX_PAYLOAD);
	char *cookie = (char *)malloc(sizeof(char) * MAX_COOKIE);
	if(!io_username_password(payload)) {
		cout << "\"Username and password cannot contain spaces!\"\n\n";
		return;
	}

	char *message;
	string response;
	char content_type[] = "application/json";
	char url[] = "/api/v1/tema/auth/login";
	message = compute_post_request(host_ip, url, content_type,
								   payload, NULL, 0, NULL);

	send_to_server(sockfd, message);
    response = receive_from_server(sockfd);

	if (response.find("200 OK") != string::npos) {
		cout << "\"Client logged in successfully!\"" << endl;

		// if login is successful, the session cookie
		// is added in the cookies_and_auth map
		get_cookie(response, cookie);
		cookies_and_auth[LOGIN] = cookie;
	} else {
		char temp[MAX_STRLEN];
		strcpy(temp, response.c_str());
		char *only_data = get_payload(temp);

		if (response.find("429 Too Many Requests") != string::npos) {
			cout << only_data + 1 << endl;
		} else if (response.find("400 Bad Request") != string::npos) {
			json resp = json::parse(only_data);
			cout << resp["error"] << endl;
		}
	}

	cout << endl;

	free(payload);
	free(cookie);
	free(message);
}

void client_enter_library(int &sockfd, char *host_ip,
                          map<string, string> &cookies_and_auth)
{
	if (cookies_and_auth.count(LIBRARY)) {
		cout << "\n\"Already in the library!\"\n\n";
		return;
	}

	char *message;
	string response;
	char url[] = "/api/v1/tema/library/access";

	// if a session cookie exists, it is added to the 'to be sent' message
	char **cookie_param = (char **)malloc(sizeof(char *));
	cookie_param[0] = (char*)malloc(sizeof(char) * MAX_COOKIE);
	if (cookies_and_auth.count(LOGIN)) {
		strcpy(cookie_param[0], cookies_and_auth[LOGIN].c_str());
		message = compute_get_request(host_ip, url, NULL,
									  cookie_param, 1, NULL);
	} else {
		message = compute_get_request(host_ip, url, NULL, NULL, 0, NULL);
	}

	send_to_server(sockfd, message);
    response = receive_from_server(sockfd);

	char temp[MAX_STRLEN];
	strcpy(temp, response.c_str());
	char *only_data = get_payload(temp);
	json resp = json::parse(only_data);

	cout << endl;
	if (response.find("200 OK") != string::npos) {
		cout << "\"Library entered successfully!\"" << endl;

		// the authorization token is added in the cookies_and_auth map
		string cookie = resp["token"];
		cookies_and_auth[LIBRARY] = cookie;
	} else {
		if (response.find("429 Too Many Requests") != string::npos) {
			cout << only_data + 1 << endl;
		} else if (response.find("401 Unauthorized") != string::npos) {
			cout << resp["error"] << endl;
		}
	}

	cout << endl;

	free(cookie_param[0]);
	free(cookie_param);
	free(message);
}

void client_get_books(int &sockfd, char *host_ip,
                      map<string, string> &cookies_and_auth)
{
	char *message;
	string response;
	char url[] = "/api/v1/tema/library/books";

	// if an authorization token exists, it is added to the message
	char *auth_param = (char*)malloc(sizeof(char) * MAX_COOKIE);
	if (cookies_and_auth.count(LIBRARY)) {
		strcpy(auth_param, cookies_and_auth[LIBRARY].c_str());
		message = compute_get_request(host_ip, url, NULL, NULL, 0, auth_param);
	} else {
		message = compute_get_request(host_ip, url, NULL, NULL, 0, NULL);
	}

	send_to_server(sockfd, message);
    response = receive_from_server(sockfd);

	char temp[MAX_STRLEN];
	strcpy(temp, response.c_str());
	char *only_data = get_payload(temp);

	cout << endl;
	if (response.find("200 OK") != string::npos) {
		cout << "The books in the library are:" << endl;

		json resp = json::parse(only_data);
		string temp(resp.dump(4));
		cout << temp << endl;
	} else {
		if (response.find("429 Too Many Requests") != string::npos) {
			cout << only_data + 1 << endl;
		} else if (response.find("403 Forbidden") != string::npos) {
			cout << "\"You don't have access to the library!\"" << endl;
		}
	}

	cout << endl;

	free(auth_param);
	free(message);
}

void client_get_book(int &sockfd, char *host_ip,
                     map<string, string> &cookies_and_auth)
{
	char *message;
	string response;
	int id;

	if (!io_id(id)) {
		cout << "\n\"ID may contain only digits!\"\n\n";
		return;
	}

	char url[] = "/api/v1/tema/library/books/";
	strcat(url, to_string(id).c_str());

	// if an authorization token exists, it is added to the message
	char *auth_param = (char*)malloc(sizeof(char) * MAX_COOKIE);
	if (cookies_and_auth.count(LIBRARY)) {
		strcpy(auth_param, cookies_and_auth[LIBRARY].c_str());
		message = compute_get_request(host_ip, url, NULL, NULL, 0, auth_param);
	} else {
		message = compute_get_request(host_ip, url, NULL, NULL, 0, NULL);
	}

	send_to_server(sockfd, message);
    response = receive_from_server(sockfd);

	char temp[MAX_STRLEN];
	strcpy(temp, response.c_str());
	char *only_data = get_payload(temp);

	json resp = json::parse(only_data);
	cout << endl;
	if (response.find("200 OK") != string::npos) {
		cout << "The book with \"id\": \"" << id << "\":\n";
		string temp(resp.dump(4));
		cout << temp << endl;
	} else {
		if (response.find("429 Too Many Requests") != string::npos) {
			cout << only_data + 1 << endl;
		} else if (response.find("403 Forbidden") != string::npos) {
			cout << "\"You don't have access to the library!\"" << endl;
		} else if (response.find("404 Not Found") != string::npos) {
			cout << resp["error"] << endl;
		}
	}

	cout << endl;

	free(auth_param);
	free(message);
}

void client_add_book(int &sockfd, char *host_ip,
                     map<string, string> &cookies_and_auth)
{
	char *payload = (char *)malloc(sizeof(char) * MAX_PAYLOAD);
	if (!io_book(payload)) {
		cout << "\n\"Invalid data entered! Please try again!\"\n\n";
		return;
	}

	char *message;
	string response;
	char content_type[] = "application/json";
	char url[] = "/api/v1/tema/library/books";

	// if an authorization token exists, it is added to the message
	char *auth_param = (char*)malloc(sizeof(char) * MAX_COOKIE);
	if (cookies_and_auth.count(LIBRARY)) {
		strcpy(auth_param, cookies_and_auth[LIBRARY].c_str());
		message = compute_post_request(host_ip, url, content_type,
									   payload, NULL, 0, auth_param);
	} else {
		message = compute_post_request(host_ip, url, content_type,
									   payload, NULL, 0, NULL);
	}
	
	send_to_server(sockfd, message);
    response = receive_from_server(sockfd);

	char temp[MAX_STRLEN];
	strcpy(temp, response.c_str());
	char *only_data = get_payload(temp);

	cout << endl;
	if (response.find("200 OK") != string::npos) {
		cout << "\"Book added successfully!\"\n";
	} else {
		if (response.find("429 Too Many Requests") != string::npos) {
			cout << only_data + 1 << endl;
		} else if (response.find("403 Forbidden") != string::npos) {
			cout << "\"You don't have access to the library!\"" << endl;
		}
	}
	
	cout << endl;

	free(auth_param);
	free(payload);
	free(message);
}

void client_delete_book(int &sockfd, char *host_ip,
                        map<string, string> &cookies_and_auth)
{
	char *message;
	string response;
	int id;

	if (!io_id(id)) {
		cout << "\n\"ID may contain only digits!\"\n\n";
		return;
	}

	char url[] = "/api/v1/tema/library/books/";
	strcat(url, to_string(id).c_str());

	// if an authorization token exists, it is added to the message
	char *auth_param = (char*)malloc(sizeof(char) * MAX_COOKIE);
	if (cookies_and_auth.count(LIBRARY)) {
		strcpy(auth_param, cookies_and_auth[LIBRARY].c_str());
		message = compute_delete_request(host_ip, url,
										 NULL, NULL, 0, auth_param);
	} else {
		message = compute_delete_request(host_ip, url,
										 NULL, NULL, 0, NULL);
	}

	send_to_server(sockfd, message);
    response = receive_from_server(sockfd);

	char temp[MAX_STRLEN];
	strcpy(temp, response.c_str());
	char *only_data = get_payload(temp);

	cout << endl;
	if (response.find("200 OK") != string::npos) {
		cout << "\"The book with \"id\": \"";
		cout << id << "\" was deleted from the library!\"\n";
	} else {
		if (response.find("429 Too Many Requests") != string::npos) {
			cout << only_data + 1 << endl;
		} else if (response.find("403 Forbidden") != string::npos) {
			cout << "\"You don't have access to the library!\"" << endl;
		} else if (response.find("404 Not Found") != string::npos) {
			cout << "\"The book with \"id\": \"";
			cout << id << "\" was not found!\"\n";
		}
	}

	cout << endl;

	free(auth_param);
	free(message);
}

void client_logout(int &sockfd, char *host_ip,
                   map<string, string> &cookies_and_auth)
{
	char *message;
	string response;
	char url[] = "/api/v1/tema/auth/logout";

	// if a login cookie exists, it is added to the message
	char **cookie_param = (char **)malloc(sizeof(char *));
	cookie_param[0] = (char*)malloc(sizeof(char) * MAX_COOKIE);
	if (cookies_and_auth.count(LOGIN)) {
		strcpy(cookie_param[0], cookies_and_auth[LOGIN].c_str());
		message = compute_get_request(host_ip, url,
									  NULL, cookie_param, 1, NULL);
	} else {
		message = compute_get_request(host_ip, url,
									  NULL, NULL, 0, NULL);
	}

	send_to_server(sockfd, message);
    response = receive_from_server(sockfd);

	char temp[MAX_STRLEN];
	strcpy(temp, response.c_str());
	char *only_data = get_payload(temp);

	cout << endl;
	if (response.find("200 OK") != string::npos) {
		cout << "\"Client logged out successfully!\"" << endl;
		cookies_and_auth.clear();
	} else {
		if (response.find("429 Too Many Requests") != string::npos) {
			cout << only_data + 1 << endl;
		} else if (response.find("400 Bad Request") != string::npos) {
			json resp = json::parse(only_data);
			cout << resp["error"] << endl;
		}
	}

	cout << endl;

	free(cookie_param[0]);
	free(cookie_param);
	free(message);
}
