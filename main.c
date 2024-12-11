#include <gtk/gtk.h>
#include "login.c"

static void login(GtkApplication *app, gpointer user_data);

static void on_activate(GtkApplication *app, gpointer user_data) {
    // Abre a tela de login
    login(app, NULL);
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    app = gtk_application_new ("controle.salas", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect (app, "activate", G_CALLBACK (on_activate), NULL);
    status = g_application_run (G_APPLICATION (app), argc, argv);
    g_object_unref (app);

    return status;
}

