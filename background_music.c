#include <gtk/gtk.h>
#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>

// Function to play music
void play_music(GtkWidget *widget, gpointer data) {
    GtkWidget *folder_chooser = (GtkWidget *) data;
    const gchar *folder_path = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(folder_chooser));

    // Open the selected folder
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(folder_path)) != NULL) {
        // Iterate over all files in the folder
        while ((ent = readdir(dir)) != NULL) {
            // Check if the file has an extension of mp3 or wav
            if (strstr(ent->d_name, ".mp3") != NULL || strstr(ent->d_name, ".wav") != NULL) {
                // Create a command to play music using vlc
                gchar *command = g_strdup_printf("vlc \"%s/%s\" &", folder_path, ent->d_name);
                // Execute the command
                system(command);
                g_free(command);
            }
        }
        closedir(dir);
    } else {
        // Error handling if unable to open the folder
        perror("Unable to open directory");
    }
}

int main(int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *button;
    GtkWidget *folder_chooser;
    GtkWidget *box;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Simple Music Player");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 200);

    box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_container_add(GTK_CONTAINER(window), box);

    folder_chooser = gtk_file_chooser_button_new("Select Folder", GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER);
    gtk_box_pack_start(GTK_BOX(box), folder_chooser, TRUE, TRUE, 0);

    button = gtk_button_new_with_label("Play");
    gtk_box_pack_start(GTK_BOX(box), button, TRUE, TRUE, 0);

    g_signal_connect(button, "clicked", G_CALLBACK(play_music), folder_chooser);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}

