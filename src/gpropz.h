/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef GPROPZ_H
#define GPROPZ_H

#include <glib.h>
#include <glib-object.h>

#include "gpropz-config.h"

G_BEGIN_DECLS

typedef struct _GpropzValueFilter {
  void (*get_filter) (GObject      *object,
                      gconstpointer prop,
                      guint         prop_id,
                      gpointer      target,
                      GParamSpec   *pspec);

  void (*set_filter) (GObject      *object,
                      gpointer      prop,
                      guint         prop_id,
                      gconstpointer source,
                      GParamSpec   *pspec);
} GpropzValueFilter;

void _gpropz_internal_get_with_filter (GObject    *object,
                                       GParamSpec *pspec,
                                       gpointer    target,
                                       gsize       size);
void _gpropz_internal_set_with_filter (GObject      *object,
                                       GParamSpec   *pspec,
                                       gconstpointer source,
                                       gsize         size);

void gpropz_auto_get_property (GObject    *object,
                               guint       prop_id,
                               GValue     *value,
                               GParamSpec *spec);

void gpropz_auto_set_property (GObject      *object,
                               guint         prop_id,
                               const GValue *value,
                               GParamSpec   *spec);

void gpropz_class_init_property_functions (GObjectClass *object_class);

void gpropz_bind_property_full (GObjectClass      *object_class,
                                gsize              struct_offset,
                                guint              prop_id,
                                GParamSpec        *pspec,
                                GpropzValueFilter *filter);

void gpropz_install_property_full (GObjectClass      *object_class,
                                   gsize              struct_offset,
                                   guint              prop_id,
                                   GParamSpec        *pspec,
                                   GpropzValueFilter *filter);

#define gpropz_bind_property(klass, TypeName, member_name, prop_id, pspec, filter) \
  gpropz_bind_property_full (klass, offsetof (TypeName, member_name), prop_id, pspec, filter)

#define gpropz_bind_property_private(klass, TypeName, member_name, prop_id, pspec, filter) \
  gpropz_bind_property_full (klass, G_PRIVATE_OFFSET (TypeName, member_name), prop_id, pspec, \
                             filter)

#define gpropz_install_property(klass, TypeName, member_name, prop_id, pspec, filter) \
  gpropz_install_property_full (klass, offsetof (TypeName, member_name), prop_id, pspec, filter)

#define gpropz_install_property_private(klass, TypeName, member_name, prop_id, pspec, filter) \
  gpropz_install_property_full (klass, G_PRIVATE_OFFSET (TypeName, member_name), prop_id, \
                                pspec, filter)

#define GPROPZ_DEFINE_RO_FULL(member_type, TypeName, type_name, method_name, member_name, pspec) \
  member_type \
  type_name##_get_##method_name (TypeName* _obj) \
  { \
    member_type tmp; \
    _gpropz_internal_get_with_filter (G_OBJECT (_obj), pspec, &tmp, sizeof(member_type)); \
    return tmp; \
  }

#define GPROPZ_DEFINE_RO(member_type, TypeName, type_name, name, pspec) \
  GPROPZ_DEFINE_RO_FULL (member_type, TypeName, type_name, name, name, pspec)

#define GPROPZ_DEFINE_RW_FULL(member_type, TypeName, type_name, method_name, member_name, pspec) \
  GPROPZ_DEFINE_RO_FULL (member_type, TypeName, type_name, method_name, member_name, pspec) \
  void \
  type_name##_set_##method_name (TypeName* _obj, member_type _new) \
  { \
    _gpropz_internal_set_with_filter (G_OBJECT (_obj), pspec, &_new, sizeof(member_type)); \
  }

#define GPROPZ_DEFINE_RW(member_type, TypeName, type_name, name, pspec) \
  GPROPZ_DEFINE_RW_FULL (member_type, TypeName, type_name, name, name, pspec)

G_END_DECLS

#endif
