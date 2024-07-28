#include <gtk/gtk.h>
#include <stdlib.h>

GtkWidget *text_view;
GtkTextBuffer *buffer;
gchar *current_file = NULL;

void on_open_file(GtkWidget *widget, gpointer window)
{
    GtkWidget *dialog;
    dialog = gtk_file_chooser_dialog_new("Open File",
                                         GTK_WINDOW(window),
                                         GTK_FILE_CHOOSER_ACTION_OPEN,
                                         "_Cancel", GTK_RESPONSE_CANCEL,
                                         "_Open", GTK_RESPONSE_ACCEPT,
                                         NULL);

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT)
    {
        char *filename;
        FILE *file;
        char *content;
        size_t length;

        filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
        file = fopen(filename, "r");
        if (file)
        {
            fseek(file, 0, SEEK_END);
            length = ftell(file);
            fseek(file, 0, SEEK_SET);
            content = (char *)malloc(length + 1);
            if (content)
            {
                fread(content, 1, length, file);
                content[length] = '\0';
                fclose(file);

                buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
                gtk_text_buffer_set_text(buffer, content, -1);

                if (current_file)
                {
                    g_free(current_file);
                }
                current_file = g_strdup(filename);

                free(content);
            }
            else
            {
                g_printerr("Failed to allocate memory for file content\n");
            }
        }
        else
        {
            g_printerr("Failed to open file %s\n", filename);
        }
        g_free(filename);
    }

    gtk_widget_destroy(dialog);
}

void on_save_file_as(GtkWidget *widget, gpointer window)
{
    GtkWidget *dialog;
    dialog = gtk_file_chooser_dialog_new("Save File As",
                                         GTK_WINDOW(window),
                                         GTK_FILE_CHOOSER_ACTION_SAVE,
                                         "_Cancel", GTK_RESPONSE_CANCEL,
                                         "_Save", GTK_RESPONSE_ACCEPT,
                                         NULL);

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT)
    {
        char *filename;
        FILE *file;
        GtkTextIter start, end;
        char *content;

        filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
        buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
        gtk_text_buffer_get_start_iter(buffer, &start);
        gtk_text_buffer_get_end_iter(buffer, &end);
        content = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);

        file = fopen(filename, "w");
        fwrite(content, 1, strlen(content), file);
        fclose(file);

        if (current_file)
        {
            g_free(current_file);
        }
        current_file = g_strdup(filename);

        g_free(filename);
        g_free(content);
    }

    gtk_widget_destroy(dialog);
}

void on_save_file(GtkWidget *widget, gpointer window)
{
    if (current_file)
    {
        FILE *file;
        GtkTextIter start, end;
        char *content;

        buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
        gtk_text_buffer_get_start_iter(buffer, &start);
        gtk_text_buffer_get_end_iter(buffer, &end);
        content = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);

        file = fopen(current_file, "w");
        fwrite(content, 1, strlen(content), file);
        fclose(file);

        g_free(content);
    }
    else
    {
        on_save_file_as(widget, window);
    }
}

void on_quit(GtkWidget *widget, gpointer window)
{
    if (current_file)
    {
        g_free(current_file);
    }
    gtk_widget_destroy(GTK_WIDGET(window));
}

void on_new_window(GtkWidget *widget, gpointer window)
{
    gchar *argv[] = {g_strdup("xnote"), NULL};
    GError *error = NULL;
    GSpawnFlags flags = G_SPAWN_SEARCH_PATH;
    g_spawn_async(NULL, argv, NULL, flags, NULL, NULL, NULL, &error);
    if (error)
    {
        g_printerr("Failed to open new window: %s\n", error->message);
        g_error_free(error);
    }
    g_free(argv[0]);
}

void on_toggle_day_night_mode(GtkWidget *widget, gpointer window)
{
    static gboolean dark_mode = FALSE;
    GtkSettings *settings = gtk_settings_get_default();
    g_object_set(settings, "gtk-application-prefer-dark-theme", dark_mode, NULL);
    dark_mode = !dark_mode;
}

