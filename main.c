#include <gtk/gtk.h>

static void registro (GtkWidget *widget, gpointer data) {

}

static void modificar (GtkWidget *widget, gpointer data) {
    GtkWidget *window_modificar;

    // setup da janela de modificar
    window_modificar = gtk_window_new ();
    gtk_window_set_title (GTK_WINDOW (window_modificar), "Modificar");
    gtk_window_set_default_size(GTK_WINDOW(window_modificar), 400, 300);

    gtk_window_present (GTK_WINDOW (window_modificar));
}

static void activate (GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *botao_registro;
    GtkWidget *botao_modificar;
    GtkWidget *sala[35];
    GtkWidget *horario[18];
    GtkWidget *labels_vazias[3];
    GtkWidget *botoes[630];
    GtkWidget *scr;

    char *salas[] = {"A03", "B01", "B02", "B03", "B04", "B09", "I04", "I06", "I08", "I09", "I10", "I11", "I12", "I13", "I14", "I15", "K03", "K04", "K05", "K09", "K10", "K11", "K12", "K13", "K14", "K15", "K16", "LIP01", "LIP02", "LIP03", "LIP07", "J01", "J08", "LMC", "LMS"};

    char *horarios[] = {" 07:10 - 08:00 ", " 08:00 - 08:50 ", " 08:50 - 09:40 ", " 09:40 - 10:30 ", " 10:30 - 11:20 ", " 11:20 - 12:10 ", " 12:10 - 13:00 ", " 13:00 - 13:50 ", " 13:50 - 14:40 ", " 14:40 - 15:30 ", " 15:30 - 16:20 ", " 16:20 - 17:10 ", " 17:10 - 18:00 ", " 18:00 - 18:50 ", " 18:50 - 19:40 ", " 19:40 - 20:30 ", " 20:30 - 21:20 ", " 21:20 - 22:10 "};

    // setup da janela
    window = gtk_application_window_new (app);
    gtk_window_set_title (GTK_WINDOW (window), "Window");
    gtk_window_set_default_size(GTK_WINDOW(window), 1000, 1000);

    // setup do scroll
    scr = gtk_scrolled_window_new();
    gtk_window_set_child(GTK_WINDOW(window), scr);

    // setup do grid
    grid = gtk_grid_new();
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scr), grid);

    // label vazia
    labels_vazias[0] = gtk_label_new(" ");
    gtk_grid_attach(GTK_GRID(grid), labels_vazias[0], 0, 0, 1, 1);

    labels_vazias[1] = gtk_label_new(" ");
    gtk_grid_attach(GTK_GRID(grid), labels_vazias[1], 4, 2, 1, 1);

    labels_vazias[2] = gtk_label_new(" ");
    gtk_grid_attach(GTK_GRID(grid), labels_vazias[2], 0, 4, 1, 1);

    // setup das labels
    for (int i = 0; i < 35; i++){
        sala[i] = gtk_label_new(salas[i]);
        gtk_grid_attach(GTK_GRID(grid), sala[i], 1, i+4, 1, 1);
    }

    // setup dos horarios
    for (int i = 0; i < 18; i++) {
        horario[i] = gtk_label_new(horarios[i]);
        gtk_grid_attach(GTK_GRID(grid), horario[i], i+2, 3, 1, 1);
    }
    // setup do botao de registro
    botao_registro = gtk_button_new_with_label("Registrar");
    g_signal_connect(botao_registro, "clicked", G_CALLBACK(registro), NULL);
    gtk_grid_attach(GTK_GRID(grid), botao_registro, 2, 1, 2, 1);

    // setup do botao de modificar
    botao_modificar = gtk_button_new_with_label("Modificar");
    g_signal_connect(botao_modificar, "clicked", G_CALLBACK(modificar), NULL);
    gtk_grid_attach(GTK_GRID(grid), botao_modificar, 5, 1, 2, 1);

    // setup dos botoes
    for (int i = 0; i < 35; i++) {
        for (int j = 0; j < 18; j++) {
            botoes[j] = gtk_link_button_new_with_label("livre", NULL);
            gtk_grid_attach(GTK_GRID(grid), botoes[j], j+2, i+4, 1, 1);
        }
    }

    gtk_window_present (GTK_WINDOW (window));

}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    app = gtk_application_new ("teste.teste", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
    status = g_application_run (G_APPLICATION (app), argc, argv);
    g_object_unref (app);

    return status;
}

