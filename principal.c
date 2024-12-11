#include <gtk/gtk.h>

static const char *const salas[] = {
    "A03", "B01", "B02", "B03", "B04", "B09", "I04", 
    "I06", "I08", "I09", "I10", "I11", "I12", "I13", 
    "I14", "I15", "K03", "K04", "K05", "K09", "K10", 
    "K11", "K12", "K13", "K14", "K15", "K16", "LIP01", 
    "LIP02", "LIP03", "LIP07", "J01", "J08", "LMC", "LMS", NULL
};

static char *horarios_inicio[] = {
    "07:10", "08:00", "08:50", "09:40", "10:30",
    "11:20", "12:10", "13:00", "13:50", "14:40",
    "15:30", "16:20", "17:10", "18:00", "18:50",
    "19:40", "20:30", "21:20", "22:10"
};

static void teste(GtkButton *button, gpointer user_data) {
    GtkWidget *window_teste;
    
    // setup da janela de modificar
    window_teste = gtk_window_new ();
    gtk_window_set_title (GTK_WINDOW (window_teste), "teste");
    gtk_window_set_default_size(GTK_WINDOW(window_teste), 400, 300);

    gtk_window_present (GTK_WINDOW (window_teste));
}

// static const char* horario_selecionado(GtkDropDown *dropdown) {
    // guint posicao_selecionada = gtk_drop_down_get_selected(dropdown);

    // const char *item_selecionado = gtk_string_list_get_string(
    //    GTK_STRING_LIST(gtk_drop_down_get_model(dropdown)),
    //    posicao_selecionada
    // );

    return posicao_selecionada;
}

// Callback para capturar a data selecionada
static void date_selected(GtkCalendar *calendar, gpointer user_data) {
    // Obtém a data selecionada como GDateTime
    GDateTime *date = gtk_calendar_get_date(GTK_CALENDAR(calendar));

    // Formata a data
    gchar *date_text = g_date_time_format(date, "%d/%m/%Y");

    // Atualiza o texto do botão com a data selecionada
    GtkWidget *button = GTK_WIDGET(user_data);
    gtk_button_set_label(GTK_BUTTON(button), date_text);

    // Libera a memória
    g_free(date_text);
    g_date_time_unref(date);

    // Fecha o popover
    GtkWidget *popover = gtk_widget_get_parent(GTK_WIDGET(calendar));
    gtk_popover_popdown(GTK_POPOVER(popover));
}

static void popover_botao(GtkButton *button, gpointer user_data) {
    GtkPopover *popover = GTK_POPOVER(user_data);
    gtk_popover_popup(popover);
}

// ################# JANELA DE MODIFICAR CASO MONITORIA SIM ##############################################################

static void check_clicado_sim(GtkCheckButton *check_button, gpointer user_data) {
    GtkGrid *grid = GTK_GRID(user_data);

    GtkWidget *materia = gtk_label_new("Materia");
    gtk_grid_attach(GTK_GRID(grid), materia, 0, 6, 1, 1);

    GtkWidget *entry_materia = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), entry_materia, 1, 6, 1, 1);

    GtkWidget *nome_monitor = gtk_label_new("Nome do Monitor");
    gtk_grid_attach(GTK_GRID(grid), nome_monitor, 0, 7, 1, 1);

    GtkWidget *entry_monitor = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), entry_monitor, 1, 7, 1, 1);

    GtkWidget *nome_prof = gtk_label_new("Nome do Professor");
    gtk_grid_attach(GTK_GRID(grid), nome_prof, 0, 8, 1, 1);

    GtkWidget *entry_prof = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), entry_prof, 1, 8, 1, 1);
    
    GtkWidget *telefone = gtk_label_new("Telefone");
    gtk_grid_attach(GTK_GRID(grid), telefone, 0, 9, 1, 1);

    GtkWidget *entry_telefone = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), entry_telefone, 1, 9, 1, 1);
    
    GtkWidget *cpf = gtk_label_new("Cpf");
    gtk_grid_attach(GTK_GRID(grid), cpf, 0, 10, 1, 1);

    GtkWidget *entry_cpf = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), entry_cpf, 1, 10, 1, 1);
    
    GtkWidget *sala = gtk_label_new("Sala");
    gtk_grid_attach(GTK_GRID(grid), sala, 0, 11, 1, 1);
    
    GtkWidget *sala_drop = gtk_drop_down_new_from_strings(salas);
    gtk_grid_attach(GTK_GRID(grid), sala_drop, 1, 11, 1, 1);

    GtkWidget *botao_reg = gtk_button_new_with_label("Registrar");
    gtk_grid_attach(GTK_GRID(grid), botao_reg, 0, 12, 2, 1);

}

