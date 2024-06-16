#include <gtk/gtk.h>
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <gio/gio.h> // Include gio for g_rename

// Function prototypes
void create_file(GtkWidget *widget, gpointer data);
void copy_file(GtkWidget *widget, gpointer data);
void move_file(GtkWidget *widget, gpointer data);
void delete_file(GtkWidget *widget, gpointer data);
void get_file_info(GtkWidget *widget, gpointer data);
void rename_file(GtkWidget *widget, gpointer data);
void print_file(GtkWidget *widget, gpointer data);

// Global variables
GtkWidget *file_list;
GtkListStore *store;
GtkWidget *menubar;

int main(int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *vbox;
    GtkWidget *file_menu;
    GtkWidget *file_menu_item;
    GtkWidget *create_menu_item;
    GtkWidget *copy_menu_item;
    GtkWidget *move_menu_item;
    GtkWidget *delete_menu_item;
    GtkWidget *info_menu_item;
    GtkWidget *print_menu_item;
    GtkWidget *rename_menu_item; // Added
    GtkWidget *scrolled_window;
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "File Manager");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);

    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(window), vbox);
    
    menubar = gtk_menu_bar_new();
    gtk_box_pack_start(GTK_BOX(vbox), menubar, FALSE, FALSE, 0);

    file_menu_item = gtk_menu_item_new_with_label("File");
    gtk_menu_shell_append(GTK_MENU_SHELL(menubar), file_menu_item);

    file_menu = gtk_menu_new();
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(file_menu_item), file_menu);

    create_menu_item = gtk_menu_item_new_with_label("Create");
    gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), create_menu_item);
    g_signal_connect(create_menu_item, "activate", G_CALLBACK(create_file), NULL);

    copy_menu_item = gtk_menu_item_new_with_label("Copy");
    gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), copy_menu_item);
    g_signal_connect(copy_menu_item, "activate", G_CALLBACK(copy_file), NULL);

    move_menu_item = gtk_menu_item_new_with_label("Move");
    gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), move_menu_item);
    g_signal_connect(move_menu_item, "activate", G_CALLBACK(move_file), NULL);

    delete_menu_item = gtk_menu_item_new_with_label("Delete");
    gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), delete_menu_item);
    g_signal_connect(delete_menu_item, "activate", G_CALLBACK(delete_file), NULL);

    info_menu_item = gtk_menu_item_new_with_label("Info");
    gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), info_menu_item);
    g_signal_connect(info_menu_item, "activate", G_CALLBACK(get_file_info), NULL);

    print_menu_item = gtk_menu_item_new_with_label("Print");
    gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), print_menu_item);
    g_signal_connect(print_menu_item, "activate", G_CALLBACK(print_file), NULL);

    rename_menu_item = gtk_menu_item_new_with_label("Rename"); // Added
    gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), rename_menu_item); // Added
    g_signal_connect(rename_menu_item, "activate", G_CALLBACK(rename_file), NULL); // Added

    scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_box_pack_start(GTK_BOX(vbox), scrolled_window, TRUE, TRUE, 0);

    store = gtk_list_store_new(1, G_TYPE_STRING);
    file_list = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));
    gtk_container_add(GTK_CONTAINER(scrolled_window), file_list);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("File", renderer, "text", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(file_list), column);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}


// Function to create a new file
void create_file(GtkWidget *widget, gpointer data) {
    // Open a dialog to get the name of the new file
    GtkWidget *dialog = gtk_file_chooser_dialog_new("Create File", GTK_WINDOW(gtk_widget_get_toplevel(widget)), GTK_FILE_CHOOSER_ACTION_SAVE, "_Create", GTK_RESPONSE_ACCEPT, "_Cancel", GTK_RESPONSE_CANCEL, NULL);

    // Display the dialog and get the response
    gint res = gtk_dialog_run(GTK_DIALOG(dialog));
    if (res == GTK_RESPONSE_ACCEPT) {
        // Get the filename entered by the user
        gchar *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
        // Create the file
        FILE *new_file = fopen(filename, "w");
        if (new_file != NULL) {
            fclose(new_file);
            // Add the filename to the list store
            GtkTreeIter iter;
            gtk_list_store_append(store, &iter);
            gtk_list_store_set(store, &iter, 0, filename, -1);
        } else {
            g_print("Failed to create file\n");
        }
        // Free the memory allocated for filename
        g_free(filename);
    }
    // Destroy the dialog
    gtk_widget_destroy(dialog);
}

