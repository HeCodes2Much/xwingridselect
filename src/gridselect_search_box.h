/* Copyright (c) 2020, TheCynicalTeam @ Github
 * gridselect_seach_box.h - headers for search box widget.
 */

#ifndef GRIDSELECT_SEARCH_BOX_H
#define GRIDSELECT_SEARCH_BOX_H

#include <glib.h>
#include <string.h>
#include "gridselect_box.h"

G_BEGIN_DECLS

#define GRIDSELECT_TYPE_SEARCH_BOX            (gridselect_search_box_get_type ())
#define GRIDSELECT_SEARCH_BOX(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GRIDSELECT_TYPE_SEARCH_BOX, GridSearchBox))
#define GRIDSELECT_SEARCH_BOX_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GRIDSELECT_TYPE_SEARCH_BOX, GridSearchBoxClass))
#define GRIDSELECT_IS_SEARCH_BOX(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GRIDSELECT_TYPE_SEARCH_BOX))
#define GRIDSELECT_IS_SEARCH_BOX_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GRIDSELECT_TYPE_SEARCH_BOX))
#define GRIDSELECT_SEARCH_BOX_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GRIDSELECT_TYPE_SEARCH_BOX, GridSearchBoxClass))

typedef struct _GridSearchBox      GridSearchBox;
typedef struct _GridSearchBoxClass GridSearchBoxClass;

struct _GridSearchBox
{
  GridBox parent;

  /*< private >*/
  gchar *cursor;
};

struct _GridSearchBoxClass
{
  GridBoxClass parent_class;

  void (* changed) (GridSearchBox *box);
};

GType gridselect_search_box_get_type (void);
GtkWidget *gridselect_search_box_new (void);
void gridselect_search_box_set_text (GridSearchBox *box, const gchar *text);
const gchar *gridselect_search_box_get_text (GridSearchBox *box);
void gridselect_search_box_append_text (GridSearchBox *box, const gchar *text);
void gridselect_search_box_remove_symbols (GridSearchBox *box, guint size);
void gridselect_search_box_kill_word (GridSearchBox *box);

G_END_DECLS

#endif /* GRIDSELECT_SEARCH_BOX_H */
