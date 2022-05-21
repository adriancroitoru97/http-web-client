# HTTP Web Client

HTTP Web Client implemented in `C++`. The client communicates with
a `REST API` server, which simulates an online library. Actions like `login`,
`logout`, `register`, `get_books`, `add_book`, `delete_book` etc. can be made.

## Client flow

The program listens commands until the `exit` command is typed.
It closes the  client. Any other command than the accepted ones will be
highlighted with an error message.
Any `invalid input` will be as well highlighted.

The general flow of a command:
1. The client verifies `if the command is possible`
   (for instance, for a `logout`, there has to be a session in progress).
2. If there is the case, the `I/O` part is added
   (both the prompt and the validating of the introduced data).
3. `The message is created` using the according functions.
   The **cookies/authorization headers** are added accordingly.
4. The `message` is sent and the `response` is stored in a string.
5. `The response is interpreted` using string parsing self-made or
   library functions. If the response contains a `JSON Object`,
   a library is used to easily extract the fields.
6. `Proper messages are prompted`.

As the HTTP Protocol is `connectionless`,
a socket is opened every time a command is typed.\
Each function ends with `freeding` of the dynamically allocated `memory`.


## Cookies and JWT Tokens

As many commands require a `logged in` session cookie, or
a `JWT Authorization Header`, a **map<string, string>** was used. The key can
either be `LOGIN` or `LIBRARY`. The first key stands for the session cookie,
while the second one is for the JWT Token.
The values are the actual cookies/tokens.

Both the `session cookie` and the `JWT Token` remains the same during
a session (until the `logout` command is used). The `logout` command will
clear the map, making any action in the library impossible until a new session
is started (with a `login` command),
and the library is accessed again (with `enter_library` command).

## JSON Parsing

As the payloads of both `requests` and `responses` are using
the `application/json` type, the [Nlohmann](https://github.com/nlohmann/json/)
library was used. It provided both the ease of creating
'JSON designed' strings (which were used in computing
the messages for the server) and the ease of accessing
already created structures (the responses from the server).

## License
[Adrian-Valeriu Croitoru, 322CA](https://github.com/adriancroitoru97/)