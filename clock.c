#include <gtk/gtk.h>
#include <time.h>
#include <string.h>

// Global variable to store the label widget
GtkWidget *label;
// Global variable to store the timeout ID
guint timeout_id;

// Function to update the time on the label every 500 milliseconds
gboolean update_time(gpointer data) {
    time_t rawtime;
    struct tm *info;
    char buffer[80];

    // Get the current time
    time(&rawtime);
    // Convert the time to a structure that holds the local time
    info = localtime(&rawtime);

    // Format the time as HH:MM:SS
    strftime(buffer, 80, "%H:%M:%S", info);

    // Update the label's text with the formatted time
    gtk_label_set_text(GTK_LABEL(data), buffer);

    return TRUE; // Return TRUE to keep rescheduling the timeout function
}

// Callback function to handle window close event
void on_window_destroy(GtkWidget *widget, gpointer data) {
    // Remove the timeout event when the window is destroyed
    g_source_remove(timeout_id);
    gtk_main_quit(); // Quit the GTK main loop
}

int main(int argc, char *argv[]) {
    // Declare GTK widgets for the window, box, and button
    GtkWidget *window;
    GtkWidget *box;
    GtkWidget *button;

    // Initialize GTK
    gtk_init(&argc, &argv);

    // Create a new window
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    // Set window title
    gtk_window_set_title(GTK_WINDOW(window), "Digital Clock");
    // Set the default size of the window
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);

    // Connect the "destroy" signal of the window to close the application
    g_signal_connect(window, "destroy", G_CALLBACK(on_window_destroy), NULL);

    // Create a vertical box container
    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    // Add the box to the window
    gtk_container_add(GTK_CONTAINER(window), box);

    // Create a new label with the initial time "00:00:00"
    label = gtk_label_new("00:00:00");
    // Pack the label into the box with expanding and filling properties
    gtk_box_pack_start(GTK_BOX(box), label, TRUE, TRUE, 0);

    // Apply CSS to the label for font settings
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider,
        "label {"           // Target the label widget
        "  font-family: Arial;"  // Set font family to Arial
        "  font-size: 40px;"     // Set font size to 40px
        "}", -1, NULL);
    // Get the style context of the label and apply the CSS provider
    GtkStyleContext *context = gtk_widget_get_style_context(label);
    gtk_style_context_add_provider(context,
        GTK_STYLE_PROVIDER(provider),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    // Unreference the CSS provider after applying the style
    g_object_unref(provider);

    // Create an "Exit" button
    button = gtk_button_new_with_label("Exit");
    // Pack the button into the box with expanding and filling properties
    gtk_box_pack_start(GTK_BOX(box), button, TRUE, TRUE, 0);

    // Connect the "clicked" signal of the button to close the window
    g_signal_connect(button, "clicked", G_CALLBACK(gtk_widget_destroy), window);

    // Set a timeout to update the time on the label every 500 milliseconds
    timeout_id = g_timeout_add(500, update_time, label);

    // Show all widgets in the window
    gtk_widget_show_all(window);

    // Enter the GTK main loop
    gtk_main();

    return 0;
}
