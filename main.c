#include <gtk/gtk.h>

static void apply_css(void)
{
    GtkCssProvider *provider = gtk_css_provider_new();

    gtk_css_provider_load_from_string(
        provider,
        ".imgay {"
        "  background-color: #000;"
        "  color: #fff;"
        "}"
    );

    gtk_style_context_add_provider_for_display(
        gdk_display_get_default(),
        GTK_STYLE_PROVIDER(provider),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION
    );

    g_object_unref(provider);
}

static void activate(GtkApplication *app, gpointer user_data)
{
    GtkWidget *window;
    GtkWidget *box;
    GtkWidget *picture;
    GtkWidget *label;

    window = gtk_application_window_new(app);

    gtk_widget_add_css_class(window, "imgay");

    gtk_window_set_title(GTK_WINDOW(window), "Video Player");
    gtk_window_set_default_size(GTK_WINDOW(window), 960, 540);

    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
    gtk_widget_set_halign(box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(box, GTK_ALIGN_CENTER);

    picture = gtk_picture_new_for_filename("logo0.png");
    gtk_picture_set_can_shrink(GTK_PICTURE(picture), TRUE);
    gtk_widget_set_size_request(picture, 250, 250);

    gtk_widget_set_halign(picture, GTK_ALIGN_CENTER);

    label = gtk_label_new("You must specify a video file");
    gtk_widget_set_halign(label, GTK_ALIGN_CENTER);

    gtk_box_append(GTK_BOX(box), picture);
    gtk_box_append(GTK_BOX(box), label);

    gtk_window_set_child(GTK_WINDOW(window), box);
    gtk_window_present(GTK_WINDOW(window));
}

static void open_file(
    GtkApplication *app,
    GFile **files,
    gint n_files,
    const gchar *hint
)
{
    GtkWidget *window;
    GtkWidget *video;

    if (n_files < 1)
        return;

    window = gtk_application_window_new(app);

    gtk_widget_add_css_class(window, "player-window");

    gtk_window_set_title(GTK_WINDOW(window), "Video Player");
    gtk_window_set_default_size(GTK_WINDOW(window), 960, 540);

    video = gtk_video_new_for_file(files[0]);
    gtk_window_set_child(GTK_WINDOW(window), video);

    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv)
{
    GtkApplication *app;
    int status;

    app = gtk_application_new(
        "net.gaymarkiplier.player",
        G_APPLICATION_DEFAULT_FLAGS | G_APPLICATION_HANDLES_OPEN
    );

    g_signal_connect(app, "open", G_CALLBACK(open_file), NULL);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

    g_signal_connect(app, "startup", G_CALLBACK(apply_css), NULL);

    status = g_application_run(G_APPLICATION(app), argc, argv);

    g_object_unref(app);

    return status;
}