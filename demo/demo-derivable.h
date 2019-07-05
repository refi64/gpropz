/* Any copyright is dedicated to the Public Domain.
 * https://creativecommons.org/publicdomain/zero/1.0/ */

#ifndef DEMO_OBJECT_H
#define DEMO_OBJECT_H

#include <glib-object.h>

#include "demo-simple.h"

G_BEGIN_DECLS

#define DEMO_TYPE_DERIVABLE demo_derivable_get_type ()
G_DECLARE_DERIVABLE_TYPE (DemoDerivable, demo_derivable, DEMO, DERIVABLE, GObject)

struct _DemoDerivableClass {
  GObjectClass parent_class;

  // virtual functions here...
};

DemoDerivable *demo_derivable_new (DemoSimple *simple);

DemoSimple *demo_derivable_get_simple (DemoDerivable *self);
void        demo_derivable_set_simple (DemoDerivable *self,
                                       DemoSimple    *simple);

G_END_DECLS

#endif
