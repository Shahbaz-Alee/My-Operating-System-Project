#include <gtk/gtk.h>
#include <time.h>
#include <string.h>

GtkWidget *label;
guint timeout_id;

gboolean update_time(gpointer data) {
    time_t rawtime;
    struct tm *info;
    char buffer[80];

    time(&rawtime);
    info = localtime(&rawtime);

    strftime(buffer, 80, "%H:%M:%S", info);

    gtk_label_set_text(GTK_LABEL(data), buffer);

    return TRUE; // Return TRUE to reschedule the timeout function
}

void on_window_destroy(GtkWidget *widget, gpointer data) {
  g_source_remove(timeout_id);
  gtk_main_quit();
}

int main(int argc, char *argv[]) {
  GtkWidget *window;
  GtkWidget *box;
  GtkWidget *button;

  gtk_init(&argc, &argv);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "Digital Clock");
  gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);

  g_signal_connect(window, "destroy", G_CALLBACK(on_window_destroy), NULL);

  box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  gtk_container_add(GTK_CONTAINER(window), box);

  label = gtk_label_new("00:00:00");
  gtk_box_pack_start(GTK_BOX(box), label, TRUE, TRUE, 0);

    // Use CSS for setting font
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider,
        "label {"
        "  font-family: Arial;"
        "  font-size: 40px;"
        "}",
        -1,
        NULL);
    GtkStyleContext *context = gtk_widget_get_style_context(label);
    gtk_style_context_add_provider(context,
        GTK_STYLE_PROVIDER(provider),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    g_object_unref(provider);

    button = gtk_button_new_with_label("Exit");
    gtk_box_pack_start(GTK_BOX(box), button, TRUE, TRUE, 0);

    g_signal_connect(button, "clicked", G_CALLBACK(gtk_widget_destroy), window); // Close window when button is clicked

  timeout_id = g_timeout_add(500, update_time, label);

  gtk_widget_show_all(window);

  gtk_main();

  return 0;
}

