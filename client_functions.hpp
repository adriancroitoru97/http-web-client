#ifndef _CLIENT_FUNCTIONS_
#define _CLIENT_FUNCTIONS_

#include "io.hpp"
#include "requests.h"
#include <iostream>
#include "json/single_include/nlohmann/json.hpp"

#define LOGIN "login"
#define LIBRARY "library"

// Register an account on the server
void client_register(int &sockfd, char *host_ip,
                     map<string, string> &cookies_and_auth);

// Login to the server
void client_login(int &sockfd, char *host_ip,
                  map<string, string> &cookies_and_auth);

// Enter the library
void client_enter_library(int &sockfd, char *host_ip,
                          map<string, string> &cookies_and_auth);

// Get all books in the library
void client_get_books(int &sockfd, char *host_ip,
                      map<string, string> &cookies_and_auth);

// Get a specific book, based on its id
void client_get_book(int &sockfd, char *host_ip,
                     map<string, string> &cookies_and_auth);

// Add a book in the library
void client_add_book(int &sockfd, char *host_ip,
                     map<string, string> &cookies_and_auth);

// Delete a specific book, based on its id
void client_delete_book(int &sockfd, char *host_ip,
                        map<string, string> &cookies_and_auth);

// Logout from the server
void client_logout(int &sockfd, char *host_ip,
                   map<string, string> &cookies_and_auth);

#endif