// Function to copy a file
void copy_file(GtkWidget *widget, gpointer data) {
    // Open a dialog to select the file to copy
    GtkWidget *dialog = gtk_file_chooser_dialog_new("Copy File", GTK_WINDOW(gtk_widget_get_toplevel(widget)), GTK_FILE_CHOOSER_ACTION_OPEN, "_Copy", GTK_RESPONSE_ACCEPT, "_Cancel", GTK_RESPONSE_CANCEL, NULL);
    gtk_file_chooser_set_select_multiple(GTK_FILE_CHOOSER(dialog), FALSE);

    // Display the dialog and get the response
    gint res = gtk_dialog_run(GTK_DIALOG(dialog));
    if (res == GTK_RESPONSE_ACCEPT) {
        // Get the source filename
        gchar *src_filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
        // Close the dialog
        gtk_widget_destroy(dialog);

        // Open a dialog to select the destination directory
        dialog = gtk_file_chooser_dialog_new("Select Destination Directory", GTK_WINDOW(gtk_widget_get_toplevel(widget)), GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER, "_Copy", GTK_RESPONSE_ACCEPT, "_Cancel", GTK_RESPONSE_CANCEL, NULL);
        // Display the dialog and get the response
        res = gtk_dialog_run(GTK_DIALOG(dialog));
        if (res == GTK_RESPONSE_ACCEPT) {
            // Get the destination directory
            gchar *dest_dir = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
            // Construct the destination filename
            gchar *dest_filename = g_build_filename(dest_dir, g_path_get_basename(src_filename), NULL);
            // Copy the file
            if (g_file_copy(g_file_new_for_path(src_filename), g_file_new_for_path(dest_filename), G_FILE_COPY_OVERWRITE, NULL, NULL, NULL, NULL)) {
                // Add the copied file to the list store
                GtkTreeIter iter;
                gtk_list_store_append(store, &iter);
                gtk_list_store_set(store, &iter, 0, dest_filename, -1);
            } else {
                g_print("Failed to copy file\n");
            }
            // Free the memory allocated for filenames and directory
            g_free(dest_filename);
            g_free(dest_dir);
        }
        // Destroy the dialog
        gtk_widget_destroy(dialog);
        // Free the memory allocated for src_filename
        g_free(src_filename);
    } else {
        // Close the dialog
        gtk_widget_destroy(dialog);
    }
}


// Function to move a file
void move_file(GtkWidget *widget, gpointer data) {
    // Open a dialog to select the file to move
    GtkWidget *dialog = gtk_file_chooser_dialog_new("Move File", GTK_WINDOW(gtk_widget_get_toplevel(widget)), GTK_FILE_CHOOSER_ACTION_OPEN, "_Move", GTK_RESPONSE_ACCEPT, "_Cancel", GTK_RESPONSE_CANCEL, NULL);
    gtk_file_chooser_set_select_multiple(GTK_FILE_CHOOSER(dialog), FALSE);

    // Display the dialog and get the response
    gint res = gtk_dialog_run(GTK_DIALOG(dialog));
    if (res == GTK_RESPONSE_ACCEPT) {
        // Get the source filename
        gchar *src_filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
        // Close the dialog
        gtk_widget_destroy(dialog);

        // Open a dialog to select the destination directory
        dialog = gtk_file_chooser_dialog_new("Select Destination Directory", GTK_WINDOW(gtk_widget_get_toplevel(widget)), GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER, "_Move", GTK_RESPONSE_ACCEPT, "_Cancel", GTK_RESPONSE_CANCEL, NULL);
        // Display the dialog and get the response
        res = gtk_dialog_run(GTK_DIALOG(dialog));
        if (res == GTK_RESPONSE_ACCEPT) {
            // Get the destination directory
            gchar *dest_dir = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
            // Construct the destination filename
            gchar *dest_filename = g_build_filename(dest_dir, g_path_get_basename(src_filename), NULL);
            // Move the file
            if (rename(src_filename, dest_filename) == 0) {
                // Add the moved file to the list store
                GtkTreeIter iter;
                gtk_list_store_append(store, &iter);
                gtk_list_store_set(store, &iter, 0, dest_filename, -1);
            } else {
                g_print("Failed to move file\n");
            }
            // Free the memory allocated for filenames and directory
            g_free(dest_filename);
            g_free(dest_dir);
        }
        // Destroy the dialog
        gtk_widget_destroy(dialog);
        // Free the memory allocated for src_filename
        g_free(src_filename);
    } else {
        // Close the dialog
        gtk_widget_destroy(dialog);
    }
}

