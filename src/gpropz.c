/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "gpropz.h"

G_DEFINE_QUARK ("gpropz-internal-data-quark", gpropz_internal_data)

#define GPROPZ_INTERNAL_DATA (gpropz_internal_data_quark ())

typedef struct _GpropzInternalData GpropzInternalData;

struct _GpropzInternalData {
  gsize              struct_offset;
  guint              prop_id;
  GpropzValueFilter *filter;
};

#define GPROPZ_BOOLEAN_VALUE boolean
#define GPROPZ_CHAR_VALUE schar
#define GPROPZ_UCHAR_VALUE uchar
#define GPROPZ_INT_VALUE int
#define GPROPZ_UINT_VALUE uint
#define GPROPZ_UNICHAR_VALUE GPROPZ_UINT_VALUE
#define GPROPZ_LONG_VALUE long
#define GPROPZ_ULONG_VALUE ulong
#define GPROPZ_INT64_VALUE int64
#define GPROPZ_UINT64_VALUE uint64
#define GPROPZ_FLOAT_VALUE float
#define GPROPZ_DOUBLE_VALUE double
#define GPROPZ_ENUM_VALUE enum
#define GPROPZ_FLAGS_VALUE flags
#define GPROPZ_STRING_VALUE string
#define GPROPZ_PARAM_VALUE param
#define GPROPZ_BOXED_VALUE boxed
#define GPROPZ_POINTER_VALUE pointer
#define GPROPZ_OBJECT_VALUE object
#define GPROPZ_GTYPE_VALUE gtype
#define GPROPZ_VARIANT_VALUE variant

