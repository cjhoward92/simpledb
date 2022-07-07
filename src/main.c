#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#include "trie.h"

#define MAX_BUF_SIZE 1024

static const int server_port = 11443;

typedef unsigned char bool;
#define true 1
#define false 0

static volatile bool is_running = true;

int create_socket(bool is_server) {
    int s;
    int optval = 1;
    struct sockaddr_in addr;

    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) {
        perror("Unable to create socket");
        exit(EXIT_FAILURE);
    }

    if (!is_server) {
        return s;
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(server_port);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0) {
        perror("Error calling setsockopt(SO_REUSEADDR)!");
        exit(EXIT_FAILURE);
    }

    if (bind(s, (struct sockaddr*) &addr, sizeof(addr)) < 0) {
        perror("Error calling bind(s)");
        exit(EXIT_FAILURE);
    }

    if (listen(s, 1) < 0) {
        perror("Error calling listen(s)");
        exit(EXIT_FAILURE);
    }

    return s;
}

SSL_CTX* create_context(bool is_server) {
    const SSL_METHOD *method;
    SSL_CTX *ctx;

    if (is_server) {
        method = TLS_server_method();
    } else {
        method = TLS_client_method();
    }

    ctx = SSL_CTX_new(method);
    if (ctx == NULL) {
        perror("Unable to create the SSL context");
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }

    return ctx;
}

void configure_server_context(SSL_CTX *ctx) {
     if (SSL_CTX_use_certificate_chain_file(ctx, "cert.pem") <= 0) {
         ERR_print_errors_fp(stderr);
         exit(EXIT_FAILURE);
     }

     if (SSL_CTX_use_PrivateKey_file(ctx, "key.pem", SSL_FILETYPE_PEM) <= 0) {
         ERR_print_errors_fp(stderr);
         exit(EXIT_FAILURE);
     }
}

void configure_client_context(SSL_CTX *ctx)
{
    /*
     * Configure the client to abort the handshake if certificate verification
     * fails
     */
    SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER, NULL);
    /*
     * In a real application you would probably just use the default system certificate trust store and call:
     *     SSL_CTX_set_default_verify_paths(ctx);
     * In this demo though we are using a self-signed certificate, so the client must trust it directly.
     */
    if (!SSL_CTX_load_verify_locations(ctx, "cert.pem", NULL)) {
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }
}

// TODO: usage

void run_server() {
    // TODO: support non-TLS mode
    SSL_CTX *ctx = NULL;
    SSL *ssl = NULL;

    int server_skt = -1;
    int client_skt = -1;

    char rxbuf[MAX_BUF_SIZE];
    int rxcap = sizeof(rxbuf);
    int rxlen;

    char *rem_server_ip = NULL;

    struct sockaddr_in addr;
    unsigned int addr_len = sizeof(addr);

    ctx = create_context(true);

    printf("We are the server on port: %d\n\n", server_port);

    /* Configure server context with appropriate key files */
    configure_server_context(ctx);

    /* Create server socket; will bind with server port and listen */
    server_skt = create_socket(true);

    /*
     * Loop to accept clients.
     * Need to implement timeouts on TCP & SSL connect/read functions
     * before we can catch a CTRL-C and kill the server.
     */
    while (is_running) {
        /* Wait for TCP connection from client */
        client_skt = accept(server_skt, (struct sockaddr*) &addr,
                            &addr_len);
        if (client_skt < 0) {
            perror("Unable to accept");
            exit(EXIT_FAILURE);
        }

        printf("Client TCP connection accepted\n");

        /* Create server SSL structure using newly accepted client socket */
        ssl = SSL_new(ctx);
        SSL_set_fd(ssl, client_skt);

        /* Wait for SSL connection from the client */
        if (SSL_accept(ssl) <= 0) {
            ERR_print_errors_fp(stderr);
            is_running = false;
        } else {

            printf("Client SSL connection accepted\n\n");

            /* Echo loop */
            while (true) {
                /* Get message from client; will fail if client closes connection */
                if ((rxlen = SSL_read(ssl, rxbuf, rxcap)) <= 0) {
                    if (rxlen == 0) {
                        printf("Client closed connection\n");
                    }
                    ERR_print_errors_fp(stderr);
                    break;
                }
                /* Insure null terminated input */
                rxbuf[rxlen] = 0;
                /* Look for kill switch */
                if (strcmp(rxbuf, "kill\n") == 0) {
                    /* Terminate...with extreme prejudice */
                    printf("Server received 'kill' command\n");
                    is_running = false;
                    break;
                }
                /* Show received message */
                printf("Received: %s", rxbuf);
                /* Echo it back */
                if (SSL_write(ssl, rxbuf, rxlen) <= 0) {
                    ERR_print_errors_fp(stderr);
                }
            }
        }
        if (is_running) {
            /* Cleanup for next client */
            SSL_shutdown(ssl);
            SSL_free(ssl);
            close(client_skt);
        }
    }
    printf("Server exiting...\n");

    exit:
    if (ssl != NULL) {
        SSL_shutdown(ssl);
        SSL_free(ssl);
    }
    SSL_CTX_free(ctx);

    if (server_skt != -1) {
        close(server_skt);
    }
    if (client_skt != -1) {
        close(client_skt);
    }
}

int main() {
    /* Splash */
    printf("\ndead-simple-db : Simple Server (w/ OpenSSL 3.0.1-dev) : %s : %s\n\n", __DATE__,
           __TIME__);

    // TODO: support a client!
    bool is_server = true;


    if (is_server) {
        run_server();
    } else {
        // TODO
    }

    printf("Exiting simple db...\n\n");

    return 0;
}
