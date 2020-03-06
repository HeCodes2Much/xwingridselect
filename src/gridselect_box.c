/* Copyright (c) 2020, TheCynicalTeam @ Github
 * gridselect_box.c - common stuff for window_box and search_box.
 */

#include "gridselect_box.h"

#define BOX_DEFAULT_WIDTH 200
#define BOX_DEFAULT_HEIGHT 40

enum {
  CLICKED,
  LAST_SIGNAL
};

static GObject *gridselect_box_constructor (GType gtype,
					guint n_properties,
					GObjectConstructParam *properties);
static void gridselect_box_dispose (GObject *gobject);
static void gridselect_box_realize (GtkWidget *widget);
static void gridselect_box_size_request (GtkWidget *widget, GtkRequisition *requisition);
static void gridselect_box_size_allocate (GtkWidget *widget,GtkAllocation *allocation);

static gboolean gridselect_box_button_press (GtkWidget *widget, GdkEventButton *event);
static gboolean gridselect_box_button_release (GtkWidget *widget, GdkEventButton *event);
static gboolean gridselect_box_key_press (GtkWidget *widget, GdkEventKey *event);
static gboolean gridselect_box_key_release (GtkWidget *widget, GdkEventKey *event);
static gboolean gridselect_box_enter_notify (GtkWidget *widget, GdkEventCrossing *event);
static gboolean gridselect_box_leave_notify (GtkWidget *widget, GdkEventCrossing *event);

static void gridselect_box_clicked (GridBox *box);

static guint box_signals[LAST_SIGNAL] = { 0 };

G_DEFINE_TYPE (GridBox, gridselect_box, GTK_TYPE_DRAWING_AREA);

static void
gridselect_box_class_init (GridBoxClass *klass)
{
  GObjectClass *gobject_class;
  GtkWidgetClass *widget_class;

  gobject_class = G_OBJECT_CLASS (klass);
  widget_class = GTK_WIDGET_CLASS (klass);

  gobject_class->constructor = gridselect_box_constructor;
  gobject_class->dispose = gridselect_box_dispose;

  widget_class->realize = gridselect_box_realize;
  widget_class->size_request = gridselect_box_size_request;
  widget_class->size_allocate = gridselect_box_size_allocate;
  widget_class->button_press_event = gridselect_box_button_press;
  widget_class->button_release_event = gridselect_box_button_release;
  widget_class->key_press_event = gridselect_box_key_press;
  widget_class->key_release_event = gridselect_box_key_release;
  widget_class->enter_notify_event = gridselect_box_enter_notify;
  widget_class->leave_notify_event = gridselect_box_leave_notify;

  klass->clicked = NULL;

  box_signals [CLICKED] =
    g_signal_new ("clicked",
		  G_TYPE_FROM_CLASS (gobject_class),
		  G_SIGNAL_RUN_FIRST | G_SIGNAL_ACTION,
		  G_STRUCT_OFFSET (GridBoxClass, clicked),
		  NULL, NULL,
		  g_cclosure_marshal_VOID__VOID,
		  G_TYPE_NONE, 0);
}

static void
gridselect_box_init (GridBox *box)
{
  gtk_widget_set_can_focus (GTK_WIDGET (box), TRUE);
  gtk_widget_set_receives_default (GTK_WIDGET (box), TRUE);

  box->name = NULL;
}

static GObject*	gridselect_box_constructor (GType gtype,
					guint n_properties,
					GObjectConstructParam *properties)
{
  GObject *obj;
  GridBox *box;

  obj = G_OBJECT_CLASS (gridselect_box_parent_class)->constructor (gtype, n_properties, properties);

  box = GRIDSELECT_BOX (obj);

  box->font = g_strdup ("Sans 10");
  box->on_box = FALSE;
  box->box_down = FALSE;

  return obj;
}

GtkWidget* gridselect_box_new (void)
{
  return g_object_new (GRIDSELECT_TYPE_BOX, NULL);
}

GtkWidget* gridselect_box_new_with_name (gchar *name)
{
  return g_object_new (GRIDSELECT_TYPE_BOX, "name", name, NULL);
}

static void
gridselect_box_dispose (GObject *gobject)
{
  GridBox *box = GRIDSELECT_BOX (gobject);
  if (box->name)
    g_free (box->name);
  box->name = NULL;

  if (box->font)
    g_free (box->font);
  box->font = NULL;

  G_OBJECT_CLASS (gridselect_box_parent_class)->dispose (gobject);
}

static void gridselect_box_realize (GtkWidget *widget)
{
  GdkWindowAttr attributes;
  gint attributes_mask;

  g_return_if_fail (GRIDSELECT_IS_BOX (widget));

  gtk_widget_set_realized (widget, TRUE);

  attributes.window_type = GDK_WINDOW_CHILD;
  attributes.x = widget->allocation.x;
  attributes.y = widget->allocation.y;
  attributes.width = widget->allocation.width;
  attributes.height = widget->allocation.height;
  attributes.wclass = GDK_INPUT_OUTPUT;
  attributes.event_mask = gtk_widget_get_events (widget);
  attributes.event_mask |= (GDK_EXPOSURE_MASK |
			    GDK_BUTTON_PRESS_MASK |
			    GDK_BUTTON_RELEASE_MASK |
			    GDK_ENTER_NOTIFY_MASK |
			    GDK_LEAVE_NOTIFY_MASK);

  attributes.visual = gtk_widget_get_visual (widget);
  attributes.colormap = gtk_widget_get_colormap (widget);

  attributes_mask = GDK_WA_X | GDK_WA_Y | GDK_WA_VISUAL | GDK_WA_COLORMAP;

  widget->window = gdk_window_new (gtk_widget_get_parent_window (widget),
				   &attributes, attributes_mask);

  gdk_window_set_user_data (widget->window, widget);

  widget->style = gtk_style_attach (widget->style, widget->window);
}