static void check_clicado_nao(GtkCheckButton *check_button, gpointer user_data) {
    
}

// ############################################# JANELA REGISTRAR HORARIO ##################################################

static void registro (GtkWidget *widget, gpointer data) {

    // abre uma janela pra registrar horarios

    GtkWidget *window_registro;
    GtkWidget *grid_registro;

    // setup da janela de modificar
    window_registro = gtk_window_new ();
    gtk_window_set_title (GTK_WINDOW (window_registro), "Registro");
    gtk_window_set_default_size(GTK_WINDOW(window_registro), 400, 300);

    // grid
    grid_registro = gtk_grid_new();
    gtk_window_set_child(GTK_WINDOW(window_registro), grid_registro);
    
    // titulo
    GtkWidget *titulo_registro = gtk_label_new("Registro de Evento");
    gtk_grid_attach(GTK_GRID(grid_registro), titulo_registro, 0, 0, 2, 1);
    gtk_widget_set_margin_top(titulo_registro, 30);
    gtk_widget_set_margin_bottom(titulo_registro, 30);
    gtk_widget_set_margin_start(titulo_registro, 10); 
    gtk_widget_set_margin_end(titulo_registro, 3);

    // label data
    GtkWidget *label_data = gtk_label_new("Data");
    gtk_grid_attach(GTK_GRID(grid_registro), label_data, 0, 1, 1, 1);

    // botao selecione data
    GtkWidget *button = gtk_button_new_with_label("Selecione uma data");
    gtk_grid_attach(GTK_GRID(grid_registro), button, 1, 1, 1, 1);
   
    // Cria o popover
    GtkWidget *popover = gtk_popover_new();
    gtk_widget_set_parent(popover, button);

    // calendario
    GtkWidget *calendario = gtk_calendar_new();
    gtk_popover_set_child(GTK_POPOVER(popover), calendario);

    // Conecta o botão ao popover
    g_signal_connect(button, "clicked", G_CALLBACK(popover_botao), popover);

    // Conecta o sinal do calendário para capturar a data selecionada
    g_signal_connect(calendario, "day-selected", G_CALLBACK(date_selected), button);

    // label horario de inicio
    GtkWidget *hor_inilab = gtk_label_new("Horario de Inicio");
    gtk_grid_attach(GTK_GRID(grid_registro), hor_inilab, 0, 2, 1, 1);

    const char *const teste[] = {"cachorro", NULL};
    // --------------- drop down e seus horarios --------------------------------
    
    // Usa GStrvBuilder para criar um array de strings NULL-terminated
    GStrvBuilder *builder = g_strv_builder_new();
   
    for (int i = 0; i < 18; i++) {
        g_strv_builder_add(builder, horarios_inicio[i]);
    }

    // Cria o GtkStringList a partir do array de strings
    char **strv = g_strv_builder_end(builder);
    GtkStringList *string_list = gtk_string_list_new((const char *const *)strv);
    g_strfreev(strv); // Libera o array de strings

    GtkWidget *hor_ini = gtk_drop_down_new(G_LIST_MODEL(string_list), NULL);
    gtk_grid_attach(GTK_GRID(grid_registro), hor_ini, 1, 2, 1, 1);

    guint posicao_selecionada = gtk_drop_down_get_selected(GTK_DROP_DOWN(hor_ini));
    
    // const char *horario_selec = horario_selecionado(GTK_DROP_DOWN(hor_ini));

    // label horario fim
    GtkWidget *hor_fimlab = gtk_label_new("Horario do Fim");
    gtk_grid_attach(GTK_GRID(grid_registro), hor_fimlab, 0, 3, 1, 1);
    
    //drop down horario de fim

    GStrvBuilder *builder2 = g_strv_builder_new();

    for (int i = 0; i < 18; i++) {
        if (i > posicao_selecionada)
            g_strv_builder_add(builder2, horarios_inicio[i]);
    }

    char **strv2 = g_strv_builder_end(builder2);
    GtkStringList *string_list2 = gtk_string_list_new((const char *const *)strv2);
    g_strfreev(strv2);

    GtkWidget *hor_fim = gtk_drop_down_new(G_LIST_MODEL(string_list2), NULL);
    gtk_grid_attach(GTK_GRID(grid_registro), hor_fim, 1, 3, 1, 1);

    // label monitoria?
    GtkWidget *monitoria_label = gtk_label_new("Monitoria?");
    gtk_grid_attach(GTK_GRID(grid_registro), monitoria_label, 0, 4, 2, 1);

    // check buttons sim e nao
    GtkWidget *radio_button1 = gtk_check_button_new_with_label("Sim");
    gtk_grid_attach(GTK_GRID(grid_registro), radio_button1, 0, 5, 1, 1);
    
    GtkWidget *radio_button2 = gtk_check_button_new_with_label("Nao");
    gtk_check_button_set_group(GTK_CHECK_BUTTON(radio_button2), GTK_CHECK_BUTTON(radio_button1));
    gtk_grid_attach(GTK_GRID(grid_registro), radio_button2, 1, 5, 1, 1);

    g_signal_connect(radio_button1, "toggled", G_CALLBACK(check_clicado_sim), grid_registro); 

    gtk_window_present (GTK_WINDOW (window_registro));

}

