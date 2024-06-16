#include <gtk/gtk.h>

void on_activate(GtkApplication *application, gpointer user_data);
void on_button_clicked(GtkWidget *widget, gpointer data);

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkApplication *application;
    application = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(application, "activate", G_CALLBACK(on_activate), NULL);

    g_application_run(G_APPLICATION(application), argc, argv);
    g_object_unref(application);

    return 0;
}

void on_activate(GtkApplication *application, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *button;
    GtkWidget *entry;

    window = gtk_application_window_new(application);
    gtk_window_set_title(GTK_WINDOW(window), "Calculator");
    gtk_window_set_default_size(GTK_WINDOW(window), 200, 200);

    grid = gtk_grid_new();
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
    gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);
    gtk_container_add(GTK_CONTAINER(window), grid);

    entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), entry, 0, 0, 4, 1);

    gchar *button_texts[] = {"7", "8", "9", "/",
                             "4", "5", "6", "*",
                             "1", "2", "3", "-",
                             "0", ".", "=", "+"};

    gint row = 1;
    gint col = 0;
    for (gint i = 0; i < 16; i++) {
        button = gtk_button_new_with_label(button_texts[i]);
        gtk_grid_attach(GTK_GRID(grid), button, col, row, 1, 1);
        g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), entry);

        col++;
        if (col > 3) {
            col = 0;
            row++;
        }
    }

    button = gtk_button_new_with_label("C");
    gtk_grid_attach(GTK_GRID(grid), button, 0, 5, 4, 1);
    g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), entry);

    gtk_widget_show_all(window);
}

void on_button_clicked(GtkWidget *widget, gpointer data) {
    const gchar *button_text = gtk_button_get_label(GTK_BUTTON(widget));
    const gchar *entry_text = gtk_entry_get_text(GTK_ENTRY(data));

    if (strcmp(button_text, "C") == 0) {
        gtk_entry_set_text(GTK_ENTRY(data), "");
    } else if (strcmp(button_text, "=") == 0) {
        double num1, num2, result;
        char operator;
        sscanf(entry_text, "%lf %c %lf", &num1, &operator, &num2);

        switch (operator) {
            case '+':
                result = num1 + num2;
                break;
            case '-':
                result = num1 - num2;
                break;
            case '*':
                result = num1 * num2;
                break;
            case '/':
                if (num2 == 0) {
                    gtk_entry_set_text(GTK_ENTRY(data), "Error! Division by zero.");
                    return;
                }
                result = num1 / num2;
                break;
            default:
                gtk_entry_set_text(GTK_ENTRY(data), "Error! Invalid operator.");
                return;
        }

        gchar result_str[20];
        g_snprintf(result_str, 20, "%.2f", result);
        gtk_entry_set_text(GTK_ENTRY(data), result_str);
    } else {
        gchar new_text[20];
        g_snprintf(new_text, 20, "%s%s", entry_text, button_text);
        gtk_entry_set_text(GTK_ENTRY(data), new_text);
    }
}

