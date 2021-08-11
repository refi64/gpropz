/* Any copyright is dedicated to the Public Domain.
 * https://creativecommons.org/publicdomain/zero/1.0/ */

#include "demo-derivable.h"

#include <gpropz.h>

enum {
  PROP_SIMPLE = 1,
  N_PROPERTIES
};

static GParamSpec *properties[N_PROPERTIES];

typedef struct {
  DemoSimple *simple;
} DemoDerivablePrivate;

G_DEFINE_TYPE_WITH_PRIVATE (DemoDerivable, demo_derivable, G_TYPE_OBJECT)

static void
demo_derivable_dispose (GObject *object)
{
  DemoDerivable *derivable = DEMO_DERIVABLE (object);
  DemoDerivablePrivate *priv = demo_derivable_get_instance_private (derivable);
  g_clear_object (&priv->simple);
}

static void
demo_derivable_class_init (DemoDerivableClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  object_class->dispose = demo_derivable_dispose;

  gpropz_class_init_property_functions (object_class);

  properties[PROP_SIMPLE] =
    g_param_spec_object ("simple",
                         "A simple object",
                         "Showcase for gpropz with object properties",
                         DEMO_TYPE_SIMPLE, G_PARAM_READWRITE);
  gpropz_install_property_private (object_class, DemoDerivable, simple, PROP_SIMPLE,
                                   properties[PROP_SIMPLE], NULL);
}

static void
demo_derivable_init (DemoDerivable *self)
{
  DemoDerivablePrivate *priv = demo_derivable_get_instance_private (self);

  priv->simple = NULL;
}

DemoDerivable *
demo_derivable_new (DemoSimple *simple)
{
  return g_object_new (DEMO_TYPE_DERIVABLE,
                       "simple", simple,
                       NULL);
}

GPROPZ_DEFINE_RW (DemoSimple *, DemoDerivable, demo_derivable, simple, properties[PROP_SIMPLE])
