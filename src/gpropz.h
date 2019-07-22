/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef GPROPZ_H
#define GPROPZ_H

#include <glib.h>
#include <glib-object.h>

// Make sure this is updated in sync with the root directory's meson.build
#define GPROPZ_VERSION "0.1.0"

G_BEGIN_DECLS

/**
 * GpropzValueFilter:
 * @get_filter: This function, if not NULL, must read the given property (located at @prop),
 * apply whatever transformations are desired, and then write it to @target.
 * @set_filter: This function, if not NULL, must write to the property (located at @prop)
 * the value located at @source.
 *
 * A "filter" is a pair of get/set functions that override gpropz's magic getters and setters
 * with their own, custom versions. Unlike the normal GObject set_property / get_property
 * functions, these take raw pointers instead of #GValue instances, because they are also
 * for the auto-generated, property-specific C getters and setters, and we want to avoid
 * constructing #GValue instances every time.
 *
 * Filters also allow you to have a different property type than you tell GLib; simply convert
 * between the types in the filter functions.
 */
typedef struct _GpropzValueFilter {
  /*< public >*/

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

/**
 * gpropz_obj_ref_filter:
 *
 * A default filter that calls g_object_ref() on the input for both get_filter and set_filter.
 * If you only want one of the two behaviors, use gpropz_obj_ref_filter.get_filter or
 * gpropz_obj_ref_filter.set_filter to build your own filter structure.
 */
extern GpropzValueFilter gpropz_obj_ref_filter;

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

void gpropz_bind_property_full (gsize              struct_offset,
                                guint              prop_id,
                                GParamSpec        *pspec,
                                GpropzValueFilter *filter);

void gpropz_install_property_full (GObjectClass      *object_class,
                                   gsize              struct_offset,
                                   guint              prop_id,
                                   GParamSpec        *pspec,
                                   GpropzValueFilter *filter);

/**
 * gpropz_bind_property:
 *
 * Shorthand for gpropz_bind_property_full() that passes offsetof (@TypeName, @member_name)
 * as the structure offset to use.
 */
#define gpropz_bind_property(TypeName, member_name, prop_id, pspec, filter) \
  gpropz_bind_property_full (offsetof (TypeName, member_name), prop_id, pspec, filter)

/**
 * gpropz_bind_property_private:
 *
 * Shorthand for gpropz_bind_property_full() that passes G_PRIVATE_OFFSET
 * (@TypeName, @member_name) as the structure offset to use.
 */
#define gpropz_bind_property_private(TypeName, member_name, prop_id, pspec, filter) \
  gpropz_bind_property_full (G_PRIVATE_OFFSET (TypeName, member_name), prop_id, pspec, filter)

/**
 * gpropz_install_property:
 *
 * Shorthand for gpropz_install_property_full() that passes offsetof (@TypeName, @member_name)
 * as the structure offset to use.
 */
#define gpropz_install_property(klass, TypeName, member_name, prop_id, pspec, filter) \
  gpropz_install_property_full (klass, offsetof (TypeName, member_name), prop_id, pspec, filter)

/**
 * gpropz_install_property_private:
 *
 * Shorthand for gpropz_install_property_full() that passes G_PRIVATE_OFFSET (@TypeName,
 * @member_name) as the structure offset to use.
 */
#define gpropz_install_property_private(klass, TypeName, member_name, prop_id, pspec, filter) \
  gpropz_install_property_full (klass, G_PRIVATE_OFFSET (TypeName, member_name), prop_id, \
                                pspec, filter)

/**
 * GPROPZ_DEFINE_RO:
 * @member_type: The property's type.
 * @TypeName: The PascalCase name of your class.
 * @type_name: The snake_case name of your class.
 * @method_name: The name of the getter, e.g. if this "my_prop", then the generated getter will
 * be named type_name_get_my_prop. This name does not have to be the same as the property's name.
 * @pspec: A global instance of the GParamSpec that represents the property to get.
 *
 * Automatically defines a C getter function to get the given property.
 */
#define GPROPZ_DEFINE_RO(member_type, TypeName, type_name, method_name, pspec) \
  member_type \
  type_name##_get_##method_name (TypeName* _obj) \
  { \
    member_type tmp; \
    _gpropz_internal_get_with_filter (G_OBJECT (_obj), pspec, &tmp, sizeof(member_type)); \
    return tmp; \
  }

/**
 * GPROPZ_DEFINE_RW:
 *
 * Identical to GPROPZ_DEFINE_RO(), but defines a setter as well.
 */
#define GPROPZ_DEFINE_RW(member_type, TypeName, type_name, method_name, pspec) \
  GPROPZ_DEFINE_RO (member_type, TypeName, type_name, method_name, pspec) \
  void \
  type_name##_set_##method_name (TypeName* _obj, member_type _new) \
  { \
    _gpropz_internal_set_with_filter (G_OBJECT (_obj), pspec, &_new, sizeof(member_type)); \
  }

G_END_DECLS

#endif
