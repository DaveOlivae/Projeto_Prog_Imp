#include <gtk/gtk.h>
#include "principal.c"

static void principal(GtkApplication *app, gpointer user_data);

static void botao_clicado2(GtkWidget *button, gpointer user_data) {
    GtkGrid *grid = GTK_GRID(user_data);

    GtkApplication *app;
    GtkWidget *window;

    GtkWidget *parent = gtk_widget_get_parent(GTK_WIDGET(grid));
    
    // Itera até encontrar a janela
    while (parent != NULL && !GTK_IS_WINDOW(parent)) {
        parent = gtk_widget_get_parent(parent);
    }

    window = parent;  // Atribui a janela ao ponteiro
    app = gtk_window_get_application(GTK_WINDOW(window));  // Obtém o GtkApplication a partir da janela

    gtk_window_destroy(GTK_WINDOW(window));

    principal(app, NULL);
}

static void limpar(GtkWidget *grid) {
    // limpa os widgets do grid

    GtkWidget *iter = gtk_widget_get_first_child (grid);

    while (iter != NULL) {
        GtkWidget *next = gtk_widget_get_next_sibling (iter);
        gtk_grid_remove (GTK_GRID(grid), iter);
        iter = next;
    }
}

static void cadastro(GtkWidget *button, gpointer user_data); 

static void tela_login(GtkGrid *grid) {

     // cria a tela de login
    
    limpar(GTK_WIDGET(grid));

    GtkWidget *image1;
    GtkWidget *image2;
    GtkWidget *titulo;
    GtkWidget *log_label;
    GtkWidget *senha_label;
    GtkWidget *login;
    GtkWidget *senha;
    GtkWidget *botao_entrar;
    GtkWidget *botao_cadastro;

    // logo da upe
    image1 = gtk_image_new_from_file("LOGO-UPE-nobg.png");
    gtk_widget_set_size_request(image1, 230, 130);
    gtk_grid_attach(grid, image1, 0, 0, 1, 1);

    // logo da poli
    image2 = gtk_image_new_from_file("Poliupe-nobg.png");
    gtk_widget_set_size_request(image2, 230, 130);
    gtk_grid_attach(GTK_GRID(grid), image2, 4, 0, 1, 1);

    // titulo
    titulo = gtk_label_new("Controle de Salas");
    gtk_grid_attach(GTK_GRID(grid), titulo, 2, 0, 2, 1);

    // label de login
    log_label = gtk_label_new("Login");
    gtk_grid_attach(GTK_GRID(grid), log_label, 1, 1, 1, 1);

    // label de senha
    senha_label = gtk_label_new("Senha");
    gtk_grid_attach(GTK_GRID(grid), senha_label, 1, 2, 1, 1);

    // entry de login
    login = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), login, 2, 1, 1, 1);

    // entry de senha
    senha = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), senha, 2, 2, 1, 1);

    // botao de entrar
    botao_entrar = gtk_button_new_with_label("Entrar");
    gtk_grid_attach(GTK_GRID(grid), botao_entrar, 1, 3, 2, 1);
    g_signal_connect(botao_entrar, "clicked", G_CALLBACK(botao_clicado2), GTK_GRID(grid));

    // botao de cadastro
    botao_cadastro = gtk_button_new_with_label("Cadastro");
    gtk_grid_attach(GTK_GRID(grid), botao_cadastro, 1, 5, 1, 1);
    g_signal_connect(botao_cadastro, "clicked", G_CALLBACK(cadastro), GTK_GRID(grid));
}

static void botao_clicado(GtkWidget *button, gpointer user_data) {
    GtkGrid *grid = GTK_GRID(user_data);
    tela_login(GTK_GRID(grid));
}

static void cadastro(GtkWidget *button, gpointer user_data) {
    // modifica a tela de login para a tela de cadastro, alguns widgets sao removidos e outros adicionados

    GtkGrid *grid = GTK_GRID(user_data);

    GtkWidget *label1 = gtk_grid_get_child_at(grid, 1, 1);
    gtk_label_set_text(GTK_LABEL(label1), "Nome");

    GtkWidget *label2 = gtk_grid_get_child_at(grid, 1, 2);
    gtk_label_set_text(GTK_LABEL(label2), "Login");

    GtkWidget *buttoremove = gtk_grid_get_child_at(grid, 1, 3);
    gtk_grid_remove(GTK_GRID(grid), buttoremove);

    GtkWidget *label_senha = gtk_label_new("Senha");
    gtk_grid_attach(GTK_GRID(grid), label_senha, 1, 3, 1, 1);

    GtkWidget *senh = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), senh, 2, 3, 1, 1);

    GtkWidget *confirm = gtk_label_new("Confirmar Senha");
    gtk_grid_attach(GTK_GRID(grid), confirm, 1, 4, 1, 1);

    GtkWidget *confirm_senh = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), confirm_senh, 2, 4, 1, 1);

    GtkWidget *registr = gtk_button_new_with_label("Registrar");
    gtk_grid_attach(GTK_GRID(grid), registr, 1, 5, 2, 1);
    g_signal_connect(registr, "clicked", G_CALLBACK(botao_clicado), grid);

    GtkWidget *cancelar = gtk_button_new_with_label("Cancelar");
    gtk_grid_attach(GTK_GRID(grid), cancelar, 1, 6, 1, 1);
    g_signal_connect(cancelar, "clicked", G_CALLBACK(botao_clicado), grid);
}

static void login(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *grid;

    // setup da janela
    window = gtk_application_window_new (app);
    gtk_window_set_title (GTK_WINDOW (window), "Window");
    gtk_window_set_default_size(GTK_WINDOW(window), 500, 400);

    // setup do grid
    grid = gtk_grid_new();
    gtk_window_set_child(GTK_WINDOW(window), grid);

    tela_login(GTK_GRID(grid));
    gtk_window_present (GTK_WINDOW (window));  
}