void update_font_size(GtkWidget *widget, int change)
{
    static GtkCssProvider *provider = NULL;
    static guint id = 0;
    static int font_size = 12; // Default font size

    if (provider == NULL)
    {
        provider = gtk_css_provider_new();
        gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
                                                  GTK_STYLE_PROVIDER(provider),
                                                  GTK_STYLE_PROVIDER_PRIORITY_USER);
    }

    font_size += change;
    gchar *css = g_strdup_printf("GtkTextView { font-size: %dpt; }", font_size);
    gtk_css_provider_load_from_data(provider, css, -1, NULL);
    g_free(css);
}

void on_increase_font_size(GtkWidget *widget, gpointer window)
{
    update_font_size(text_view, 2); // Increase font size
}

void on_decrease_font_size(GtkWidget *widget, gpointer window)
{
    update_font_size(text_view, -2); // Decrease font size
}

int main(int argc, char *argv[])
{
    GtkWidget *window;
    GtkWidget *vbox;
    GtkWidget *menubar;
    GtkWidget *filemenu;
    GtkWidget *viewmenu;
    GtkWidget *file;
    GtkWidget *view;
    GtkWidget *open;
    GtkWidget *save;
    GtkWidget *save_as;
    GtkWidget *quit;
    GtkWidget *new;
    GtkWidget *toggle_day_night;
    GtkWidget *increase_font;
    GtkWidget *decrease_font;
    GtkWidget *scroll;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "xNote");
    gtk_window_set_default_size(GTK_WINDOW(window), 600, 400);

    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    menubar = gtk_menu_bar_new();
    filemenu = gtk_menu_new();
    viewmenu = gtk_menu_new();

    file = gtk_menu_item_new_with_label("File");
    view = gtk_menu_item_new_with_label("View");
    open = gtk_menu_item_new_with_label("Open");
    save = gtk_menu_item_new_with_label("Save");
    save_as = gtk_menu_item_new_with_label("Save As");
    quit = gtk_menu_item_new_with_label("Quit");
    new = gtk_menu_item_new_with_label("New Window");
    toggle_day_night = gtk_menu_item_new_with_label("Toggle Day/Night Mode");
    increase_font = gtk_menu_item_new_with_label("Increase Font Size");
    decrease_font = gtk_menu_item_new_with_label("Decrease Font Size");

    gtk_menu_item_set_submenu(GTK_MENU_ITEM(file), filemenu);
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(view), viewmenu);
    gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), open);
    gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), save);
    gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), save_as);
    gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), new);
    gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), quit);
    gtk_menu_shell_append(GTK_MENU_SHELL(viewmenu), toggle_day_night);
    gtk_menu_shell_append(GTK_MENU_SHELL(viewmenu), increase_font);
    gtk_menu_shell_append(GTK_MENU_SHELL(viewmenu), decrease_font);
    gtk_menu_shell_append(GTK_MENU_SHELL(menubar), file);
    gtk_menu_shell_append(GTK_MENU_SHELL(menubar), view);

    gtk_box_pack_start(GTK_BOX(vbox), menubar, FALSE, FALSE, 0);

    text_view = gtk_text_view_new();
    scroll = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(scroll), text_view);
    gtk_box_pack_start(GTK_BOX(vbox), scroll, TRUE, TRUE, 0);

    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));

    g_signal_connect(G_OBJECT(open), "activate", G_CALLBACK(on_open_file), window);
    g_signal_connect(G_OBJECT(save), "activate", G_CALLBACK(on_save_file), window);
    g_signal_connect(G_OBJECT(save_as), "activate", G_CALLBACK(on_save_file_as), window);
    g_signal_connect(G_OBJECT(quit), "activate", G_CALLBACK(on_quit), window);
    g_signal_connect(G_OBJECT(new), "activate", G_CALLBACK(on_new_window), window);
    g_signal_connect(G_OBJECT(toggle_day_night), "activate", G_CALLBACK(on_toggle_day_night_mode), window);
    g_signal_connect(G_OBJECT(increase_font), "activate", G_CALLBACK(on_increase_font_size), window);
    g_signal_connect(G_OBJECT(decrease_font), "activate", G_CALLBACK(on_decrease_font_size), window);
    g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
