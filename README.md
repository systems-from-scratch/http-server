# HTTP server in C

An HTTP/1.1 server built from scratch in C, on top of raw TCP sockets.
No frameworks. No external libraries.

## Features

- TCP socket server with `accept` loop
- HTTP/1.1 request parser (method, URI, version, headers, body)
- Dynamic router with method + URI matching
- HTTP response builder
- Persistent connections (keep-alive)
- Concurrent clients via pthreads
- `render_template` — serves HTML, CSS, JS from a `templates/` directory

## Building
```bash
make
make run
```

Starts the server on port 8000.

## Usage
```c
#include "Include/server.h"
#include "Include/render_template.h"

char* handle_index(struct httprequest *req) {
    return render_template("index.html");
}

int main() {
    struct Server server = make_server(8000);
    routes *r = create_routes(2);
    r = add_routes(r, GET, "/", handle_index);
    server.launch(&server, r);
}
```

## Architecture
```
src/
├── client.c
├── headers.c
├── httprequest.c
├── httpresponse.c
├── render_template.c
├── routes.c
└── server.c
templates/
├─ #html/css/js files go here

main.c
```

## Part of systems-from-scratch

This is repo 02 of the systems-from-scratch project —
building infrastructure from the ground up, one layer at a time.

→ [systems-from-scratch](https://github.com/systems-from-scratch)