#include "Include/server.h"
#include "Include/httprequest.h"
#include "Include/httpresponse.h"
#include "Include/routes.h"
#include "Include/client.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include "Include/render_template.h"
#define BUFFER_SIZE 30000


char* handle_index(struct httprequest *req) {
    return render_template("index.html");
}
char* handle_projects(struct httprequest *req) {
    return render_template("projects.html");
}
char* handle_about(struct httprequest *req) {
    return render_template("about.html");
}
char* handle_css(struct httprequest *req) {
    return render_template("style.css");
}

int main() {
    printf("[server] starting on port 8000\n");
    struct Server server = make_server(8000);
    routes *r = create_routes(2);
    r = add_routes(r, GET, "/", handle_index);
    r = add_routes(r, GET, "/about", handle_about);
    r = add_routes(r, GET, "/projects", handle_projects);
    r = add_routes(r, GET, "/style.css", handle_css);
    server.launch(&server,r);
    return 0;
}