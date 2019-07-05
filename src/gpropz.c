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
typedef gchar * GPROPZ_STRING_TYPE;
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

  if (data->filter)
    data->filter->get_filter (object, prop, data->prop_id, target, pspec);
  else
    memcpy (target, prop, size);
}

void
_gpropz_internal_set_with_filter (GObject      *object,
                                  GParamSpec   *pspec,
                                  gconstpointer source,
                                  gsize         size)
{
  GpropzInternalData *data = g_param_spec_get_qdata (pspec, GPROPZ_INTERNAL_DATA);
  gpointer prop = ((gpointer) object) + data->struct_offset;

  if (data->filter)
    data->filter->set_filter (object, prop, data->prop_id, source, pspec);
  else
    memcpy (prop, source, size);
}

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

void
gpropz_auto_set_property (GObject      *object,
                          guint         prop_id,
                          const GValue *value,
                          GParamSpec   *pspec)
{
  #define GPROPZ_SET_PROPERTY_IF(TYPE, get_or_dup) \
    if (G_IS_PARAM_SPEC_##TYPE (pspec)) \
      { \
        GPROPZ_##TYPE##_TYPE pval = G_PASTE (g_value_##get_or_dup##_, \
                                             GPROPZ_##TYPE##_VALUE) (value); \
        _gpropz_internal_set_with_filter (object, pspec, &pval, sizeof(pval)); \
      }

  GPROPZ_SET_PROPERTY_IF (BOOLEAN, get)
  else GPROPZ_SET_PROPERTY_IF (CHAR, get)
  else GPROPZ_SET_PROPERTY_IF (UCHAR, get)
  else GPROPZ_SET_PROPERTY_IF (INT, get)
  else GPROPZ_SET_PROPERTY_IF (UINT, get)
  else GPROPZ_SET_PROPERTY_IF (UNICHAR, get)
  else GPROPZ_SET_PROPERTY_IF (LONG, get)
  else GPROPZ_SET_PROPERTY_IF (ULONG, get)
  else GPROPZ_SET_PROPERTY_IF (INT64, get)
  else GPROPZ_SET_PROPERTY_IF (UINT64, get)
  else GPROPZ_SET_PROPERTY_IF (FLOAT, get)
  else GPROPZ_SET_PROPERTY_IF (DOUBLE, get)
  else GPROPZ_SET_PROPERTY_IF (ENUM, get)
  else GPROPZ_SET_PROPERTY_IF (FLAGS, get)
  else GPROPZ_SET_PROPERTY_IF (STRING, dup)
  else GPROPZ_SET_PROPERTY_IF (PARAM, dup)
  else GPROPZ_SET_PROPERTY_IF (BOXED, dup)
  else GPROPZ_SET_PROPERTY_IF (POINTER, get)
  else GPROPZ_SET_PROPERTY_IF (OBJECT, dup)
  else GPROPZ_SET_PROPERTY_IF (GTYPE, get)
  else GPROPZ_SET_PROPERTY_IF (VARIANT, dup)
  else
    g_warning ("Unexpected param pspec for %s", g_param_spec_get_name (pspec));
}

void
gpropz_class_init_property_functions (GObjectClass *object_class)
{
  object_class->get_property = gpropz_auto_get_property;
  object_class->set_property = gpropz_auto_set_property;
}

void
gpropz_bind_property_full (GObjectClass      *object_class,
                           gsize              struct_offset,
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

void
gpropz_install_property_full (GObjectClass      *object_class,
                              gsize              struct_offset,
                              guint              prop_id,
                              GParamSpec        *pspec,
                              GpropzValueFilter *filter)
{
  gpropz_bind_property_full (object_class, struct_offset, prop_id, pspec, filter);
  g_object_class_install_property (object_class, prop_id, pspec);
}
