#include <gtk/gtk.h>
#include <string.h>
#include <stdio.h>

GtkWidget *text_view;
GtkTextBuffer *buffer;

void open_file(GtkWidget *widget, gpointer data) {
    GtkWidget *dialog;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
    gint res;

    dialog = gtk_file_chooser_dialog_new("Open File", GTK_WINDOW(gtk_widget_get_toplevel(widget)), action, "_Open", GTK_RESPONSE_ACCEPT, "_Cancel", GTK_RESPONSE_CANCEL, NULL);

    res = gtk_dialog_run(GTK_DIALOG(dialog));
    if (res == GTK_RESPONSE_ACCEPT) {
        char *filename;
        filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
        FILE *file = fopen(filename, "r");
        if (file!= NULL) {
            gchar *contents;
            gsize length;
            GError *error = NULL;

            if (g_file_get_contents(filename, &contents, &length, &error) == TRUE) {
                gtk_text_buffer_set_text(buffer, contents, length);
                g_free(contents);
            } else {
                fprintf(stderr, "Error: %s\n", error->message);
                g_error_free(error);
            }
        }
        fclose(file);
    }

    gtk_widget_destroy(dialog);
}

void save_file(GtkWidget *widget, gpointer data) {
    GtkWidget *dialog;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_SAVE;
    gint res;

    dialog = gtk_file_chooser_dialog_new("Save File", GTK_WINDOW(gtk_widget_get_toplevel(widget)), action, "_Save", GTK_RESPONSE_ACCEPT, "_Cancel", GTK_RESPONSE_CANCEL, NULL);

    res = gtk_dialog_run(GTK_DIALOG(dialog));
    if (res == GTK_RESPONSE_ACCEPT) {
        char *filename;
        filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
        FILE *file = fopen(filename, "w");
        if (file!= NULL) {
            GtkTextIter start, end; // Define GtkTextIter variables
            gchar *contents;
            gsize length;
            gtk_text_buffer_get_bounds(buffer, &start, &end); // Get the bounds of the buffer
            contents = gtk_text_buffer_get_text(buffer, &start, &end, FALSE); // Get the text in the buffer
            length = strlen(contents);
            fwrite(contents, 1, length, file); // Write the contents to the file
            g_free(contents); // Free the allocated memory
            fclose(file);
        }
    }

    gtk_widget_destroy(dialog);
}


void new_file(GtkWidget *widget, gpointer data) {
    gtk_text_buffer_set_text(buffer, "", 0);
}

void exit_app(GtkWidget *widget, gpointer data) {
    gtk_main_quit();
}

int main(int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *vbox;
    GtkWidget *menubar;
    GtkWidget *file_menu;
    GtkWidget *file_menu_item;
    GtkWidget *open_menu_item;
    GtkWidget *save_menu_item;
    GtkWidget *new_menu_item;
    GtkWidget *exit_menu_item;
    GtkWidget *scrolled_window;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Notepad");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);

    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    menubar = gtk_menu_bar_new();
    gtk_box_pack_start(GTK_BOX(vbox), menubar, FALSE, FALSE, 0);

    file_menu = gtk_menu_new();
    file_menu_item = gtk_menu_item_new_with_label("File");
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(file_menu_item), file_menu);
    gtk_menu_shell_append(GTK_MENU_SHELL(menubar), file_menu_item);

    open_menu_item = gtk_menu_item_new_with_label("Open");
    gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), open_menu_item);
    g_signal_connect(open_menu_item, "activate", G_CALLBACK(open_file), NULL);

    save_menu_item = gtk_menu_item_new_with_label("Save");
    gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), save_menu_item);
    g_signal_connect(save_menu_item, "activate", G_CALLBACK(save_file), NULL);

    new_menu_item = gtk_menu_item_new_with_label("New");
    gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), new_menu_item);
    g_signal_connect(new_menu_item, "activate", G_CALLBACK(new_file), NULL);

    exit_menu_item = gtk_menu_item_new_with_label("Exit");
    gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), exit_menu_item);
    g_signal_connect(exit_menu_item, "activate", G_CALLBACK(exit_app), NULL);

    scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_box_pack_start(GTK_BOX(vbox), scrolled_window, TRUE, TRUE, 0);

    text_view = gtk_text_view_new();
    gtk_container_add(GTK_CONTAINER(scrolled_window), text_view);

    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));

    gtk_widget_show_all(window);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_main();

    return 0;
}

