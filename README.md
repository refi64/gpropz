# gpropz

gpropz allows you to create GObject properties with far less boilerplate than by default.

## Highlights

- No need to define `*_get_property` and `*_set_property` functions manually. (You still can for
  more flexibility, however.)
- Automatically "bind" the getter (and optionally setter) for a property to a variable on
  your instance / private instance.
- Support for "filters", which allows you to transform / reject properties manually.

## Links

- [Website/documentation.](https://gpropz.refi64.com/)
- [Source.](https://github.com/refi64/gpropz)

## Embedding

gpropz is designed to be statically linked into your project. It generally follows the typical
style of a [Meson subproject](https://mesonbuild.com/Subprojects.html#a-simple-example). Example:

```python
gpropz_proj = subproject('gpropz')
gpropz_dep = gpropz_proj.get_variable('gpropz_dep')

# Use gpropz_dep in your project later on.
```

## Examples

See the `demo` directory for full examples.

### Registering new properties

```c
// In the *_class_init function:

GObjectClass *object_class = G_OBJECT_CLASS (klass);
gpropz_class_init_property_functions (object_class);
// The above line is shorthand for:
object_class->get_property = gpropz_auto_get_property;
object_class->set_property = gpropz_auto_set_property;
// It just assigns the get_property / set_property functions with the magic gpropz ones.

properties[PROP_NAME] =
  g_param_spec_string ("name",
                       "A name",
                       "This is some random name you probably don't care about",
                       NULL, G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);

gpropz_install_property (object_class, MyObject, name, PROP_NAME, properties[PROP_NAME],
                         NULL);
// The above line is shorthand for:
gpropz_bind_property (MyObject, name, PROP_NAME, properties[PROP_NAME]);
g_object_class_install_property (object_class, PROP_NAME, properties[PROP_NAME]);

// gpropz_bind_property creates a binding for a given property (PROP_NAME) and attaches it
// to a member of MyObject (MyObject.name).
// If you want to attach to a member of a private instance, use
// gpropz_bind_property_private/gpropz_install_property_private instead.
```

### Defining getters and setters

```c
// In the header file, they're declared as normal:

const char *myobject_get_name(MyObject *object);

int  myobject_get_age(MyObject *object);
void myobject_set_age(MyObject *object,
                      int       age);

// In the source file however, we don't have to define getters and setters manually,
// instead we can use the gpropz magic macros:

GPROPZ_DEFINE_RO (const char *, MyObject, my_object, name, properties[PROP_NAME])
GPROPZ_DEFINE_RW (int, MyObject, my_object, aged, properties[PROP_AGE])
```
