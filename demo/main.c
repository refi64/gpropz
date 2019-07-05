/* Any copyright is dedicated to the Public Domain.
 * https://creativecommons.org/publicdomain/zero/1.0/ */

#include "demo-simple.h"
#include "demo-derivable.h"

int main ()
{
  g_autoptr(DemoSimple) simple = demo_simple_new ("Name", 12);
  g_autoptr(DemoDerivable) derivable = demo_derivable_new (NULL);

  g_print ("simple.name == %s (Name)\n", demo_simple_get_name (simple));
  g_print ("simple.id == %d (10)\n", demo_simple_get_id (simple));

  g_warn_if_fail (demo_derivable_get_simple (derivable) == NULL);
  demo_derivable_set_simple (derivable, simple);
  g_warn_if_fail (demo_derivable_get_simple (derivable) == simple);

  demo_simple_set_id (simple, 39);
  g_print ("simple.id == %d (30)\n", demo_simple_get_id (simple));
}