#define GPROPZ_BASIC_TYPE(TYPE) \
  typedef G_PASTE (g, GPROPZ_##TYPE##_VALUE) GPROPZ_##TYPE##_TYPE;

GPROPZ_BASIC_TYPE(BOOLEAN)
typedef gint8 GPROPZ_CHAR_TYPE;
typedef guint8 GPROPZ_UCHAR_TYPE;
GPROPZ_BASIC_TYPE(INT)
GPROPZ_BASIC_TYPE(UINT)
typedef GPROPZ_UINT_TYPE GPROPZ_UNICHAR_TYPE;
GPROPZ_BASIC_TYPE(LONG)
GPROPZ_BASIC_TYPE(ULONG)
GPROPZ_BASIC_TYPE(INT64)
GPROPZ_BASIC_TYPE(UINT64)
GPROPZ_BASIC_TYPE(FLOAT)
GPROPZ_BASIC_TYPE(DOUBLE)
typedef gint GPROPZ_ENUM_TYPE;
typedef guint GPROPZ_FLAGS_TYPE;
typedef const gchar * GPROPZ_STRING_TYPE;
typedef GParamSpec * GPROPZ_PARAM_TYPE;
typedef gpointer GPROPZ_BOXED_TYPE;
GPROPZ_BASIC_TYPE(POINTER)
typedef gpointer GPROPZ_OBJECT_TYPE;
typedef GType GPROPZ_GTYPE_TYPE;
typedef GVariant * GPROPZ_VARIANT_TYPE;

void
_gpropz_internal_get_with_filter (GObject    *object,
                                  GParamSpec *pspec,
                                  gpointer    target,
                                  gsize       size)
{
  GpropzInternalData *data = g_param_spec_get_qdata (pspec, GPROPZ_INTERNAL_DATA);
  gconstpointer prop = ((gconstpointer) object) + data->struct_offset;

  if (data->filter && data->filter->get_filter)
    data->filter->get_filter (object, prop, data->prop_id, target, pspec);
  else
    memcpy (target, prop, size);
}

static gboolean
nested_ptr_not_null (gconstpointer source)
{ 
  return *(gconstpointer *)source != NULL;
}

void
_gpropz_internal_set_with_filter (GObject      *object,
                                  GParamSpec   *pspec,
                                  gconstpointer source,
                                  gsize         size)
{
  GpropzInternalData *data = g_param_spec_get_qdata (pspec, GPROPZ_INTERNAL_DATA);
  gpointer prop = ((gpointer) object) + data->struct_offset;

  gpointer copy = NULL;

  if (G_IS_PARAM_SPEC_STRING (pspec) && nested_ptr_not_null (source))
    {
      g_clear_pointer ((const gchar **)prop, g_free);

      copy = g_strdup (*(const gchar **)source);
      source = &copy;
    }
  else if (G_IS_PARAM_SPEC_PARAM (pspec) && nested_ptr_not_null (source))
    {
      g_clear_pointer ((GParamSpec **)prop, g_param_spec_unref);
      g_param_spec_ref_sink (*(GParamSpec **)source);
    }
  else if (G_IS_PARAM_SPEC_BOXED (pspec) && nested_ptr_not_null (source))
    {
      if (nested_ptr_not_null (prop))
        g_boxed_free (pspec->value_type, *(gpointer *)prop);

      copy = g_boxed_copy (pspec->value_type, *(gpointer *)source);
      source = &copy;
    }
  else if (G_IS_PARAM_SPEC_OBJECT (pspec) && nested_ptr_not_null (source))
    {
      g_clear_object ((GObject **)prop);
      g_object_ref_sink (*(GObject **)source);
    }
  else if (G_IS_PARAM_SPEC_VARIANT (pspec) && nested_ptr_not_null (source))
    {
      g_clear_pointer ((GVariant **)prop, g_variant_unref);
      g_variant_ref_sink (*(GVariant **)source);
    }

  if (data->filter && data->filter->set_filter)
    data->filter->set_filter (object, prop, data->prop_id, source, pspec);
  else
    memcpy (prop, source, size);

  g_object_notify_by_pspec (object, pspec);
}

/**
 * gpropz_auto_get_property:
 *
 * This function may be placed in #GObjectClass.get_property in order to use gpropz's automated
 * getter.
 */
void
gpropz_auto_get_property (GObject    *object,
                          guint       prop_id,
                          GValue     *value,
                          GParamSpec *pspec)
{
  #define GPROPZ_GET_PROPERTY_IF(TYPE) \
    if (G_IS_PARAM_SPEC_##TYPE (pspec)) \
      { \
        GPROPZ_##TYPE##_TYPE pval; \
        _gpropz_internal_get_with_filter (object, pspec, &pval, sizeof(pval)); \
        G_PASTE (g_value_set_, GPROPZ_##TYPE##_VALUE) (value, pval); \
      }

  GPROPZ_GET_PROPERTY_IF (BOOLEAN)
  else GPROPZ_GET_PROPERTY_IF (CHAR)
  else GPROPZ_GET_PROPERTY_IF (UCHAR)
  else GPROPZ_GET_PROPERTY_IF (INT)
  else GPROPZ_GET_PROPERTY_IF (UINT)
  else GPROPZ_GET_PROPERTY_IF (UNICHAR)
  else GPROPZ_GET_PROPERTY_IF (LONG)
  else GPROPZ_GET_PROPERTY_IF (ULONG)
  else GPROPZ_GET_PROPERTY_IF (INT64)
  else GPROPZ_GET_PROPERTY_IF (UINT64)
  else GPROPZ_GET_PROPERTY_IF (FLOAT)
  else GPROPZ_GET_PROPERTY_IF (DOUBLE)
  else GPROPZ_GET_PROPERTY_IF (ENUM)
  else GPROPZ_GET_PROPERTY_IF (FLAGS)
  else GPROPZ_GET_PROPERTY_IF (STRING)
  else GPROPZ_GET_PROPERTY_IF (PARAM)
  else GPROPZ_GET_PROPERTY_IF (BOXED)
  else GPROPZ_GET_PROPERTY_IF (POINTER)
  else GPROPZ_GET_PROPERTY_IF (OBJECT)
  else GPROPZ_GET_PROPERTY_IF (GTYPE)
  else GPROPZ_GET_PROPERTY_IF (VARIANT)
  else
    g_warning ("Unexpected param pspec for %s", g_param_spec_get_name (pspec));
}

/**
 * gpropz_auto_set_property:
 *
 * This function may be placed in #GObjectClass.get_property in order to use gpropz's automated
 * setter.
 */
void
gpropz_auto_set_property (GObject      *object,
                          guint         prop_id,
                          const GValue *value,
                          GParamSpec   *pspec)
{
  #define GPROPZ_SET_PROPERTY_IF(TYPE) \
    if (G_IS_PARAM_SPEC_##TYPE (pspec)) \
      { \
        GPROPZ_##TYPE##_TYPE pval = G_PASTE (g_value_get_, \
                                             GPROPZ_##TYPE##_VALUE) (value); \
        _gpropz_internal_set_with_filter (object, pspec, &pval, sizeof(pval)); \
      }

  GPROPZ_SET_PROPERTY_IF (BOOLEAN)
  else GPROPZ_SET_PROPERTY_IF (CHAR)
  else GPROPZ_SET_PROPERTY_IF (UCHAR)
  else GPROPZ_SET_PROPERTY_IF (INT)
  else GPROPZ_SET_PROPERTY_IF (UINT)
  else GPROPZ_SET_PROPERTY_IF (UNICHAR)
  else GPROPZ_SET_PROPERTY_IF (LONG)
  else GPROPZ_SET_PROPERTY_IF (ULONG)
  else GPROPZ_SET_PROPERTY_IF (INT64)
  else GPROPZ_SET_PROPERTY_IF (UINT64)
  else GPROPZ_SET_PROPERTY_IF (FLOAT)
  else GPROPZ_SET_PROPERTY_IF (DOUBLE)
  else GPROPZ_SET_PROPERTY_IF (ENUM)
  else GPROPZ_SET_PROPERTY_IF (FLAGS)
  else GPROPZ_SET_PROPERTY_IF (STRING)
  else GPROPZ_SET_PROPERTY_IF (PARAM)
  else GPROPZ_SET_PROPERTY_IF (BOXED)
  else GPROPZ_SET_PROPERTY_IF (POINTER)
  else GPROPZ_SET_PROPERTY_IF (OBJECT)
  else GPROPZ_SET_PROPERTY_IF (GTYPE)
  else GPROPZ_SET_PROPERTY_IF (VARIANT)
  else
    g_warning ("Unexpected param pspec for %s", g_param_spec_get_name (pspec));
}

/**
 * gpropz_class_init_property_functions:
 * @object_class: The #GObjectClass to set properties on.
 *
 * Set's the @object_class's get_property and set_property attributes to
 * gpropz_auto_get_property() and gpropz_auto_set_property(), respectively.
 */
void
gpropz_class_init_property_functions (GObjectClass *object_class)
{
  object_class->get_property = gpropz_auto_get_property;
  object_class->set_property = gpropz_auto_set_property;
}

/**
 * gpropz_bind_property_full:
 * @struct_offset: The offset into the object structure that this property should be bound to.
 * @prop_id: The ID associated with this property.
 * @pspec: The property's parameter spec.
 * @filter: (nullable): An optional filter that will manage reads/writes.
 *
 *
 * Binds the member located at @struct_offset within the object to the GObject property passed
 * in @prop_id and @pspec. This means that gpropz_auto_set_property() and
 * gpropz_auto_get_property() will automatically write to / read from this location in the
 * object structure. @filter specifies a #GpropzValueFilter that will be applied to the property,
 * meaning that any writes or reads will go through #GpropzValueFilter.set_filter and
 * #GpropzValueFilter.get_filter, respectively, instead of being directly written to the object.
 *
 * In most cases, you want to use gpropz_bind_property() or gpropz_bind_property_private()
 * instead, or the auto-install functions gpropz_install_property() and
 * gpropz_install_property_private().
 */
void
gpropz_bind_property_full (gsize              struct_offset,
                           guint              prop_id,
                           GParamSpec        *pspec,
                           GpropzValueFilter *filter)
{
  GpropzInternalData *data = g_new0 (GpropzInternalData, 1);
  data->struct_offset = struct_offset;
  data->prop_id = prop_id;
  data->filter = filter;
  g_param_spec_set_qdata_full (pspec, GPROPZ_INTERNAL_DATA, data, g_free);
}

/**
 * gpropz_install_property_full:
 *
 * Shorthand for calling g_object_class_install_property() and gpropz_install_property_full()
 * in succession.
 */
void
gpropz_install_property_full (GObjectClass      *object_class,
                              gsize              struct_offset,
                              guint              prop_id,
                              GParamSpec        *pspec,
                              GpropzValueFilter *filter)
{
  g_object_class_install_property (object_class, prop_id, pspec);
  gpropz_bind_property_full (struct_offset, prop_id, pspec, filter);
}
