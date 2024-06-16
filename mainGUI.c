#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/wait.h>

#define BUTTON_WIDTH 150
#define BUTTON_HEIGHT 150

// Function to resize an image to fit within the specified width and height while maintaining the aspect ratio
GdkPixbuf *resize_image(const gchar *filename, int width, int height) {
    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file(filename, NULL);
    GdkPixbuf *scaled_pixbuf = gdk_pixbuf_scale_simple(pixbuf, width, height, GDK_INTERP_BILINEAR);
    g_object_unref(pixbuf);
    return scaled_pixbuf;
}

// Callback function for task buttons
void on_task_button_clicked(GtkButton *button, gpointer user_data) {
    const char *task_name = (const char *)user_data;
    printf("%s is running...\n", task_name);
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        execlp(task_name, task_name, NULL);
        perror("exec");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[]) {
    // Check if the correct number of arguments is provided
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <RAM> <HDD> <CORES>\n", argv[0]);
        return EXIT_FAILURE;
    }
    
    // Parse command-line arguments for RAM, HDD, and CPU cores
    int ram = atoi(argv[1]);
    int hdd_capacity = atoi(argv[2]);
    int cpu_cores = atoi(argv[3]);
    
    // Suppress unused variable warnings
    (void)ram;
    (void)hdd_capacity;
    (void)cpu_cores;

    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *button;
    
    // Initialize GTK
    gtk_init(&argc, &argv);

    // Create a new window
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "MyOS");
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Set the window size
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 200);

    // Create a grid layout
    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    // Create task buttons
    const char *task_names[] = {"./clock", "./background_music", "./calculator", "./notepad", "./file_manager", "./minigames", "./shutdown"};
    const char *icon_files[] = {"clock.png", "music.jpg", "calculator.jpg", "notepad.png", "file_manager.png", "game.png", "shutdown.png"};
    for (int i = 0; i < sizeof(task_names) / sizeof(task_names[0]); i++) {
        button = gtk_button_new();
        GdkPixbuf *pixbuf = resize_image(icon_files[i], BUTTON_WIDTH, BUTTON_HEIGHT);
        GtkWidget *image = gtk_image_new_from_pixbuf(pixbuf);
        gtk_button_set_image(GTK_BUTTON(button), image);
        gtk_grid_attach(GTK_GRID(grid), button, i % 3, i / 3, 1, 1);
        g_signal_connect(button, "clicked", G_CALLBACK(on_task_button_clicked), (gpointer)task_names[i]);
        g_object_unref(pixbuf);
    }

    // Set spacing between buttons
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);

    // Show all widgets
    gtk_widget_show_all(window);

    // Run the GTK main loop
    gtk_main();

    return 0;
}