// Function to delete a file
void delete_file(GtkWidget *widget, gpointer data) {
    // Get the selected file path
    GtkTreeSelection *selection;
    GtkTreeModel *model;
    GtkTreeIter iter;
    gchar *file_path;

    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(file_list));
    if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
        gtk_tree_model_get(model, &iter, 0, &file_path, -1);

        // Remove the file from the filesystem
        if (remove(file_path) == 0) {
            // Remove the file from the list store
            gtk_list_store_remove(store, &iter);
        } else {
            g_print("Failed to delete file\n");
        }

        // Free the memory allocated for file_path
        g_free(file_path);
    }
}

// Function to get information about a file
void get_file_info(GtkWidget *widget, gpointer data) {
    // Get the selected file path
    GtkTreeIter iter;
    gchar *filename;
    struct stat st;

    GtkTreeSelection *selection;
    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(file_list));
    if (gtk_tree_selection_get_selected(selection, NULL, &iter)) {
        gtk_tree_model_get(GTK_TREE_MODEL(store), &iter, 0, &filename, -1);
        stat(filename, &st);

        // Display file information
        GtkWidget *dialog;
        dialog = gtk_message_dialog_new(GTK_WINDOW(gtk_widget_get_toplevel(widget)), GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "File Info:\nSize: %ld bytes\nModified: %s", st.st_size, ctime(&st.st_mtime));
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);

        // Free the memory allocated for filename
        g_free(filename);
    }
}

// Function to rename a file
void rename_file(GtkWidget *widget, gpointer data) {
    // Get the selected file path
    GtkTreeSelection *selection;
    GtkTreeModel *model;
    GtkTreeIter iter;
    gchar *file_path;

    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(file_list));
    if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
        gtk_tree_model_get(model, &iter, 0, &file_path, -1);

        // Create a dialog for renaming the file
        GtkWidget *dialog;
        dialog = gtk_dialog_new_with_buttons("Rename File", GTK_WINDOW(gtk_widget_get_toplevel(widget)), GTK_DIALOG_MODAL, "_Rename", GTK_RESPONSE_ACCEPT, "_Cancel", GTK_RESPONSE_CANCEL, NULL);

        // Add an entry field to the dialog and set the current filename as the default text
        GtkWidget *entry;
        entry = gtk_entry_new();
        gtk_entry_set_text(GTK_ENTRY(entry), g_path_get_basename(file_path));
        gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(dialog))), entry);

        // Display the dialog
        gint res = gtk_dialog_run(GTK_DIALOG(dialog));
        if (res == GTK_RESPONSE_ACCEPT) {
            // Get the new filename entered by the user
            const gchar *new_name = gtk_entry_get_text(GTK_ENTRY(entry));
            // Construct the new file path
            gchar *new_path = g_build_filename(g_path_get_dirname(file_path), new_name, NULL);

            // Rename the file
            if (rename(file_path, new_path) == 0) {
                // Update the filename in the list store
                gtk_list_store_set(store, &iter, 0, new_path, -1);
            } else {
                g_print("Failed to rename file\n");
            }

            // Free the memory allocated for filenames
            g_free(new_path);
        }

        // Destroy the dialog
        gtk_widget_destroy(dialog);
        // Free the memory allocated for file_path
        g_free(file_path);
    }
}


// Function to print a file
void print_file(GtkWidget *widget, gpointer data) {
    // Get the selected file path
    GtkTreeSelection *selection;
    GtkTreeModel *model;
    GtkTreeIter iter;
    gchar *file_path;

    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(file_list));
    if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
        gtk_tree_model_get(model, &iter, 0, &file_path, -1);

        // Construct the print command
        gchar *command = g_strdup_printf("lp \"%s\"", file_path);

        // Execute the print command
        int result = system(command);
        if (result != 0) {
            g_print("Failed to print file\n");
        }

        // Free the memory allocated for file_path and command
        g_free(file_path);
        g_free(command);
    }
}



