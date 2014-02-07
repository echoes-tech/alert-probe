#include <stdlib.h>
#include <stdio.h>
#include <glib.h>
#include <gio/gio.h>
#include <signal.h>

GMainLoop* loop = NULL;

gchar* errorLogFile = "testsuite/server/server.log";
gchar* resultFile = "testsuite/server/server.result";
gchar* tls_cert_file = "testsuite/server/ssl/servunittest.crt";
gchar* tls_cert_key = "testsuite/server/ssl/servunittest.key";

void log2file(const gchar *log_domain, GLogLevelFlags log_level, const gchar *message, gpointer file_path)
{
    FILE *logFile = fopen((char*)file_path, "a");
    if (logFile != NULL)
    {
        fprintf(logFile, "%s", message);
    	fclose(logFile);
    }
    else 
    {
        printf ("Error when opening file %s\n", (char*)file_path);
    }
}

/* this function will get called everytime a client attempts to connect */
gboolean
incoming_callback(GSocketService *service,
                  GTlsConnection *connection,
                  GObject *source_object,
                  gpointer user_data)
{
    GError * error = NULL;
    gchar *message = calloc(1024, sizeof (char));
    GTlsCertificate *tlscert = NULL;
    GIOStream *tls_conn;
    tlscert = g_tls_certificate_new_from_files(tls_cert_file, tls_cert_key, &error);
    if (!tlscert)
    {
        g_critical("Could not read server certificate '%s': %s\n", tls_cert_file, error->message);
        return 1;
    }

    tls_conn = g_tls_server_connection_new(G_IO_STREAM(connection), tlscert, &error);
    if (!tls_conn)
    {
        g_critical("Could not create TLS connection: %s\n", error->message);
        return 1;
    }

    if (!g_tls_connection_handshake(G_TLS_CONNECTION(tls_conn),
                                    NULL, &error))
    {
        g_critical("Error during TLS handshake: %s\n", error->message);
        return 1;
    }

    GInputStream * istream = g_io_stream_get_input_stream(tls_conn);

    g_input_stream_read(istream, message, 1024, NULL, &error);
    if (error != NULL)
    {
        g_critical("Error during read stream: %s\n", error->message);
    }

    log2file(G_LOG_DOMAIN, G_LOG_LEVEL_MASK, message, resultFile);

    free(message);

    return FALSE;
}

void signalHandling(int signum)
{
    g_main_loop_quit(loop);
}

int main(int argc, char **argv)
{
    GError * error = NULL;
    
    /* create the new socketservice */
    GSocketService * service = g_socket_service_new();
    
    g_type_init();

    /* handler for gmessage */
    g_log_set_handler(G_LOG_DOMAIN, G_LOG_LEVEL_MASK, log2file, (gpointer) errorLogFile);
    
    /* add date in log file */
    GDateTime *dateTime = g_date_time_new_now_local();
    gchar *dateTimeFormat = g_date_time_format(dateTime, "%FT%T%z");
    g_message("%s\n", dateTimeFormat);

    /* connect to the port */
    g_socket_listener_add_inet_port((GSocketListener*) service, 1900, NULL, &error);
    if (error != NULL)
    {
        g_critical("Error during connection to the por: %s\n", error->message);
    }

    /* listen to the 'incoming' signal */
    g_signal_connect(service, "incoming", G_CALLBACK(incoming_callback), NULL);

    /* start the socket service */
    g_socket_service_start(service);

    /* handler for SIGTERM */
    signal(SIGTERM, signalHandling);
    
    /* remove result file */
    remove (resultFile);

    loop = g_main_loop_new(NULL, FALSE);

    /* loop stopped by calling g_main_loop_quit() when SIGTERM catched */
    g_main_loop_run(loop);

    /* stop and clean */
    g_socket_service_stop(service);
    g_socket_listener_close((GSocketListener*) service);

    g_main_loop_unref(loop);
    
    log2file(G_LOG_DOMAIN, G_LOG_LEVEL_MASK, "end", resultFile);

    return 0;
}
