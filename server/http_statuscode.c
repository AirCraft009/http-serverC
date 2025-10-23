
typedef struct {
    int code;
    char *msg;
} HttpStatusEntry;

static const HttpStatusEntry http_status_table[] = {
    {100, "Continue"},
    {101, "Switching Protocols"},
    {200, "OK"},
    {201, "Created"},
    {202, "Accepted"},
    {204, "No Content"},
    {301, "Moved Permanently"},
    {302, "Found"},
    {304, "Not Modified"},
    {400, "Bad Request"},
    {401, "Unauthorized"},
    {403, "Forbidden"},
    {404, "Not Found"},
    {405, "Method Not Allowed"},
    {408, "Request Timeout"},
    {500, "Internal Server Error"},
    {501, "Not Implemented"},
    {502, "Bad Gateway"},
    {503, "Service Unavailable"},
};

#define HTTP_STATUS_TABLE_LEN (sizeof(http_status_table) / sizeof(HttpStatusEntry))

char* http_status_to_string(int code) {
    for (int i = 0; i < HTTP_STATUS_TABLE_LEN; i++) {
        if (http_status_table[i].code == code)
            return http_status_table[i].msg;
    }
    return "Unknown Status Code";
}