static void gridselect_box_size_request (GtkWidget *widget, GtkRequisition *requisition)
{
  requisition->width = BOX_DEFAULT_WIDTH;
  requisition->height = BOX_DEFAULT_HEIGHT;
}

static void gridselect_box_size_allocate (GtkWidget *widget, GtkAllocation *allocation)
{
  g_return_if_fail (GRIDSELECT_IS_BOX (widget));
  g_return_if_fail (allocation != NULL);

  GTK_WIDGET_CLASS (gridselect_box_parent_class)->size_allocate (widget, allocation);

  widget->allocation = *allocation;
  if (gtk_widget_get_realized (widget)) {
    gdk_window_move_resize (widget->window,
			    allocation->x, allocation->y,
			    allocation->width, allocation->height);
  }
}

static gboolean gridselect_box_button_press (GtkWidget *widget, GdkEventButton *event)
{
  GridBox *box;

  if (event->type == GDK_BUTTON_PRESS && gtk_widget_get_can_focus (widget))
  {
    box = GRIDSELECT_BOX (widget);
    if (event->button == 1) {
      box->box_down = TRUE;
      gtk_widget_grab_focus (widget);
    }
  }

  return TRUE;
}

static gboolean gridselect_box_button_release (GtkWidget *widget, GdkEventButton *event)
{
  GridBox *box;

  if (event->type == GDK_BUTTON_RELEASE && gtk_widget_get_can_focus (widget))
  {
    box = GRIDSELECT_BOX (widget);
    if (event->button == 1 && box->on_box && box->box_down)
      gridselect_box_clicked (GRIDSELECT_BOX (widget));
    box->box_down = FALSE;
  }

  return TRUE;
}
static gboolean gridselect_box_key_press (GtkWidget *widget, GdkEventKey *event)
{
  GridBox *box = GRIDSELECT_BOX (widget);
  if ((event->keyval == GDK_Return ||
      event->keyval == GDK_KP_Enter ||
      event->keyval == GDK_ISO_Enter) &&
      gtk_widget_get_can_focus (widget))
  {
    box->box_down = TRUE;
  }
  return FALSE;
}

static gboolean gridselect_box_key_release (GtkWidget *widget, GdkEventKey *event)
{
  GridBox *box = GRIDSELECT_BOX (widget);
  if ((event->keyval == GDK_Return ||
       event->keyval == GDK_KP_Enter ||
       event->keyval == GDK_ISO_Enter) &&
      box->box_down &&
      gtk_widget_get_can_focus (widget))
  {
    gridselect_box_clicked (box);
    box->box_down = FALSE;
  }
  return FALSE;
}

static gboolean gridselect_box_enter_notify (GtkWidget * widget, GdkEventCrossing * event)
{
  GridBox *box;
  GtkWidget *event_widget;

  box = GRIDSELECT_BOX (widget);
  event_widget = gtk_get_event_widget ((GdkEvent *) event);

  if ((event_widget == widget) && (event->detail != GDK_NOTIFY_INFERIOR)) {
    box->on_box = TRUE;
    gtk_widget_queue_draw (widget);
  }

  return FALSE;
}

static gboolean gridselect_box_leave_notify (GtkWidget * widget, GdkEventCrossing * event)
{
  GridBox *box;
  GtkWidget *event_widget;

  box = GRIDSELECT_BOX (widget);
  event_widget = gtk_get_event_widget ((GdkEvent *) event);

  if ((event_widget == widget) && (event->detail != GDK_NOTIFY_INFERIOR)) {
    box->on_box = FALSE;
    gtk_widget_queue_draw (widget);
  }

  return FALSE;
}

void gridselect_box_clicked (GridBox *box)
{
  g_signal_emit (box, box_signals [CLICKED], 0);
}

void
gridselect_box_set_name (GridBox *box, const gchar *name)
{
  gchar *new_name;
  g_return_if_fail (GRIDSELECT_IS_BOX (box));

  new_name = g_strdup (name);
  g_free (box->name);
  box->name = new_name;

  gtk_widget_queue_draw (GTK_WIDGET (box));
}

const gchar *
gridselect_box_get_name (GridBox *box)
{
  g_return_val_if_fail (GRIDSELECT_IS_BOX (box), NULL);

  return box->name;
}

void gridselect_box_paint (GridBox *box, cairo_t *cr, gint width, gint height)
{
  gboolean has_focus = gtk_widget_has_focus (GTK_WIDGET (box));

  // Draw border
  cairo_rectangle (cr, 0, 0, width, height);
  if (has_focus) {
    cairo_set_source_rgb (cr, 1.0, 1.0, 1.0);
    cairo_set_line_width (cr, 4);
    cairo_stroke_preserve (cr);
  }
  cairo_set_source_rgb (cr, 0.0, 0.0, 0.0);
  cairo_set_line_width (cr, 1);
  cairo_stroke (cr);
}

void gridselect_box_set_font (GridBox *box, const gchar *font)
{
  g_return_if_fail (GRIDSELECT_IS_BOX (box));

  if (box->font)
    g_free (box->font);

  box->font = g_strdup (font);
}
