#include "io.hpp"
#include "json/single_include/nlohmann/json.hpp"

using namespace std;
using json = nlohmann::json;

int io_username_password(char *payload) {
	char username[MAX_PAYLOAD], password[MAX_PAYLOAD];
	cout << "username="; cin.getline(username, MAX_PAYLOAD);
	cout << "password="; cin.getline(password, MAX_PAYLOAD);
	cout << endl;

	if (strchr(username, ' ') || strchr(password, ' ')) {
		return 0;
	}

	json payload_json;
	payload_json["username"] = username;
	payload_json["password"] = password;
	string payload_string = payload_json.dump();
	strcpy(payload, payload_string.c_str());

	return 1;
}

int io_id(int &id) {
	char temp[MAX_PAYLOAD];

	cout << "id=";
	cin.getline(temp, MAX_PAYLOAD);
	// null id is not accepted
	if (!strlen(temp)) {
		return 0;
	}

	// only digits accepted
	for (unsigned int i = 0; i < strlen(temp); i++) {
        if (!isdigit(temp[i])) {
			return 0;
		}
    }

	id = stoi(temp);

    return 1;
}

int io_book(char *payload) {
	char title[MAX_PAYLOAD], author[MAX_PAYLOAD];
	char genre[MAX_PAYLOAD], publisher[MAX_PAYLOAD];
	char temp[MAX_PAYLOAD];
	int page_count;

	cout << "title="; cin.getline(title, MAX_PAYLOAD);
	cout << "author="; cin.getline(author, MAX_PAYLOAD);
	cout << "genre="; cin.getline(genre, MAX_PAYLOAD);
	cout << "publisher="; cin.getline(publisher, MAX_PAYLOAD);
	cout << "page_count="; cin.getline(temp, MAX_PAYLOAD);
	// null fields are not accepted
	if (!strlen(title) || !strlen(author) || !strlen(genre) ||
		!strlen(publisher) || !strlen(temp))
	{
		return 0;
	}

	// only digits accepted for page_count field
	for (unsigned int i = 0; i < strlen(temp); i++) {
        if (!isdigit(temp[i])) {
			return 0;
		}
    }
	page_count = stoi(temp);

	json payload_json;
	payload_json["title"] = title;
	payload_json["author"] = author;
	payload_json["genre"] = genre;
	payload_json["page_count"] = page_count;
	payload_json["publisher"] = publisher;
	string payload_string = payload_json.dump();
	strcpy(payload, payload_string.c_str());

    return 1;
}
