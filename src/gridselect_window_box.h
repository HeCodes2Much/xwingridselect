/* Copyright (c) 2020, TheCynicalTeam @ Github
 * gridselect_window_box.h - headers for window box widget
 */

#ifndef GRIDSELECT_WINDOW_BOX_H
#define GRIDSELECT_WINDOW_BOX_H

#include <stdlib.h>
#include <gtk/gtk.h>
#include <cairo.h>
#include <gdk/gdkkeysyms.h>
#include <string.h>
#include "gridselect_box.h"
#ifdef X11
#include "x_interaction.h"
#endif
#ifdef WIN32
#include "win32_interaction.h"
#endif

G_BEGIN_DECLS

#define GRIDSELECT_TYPE_WINDOW_BOX            (gridselect_window_box_get_type ())
#define GRIDSELECT_WINDOW_BOX(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GRIDSELECT_TYPE_WINDOW_BOX, GridWindowBox))
#define GRIDSELECT_WINDOW_BOX_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GRIDSELECT_TYPE_WINDOW_BOX, GridWindowBoxClass))
#define GRIDSELECT_IS_WINDOW_BOX(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GRIDSELECT_TYPE_WINDOW_BOX))
#define GRIDSELECT_IS_WINDOW_BOX_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GRIDSELECT_TYPE_WINDOW_BOX))
#define GRIDSELECT_WINDOW_BOX_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GRIDSELECT_TYPE_WINDOW_BOX, GridWindowBoxClass))

typedef struct _GridWindowBox      GridWindowBox;
typedef struct _GridWindowBoxClass GridWindowBoxClass;

struct _GridWindowBox
{
  GridBox parent;

  /*< private >*/
  gboolean is_window;
  Window xwindow;

  gchar *opt_name;
  gboolean show_desktop;
  gboolean show_titles;
  gint desktop;

  gboolean colorize;
  gdouble r, g, b;
  guchar color_offset;

  gboolean has_icon;
  GdkPixbuf *icon_pixbuf;
  cairo_t *icon_context;
  cairo_surface_t *icon_surface;
};

struct _GridWindowBoxClass
{
  GridBoxClass parent_class;

  void (* pressed)  (GridWindowBox *box);
  void (* released) (GridWindowBox *box);
  void (* clicked)  (GridWindowBox *box);
  void (* enter)    (GridWindowBox *box);
  void (* leave)    (GridWindowBox *box);
  void (* activate) (GridWindowBox *box);
};

GType gridselect_window_box_get_type (void);
GtkWidget* gridselect_window_box_new (void);
GtkWidget* gridselect_window_box_new_with_xwindow (Window win);
GtkWidget* gridselect_window_box_new_with_name (gchar *name);
void gridselect_window_box_set_is_window (GridWindowBox *box, gboolean is_window);
gboolean gridselect_window_box_get_is_window (GridWindowBox *box);
void gridselect_window_box_set_xwindow (GridWindowBox *box, Window window);
Window gridselect_window_box_get_xwindow (GridWindowBox *box);
void gridselect_window_box_set_name (GridWindowBox *box, const gchar *name);
const gchar *gridselect_window_box_get_name (GridWindowBox *box);
void gridselect_window_box_set_opt_name (GridWindowBox *box, const gchar *opt_name);
const gchar *gridselect_window_box_get_opt_name (GridWindowBox *box);
void gridselect_window_box_update_xwindow_name (GridWindowBox *box);
void gridselect_window_box_update_opt_name (GridWindowBox *box);
void gridselect_window_box_setup_icon_from_wm (GridWindowBox *box, guint req_width, guint req_height);
void gridselect_window_box_setup_icon_from_theme (GridWindowBox *box, const gchar *name, guint req_width, guint req_height);
void gridselect_window_box_setup_icon_from_file (GridWindowBox *box, const gchar *file, guint req_width, guint req_height);
void gridselect_window_box_set_colorize (GridWindowBox *box, gboolean colorize);
void gridselect_window_box_set_show_desktop (GridWindowBox *box, gboolean show_desktop);
void gridselect_window_box_set_show_titles (GridWindowBox *box, gboolean show_titles);
void gridselect_window_box_set_color_offset (GridWindowBox *box, guchar color_offset);
void gridselect_window_box_set_color_from_string (GridWindowBox *box, const gchar *color);
gint gridselect_window_box_get_desktop (GridWindowBox *box);
void gridselect_window_box_set_desktop (GridWindowBox *box, gint desktop);

G_END_DECLS

#endif /* GRIDSELECT_WINDOW_BOX_H */
