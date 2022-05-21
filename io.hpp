#ifndef _IO_
#define _IO_

#include <iostream>
#include <string.h>
#include "json/single_include/nlohmann/json.hpp"
#include "io.hpp"
#include "helpers.hpp"

using namespace std;

// IO session for login/register -returns 0 if invalid credentials, 1 otherwise
int io_username_password(char *payload);

// IO session for getting an id -returns 0 if invalid credentials, 1 otherwise
int io_id(int &id);

// IO session for getting a book -returns 0 if invalid credentials, 1 otherwise
int io_book(char *payload);

#endif