// ##################################################### JANELA DE MODIFICAR HORARIO ##################################################

static void modificar (GtkWidget *widget, gpointer data) {

    // abre uma janela pra modificar horarios

    GtkWidget *window_modificar;
    GtkWidget *grid_modificar;
    GtkWidget *mod[4];
    GtkWidget *botao_modificar2;

    const char *const mods[] = {"Sala:","Data:","Inicio:","Fim:", NULL};

    // setup da janela de modificar
    window_modificar = gtk_window_new ();
    gtk_window_set_title (GTK_WINDOW (window_modificar), "Modificar");
    gtk_window_set_default_size(GTK_WINDOW(window_modificar), 400, 300);

    // setup da grid
    grid_modificar = gtk_grid_new();
    gtk_window_set_child(GTK_WINDOW(window_modificar), grid_modificar);

    // titulo
    GtkWidget *titulo_modificar = gtk_label_new("Modificar Registro");
    gtk_grid_attach(GTK_GRID(grid_modificar), titulo_modificar, 0, 0, 2, 1);

    // label sala
    GtkWidget *sala_label = gtk_label_new("Sala");
    gtk_grid_attach(GTK_GRID(grid_modificar), sala_label, 0, 1, 1, 1);
    
    // drop down sala
    GtkWidget *sala_drop = gtk_drop_down_new_from_strings(mods);
    gtk_grid_attach(GTK_GRID(grid_modificar), sala_drop, 1, 1, 1, 1);
   
    // label data
    GtkWidget *data_label = gtk_label_new("Data");
    gtk_grid_attach(GTK_GRID(grid_modificar), data_label, 0, 2, 1, 1);

    // label horario inicio
    GtkWidget *hor_ini_label = gtk_label_new("Horario de Inicio");
    gtk_grid_attach(GTK_GRID(grid_modificar), hor_ini_label, 0, 3, 1, 1);

    // drop down horario inicio
    GtkWidget *hor_ini_drop = gtk_drop_down_new_from_strings(mods);
    gtk_grid_attach(GTK_GRID(grid_modificar), hor_ini_drop, 1, 3, 1, 1);
    
    // label horario fim
    GtkWidget *hor_fim_label = gtk_label_new("Horario de Fim");
    gtk_grid_attach(GTK_GRID(grid_modificar), hor_fim_label, 0, 4, 1, 1);

    // drop down horario fim
    GtkWidget *hor_fim_drop = gtk_drop_down_new_from_strings(mods);
    gtk_grid_attach(GTK_GRID(grid_modificar), hor_fim_drop, 1, 4, 1, 1);

    // ############## setup do calendario da tela de modificar ###############

    // cria o botao de selecionar a data
    GtkWidget *botao = gtk_button_new_with_label("Selecione a Data");
    gtk_grid_attach(GTK_GRID(grid_modificar), botao, 1, 2, 1, 1);

    // Cria o popover
    GtkWidget *popover = gtk_popover_new();
    gtk_widget_set_parent(popover, botao);

    // calendario
    GtkWidget *calendario = gtk_calendar_new();
    gtk_popover_set_child(GTK_POPOVER(popover), calendario);

    // Conecta o botão ao popover
    g_signal_connect(botao, "clicked", G_CALLBACK(popover_botao), popover);

    // Conecta o sinal do calendário para capturar a data selecionada
    g_signal_connect(calendario, "day-selected", G_CALLBACK(date_selected), botao);

    // setup do botao
    botao_modificar2 = gtk_button_new_with_label("Verificar Evento");
    gtk_grid_attach(GTK_GRID(grid_modificar), botao_modificar2, 0, 5, 2, 1);

    gtk_window_present (GTK_WINDOW (window_modificar)); 
}

