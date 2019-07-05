/* Any copyright is dedicated to the Public Domain.
 * https://creativecommons.org/publicdomain/zero/1.0/ */

#include "demo-simple.h"

#include <gpropz.h>

enum {
  PROP_NAME = 1,
  PROP_ID = 2,
  N_PROPERTIES
};

static GParamSpec *properties[N_PROPERTIES];

struct _DemoSimple {
  GObject parent;

  char *name;
  int   id;
};

G_DEFINE_TYPE (DemoSimple, demo_simple, G_TYPE_OBJECT)

static void
demo_simple_id_get_filter (GObject      *object,
                           gconstpointer prop,
                           guint         prop_id,
                           gpointer      target,
                           GParamSpec   *pspec)
{
  DemoSimple *self = DEMO_SIMPLE (object);

  g_return_if_fail (prop == &self->id);
  g_return_if_fail (prop_id == PROP_ID);
  g_return_if_fail (pspec == properties[PROP_ID]);

  int *int_target = target;
  *int_target = self->id * 10;
}

static void
demo_simple_id_set_filter (GObject      *object,
                           gpointer      prop,
                           guint         prop_id,
                           gconstpointer source,
                           GParamSpec   *pspec)
{
  DemoSimple *self = DEMO_SIMPLE (object);

  g_return_if_fail (prop == &self->id);
  g_return_if_fail (prop_id == PROP_ID);
  g_return_if_fail (pspec == properties[PROP_ID]);

  const int *int_source = source;
  self->id = *int_source / 10;
}

static void
demo_simple_finalize (GObject *object)
{
  DemoSimple *self = DEMO_SIMPLE (object);

  g_clear_pointer (&self->name, g_free);
}

GpropzValueFilter demo_simple_id_filter = {.get_filter = demo_simple_id_get_filter,
                                           .set_filter = demo_simple_id_set_filter};

static void
demo_simple_class_init (DemoSimpleClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  object_class->finalize = demo_simple_finalize;

  gpropz_class_init_property_functions (object_class);

  properties[PROP_NAME] =
    g_param_spec_string ("name",
                         "A name",
                         "This is some random name you probably don't care about",
                         NULL, G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);
  gpropz_install_property (object_class, DemoSimple, name, PROP_NAME, properties[PROP_NAME],
                           NULL);

  properties[PROP_ID] =
    g_param_spec_int ("id",
                      "Some ID",
                      "Used to track your habits across the fourth dimension (and for filters)",
                      0, 100, 0, G_PARAM_READWRITE);
  gpropz_install_property (object_class, DemoSimple, id, PROP_ID, properties[PROP_ID],
                           &demo_simple_id_filter);
}

static void
demo_simple_init (DemoSimple *self)
{
  self->name = NULL;
  self->id = -1;
}

DemoSimple *
demo_simple_new (const char *name,
                 int         id)
{
  return g_object_new (DEMO_TYPE_SIMPLE,
                       "name", name,
                       "id", id,
                       NULL);
}

GPROPZ_DEFINE_RO (const char *, DemoSimple, demo_simple, name, properties[PROP_NAME])
GPROPZ_DEFINE_RW (int, DemoSimple, demo_simple, id, properties[PROP_ID])
