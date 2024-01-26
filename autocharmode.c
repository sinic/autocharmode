/* Copyright (C) 2021 Simon Nicolussi
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
 */
#include "gdk/gdkx.h"
#include "glib.h"
#include "glib/gprintf.h"
#include "gtk/gtk.h"
#include "gtk/gtkimmodule.h"

GType type = 0;

typedef struct {
  GtkIMContextSimple parent;
  Window window;
} AutoCharModeContext;

static void toggle(const gchar *action, GtkIMContext *context) {
  gchar *s = g_strdup_printf("emacsclient -eu (exwm-input-%s-keyboard#x%lx)",
                             action, ((AutoCharModeContext *)context)->window);
  g_spawn_command_line_sync(s, NULL, NULL, NULL, NULL);
  g_free(s);
}

static void focus_in(GtkIMContext *context) {
  toggle("release", context);
}

static void focus_out(GtkIMContext *context) {
  toggle("grab", context);
}

static void set_client_window(GtkIMContext *context, GdkWindow *window) {
  AutoCharModeContext *sub = (AutoCharModeContext *)context;

  if (window == NULL)
    sub->window = 0;
  else {
    GdkWindow *toplevel = gdk_window_get_effective_toplevel(window);
    sub->window = gdk_x11_window_get_xid(toplevel);
  }
}

static void init(gpointer g_class) {
  GTK_IM_CONTEXT_CLASS(g_class)->focus_in = focus_in;
  GTK_IM_CONTEXT_CLASS(g_class)->focus_out = focus_out;
  GTK_IM_CONTEXT_CLASS(g_class)->set_client_window = set_client_window;
}

G_MODULE_EXPORT
void im_module_init(GTypeModule *module) {
  static const GTypeInfo info = { .class_size = sizeof(GtkIMContextSimpleClass),
    .base_init = init, .instance_size = sizeof(AutoCharModeContext) };

  type = g_type_module_register_type(module, GTK_TYPE_IM_CONTEXT_SIMPLE,
                                     "AutoCharModeContext", &info, 0);
}

G_MODULE_EXPORT
void im_module_exit(void) {
}

G_MODULE_EXPORT
void im_module_list(const GtkIMContextInfo ***contexts, int *n_contexts) {
  static const GtkIMContextInfo info = {"autocharmode", "Toggle EXWM char-mode",
                                        "", "", ""},
    *info_list[] = { &info };

  *contexts = info_list;
  *n_contexts = G_N_ELEMENTS(info_list);
}

G_MODULE_EXPORT
GtkIMContext *im_module_create(const gchar *context_id) {
  if (g_strcmp0(context_id, "autocharmode") == 0)
    return g_object_new(type, NULL);
  return NULL;
}
