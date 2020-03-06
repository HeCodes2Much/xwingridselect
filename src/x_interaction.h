/* Copyright (c) 2020, TheCynicalTeam @ Github
 * x_interaction.h - layer to talk with X11.
 */

#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/extensions/Xcomposite.h>
#include <X11/extensions/Xdamage.h>

#ifndef X_INTERACTION_H
#define X_INTERACTION_H

Atom a_UTF8_STRING;

Atom a_WM_CLASS;
Atom a_WM_NAME;
Atom a_WM_WINDOW_ROLE;

Atom a_NET_SUPPORTING_WM_CHECK;
Atom a_NET_CLIENT_LIST;
Atom a_NET_DESKTOP_VIEWPORT;
Atom a_NET_CURRENT_DESKTOP;
Atom a_NET_ACTIVE_WINDOW;
Atom a_NET_WORKAREA;

Atom a_NET_WM_DESKTOP;
Atom a_NET_WM_NAME;
Atom a_NET_WM_VISIBLE_NAME;
Atom a_NET_WM_ICON;
Atom a_NET_WM_USER_TIME;

Atom a_NET_WM_WINDOW_TYPE;

Atom a_NET_WM_WINDOW_TYPE_DESKTOP;
Atom a_NET_WM_WINDOW_TYPE_DOCK;
Atom a_NET_WM_WINDOW_TYPE_TOOLBAR;
Atom a_NET_WM_WINDOW_TYPE_MENU;
Atom a_NET_WM_WINDOW_TYPE_UTILITY;
Atom a_NET_WM_WINDOW_TYPE_SPLASH;
Atom a_NET_WM_WINDOW_TYPE_DIALOG;
Atom a_NET_WM_WINDOW_TYPE_DROPDOWN_MENU;
Atom a_NET_WM_WINDOW_TYPE_POPUP_MENU;
Atom a_NET_WM_WINDOW_TYPE_TOOLTIP;
Atom a_NET_WM_WINDOW_TYPE_NOTIFICATION;
Atom a_NET_WM_WINDOW_TYPE_COMBO;
Atom a_NET_WM_WINDOW_TYPE_DND;
Atom a_NET_WM_WINDOW_TYPE_NORMAL;

Atom a_NET_WM_STATE;
Atom a_NET_WM_STATE_SKIP_TASKBAR;


void atoms_init ();
void* property (Window win, Atom prop, Atom type, int *nitems);
void climsg(Window win, long type, long l0, long l1, long l2, long l3, long l4);
int wm_supports_ewmh ();
char* get_window_name (Window win);
char* get_window_class (Window win);
int get_window_desktop (Window win);
Window* sorted_windows_list (Window *myown, Window *active_win, int *nitems, gboolean only_current);
void switch_to_window (Window win);
GdkPixbuf *get_window_icon (Window win, guint req_width, guint req_height);
gboolean already_opened ();

#endif /* X_INTERACTION_H */
