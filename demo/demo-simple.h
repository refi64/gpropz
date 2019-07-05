/* Any copyright is dedicated to the Public Domain.
 * https://creativecommons.org/publicdomain/zero/1.0/ */

#ifndef DEMO_SIMPLE_H
#define DEMO_SIMPLE_H

#include <glib-object.h>

G_BEGIN_DECLS

#define DEMO_TYPE_SIMPLE demo_simple_get_type ()
G_DECLARE_FINAL_TYPE (DemoSimple, demo_simple, DEMO, SIMPLE, GObject)

DemoSimple *demo_simple_new (const char *name,
                             int         id);

const char * demo_simple_get_name (DemoSimple *self);

int  demo_simple_get_id (DemoSimple *self);
void demo_simple_set_id (DemoSimple *self,
                         int         id);

G_END_DECLS

#endif