// ################################################### JANELA PRINCIPAL ####################################################

static void principal(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *botao_registro;
    GtkWidget *botao_modificar;
    GtkWidget *sala[35];
    GtkWidget *horario[18];
    GtkWidget *labels_vazias[3];
    GtkWidget *botoes[630];
    GtkWidget *scr;

    char *horarios[] = {
        " 07:10 - 08:00 ", " 08:00 - 08:50 ", " 08:50 - 09:40 ", " 09:40 - 10:30 ", 
        " 10:30 - 11:20 ", " 11:20 - 12:10 ", " 12:10 - 13:00 ", " 13:00 - 13:50 ", 
        " 13:50 - 14:40 ", " 14:40 - 15:30 ", " 15:30 - 16:20 ", " 16:20 - 17:10 ", 
        " 17:10 - 18:00 ", " 18:00 - 18:50 ", " 18:50 - 19:40 ", " 19:40 - 20:30 ", 
        " 20:30 - 21:20 ", " 21:20 - 22:10 "
    };

    // setup da janela
    window = gtk_application_window_new (app);
    gtk_window_set_title (GTK_WINDOW (window), "Controle de Salas");
    gtk_window_maximize(GTK_WINDOW(window));

    // setup do scroll
    scr = gtk_scrolled_window_new();
    gtk_window_set_child(GTK_WINDOW(window), scr);

    // setup do grid
    grid = gtk_grid_new();
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scr), grid);

    // setup das labels das salas
    for (int i = 0; i < 35; i++){
        sala[i] = gtk_label_new(salas[i]);
        gtk_grid_attach(GTK_GRID(grid), sala[i], 1, i+4, 1, 1);
        gtk_widget_set_margin_top(sala[i], 3);
        gtk_widget_set_margin_bottom(sala[i], 3);
        gtk_widget_set_margin_start(sala[i], 50); 
        gtk_widget_set_margin_end(sala[i], 3);
    };

    // setup dos horarios
    for (int i = 0; i < 18; i++) {
        horario[i] = gtk_label_new(horarios[i]);
        gtk_grid_attach(GTK_GRID(grid), horario[i], i+2, 3, 1, 1);
        gtk_widget_set_margin_top(horario[i], 30);
        gtk_widget_set_margin_bottom(horario[i], 3);
        gtk_widget_set_margin_start(horario[i], 5); 
        gtk_widget_set_margin_end(horario[i], 5);
    }
    // setup do botao de registro
    botao_registro = gtk_button_new_with_label("Registrar");
    g_signal_connect(botao_registro, "clicked", G_CALLBACK(registro), NULL);
    gtk_grid_attach(GTK_GRID(grid), botao_registro, 2, 1, 2, 1);
    gtk_widget_set_margin_top(botao_registro, 30);
    gtk_widget_set_margin_bottom(botao_registro, 10);
    gtk_widget_set_margin_start(botao_registro, 3); 
    gtk_widget_set_margin_end(botao_registro, 3);

    // setup do botao de modificar
    botao_modificar = gtk_button_new_with_label("Modificar");
    g_signal_connect(botao_modificar, "clicked", G_CALLBACK(modificar), NULL);
    gtk_grid_attach(GTK_GRID(grid), botao_modificar, 5, 1, 2, 1);
    gtk_widget_set_margin_top(botao_modificar, 30);
    gtk_widget_set_margin_bottom(botao_modificar, 10);
    gtk_widget_set_margin_start(botao_modificar, 3); 
    gtk_widget_set_margin_end(botao_modificar, 3);

    // setup dos botoes
    for (int i = 0; i < 35; i++) {
        for (int j = 0; j < 18; j++) {
            botoes[j] = gtk_button_new_with_label("livre");
            g_signal_connect(botoes[j], "clicked", G_CALLBACK(teste), NULL);
            gtk_grid_attach(GTK_GRID(grid), botoes[j], j+2, i+4, 1, 1);
        }
    }

    gtk_window_present (GTK_WINDOW (window));

}

