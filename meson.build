# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.

# Make sure this is updated in sync with gpropz.h
project('gpropz', 'c', version: '0.1.0')

gnome = import('gnome')

glib_dep = dependency('glib-2.0')
gobject_dep = dependency('gobject-2.0')

subdir('src')
subdir('demo')

gtk_doc = get_option('gtk-doc')
pandoc = find_program('pandoc', required : gtk_doc)
if pandoc.found()
    subdir('docs')
endif
