# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.

glib_prefix = glib_dep.get_pkgconfig_variable('prefix')
gtk_docs = join_paths(glib_prefix, 'share', 'gtk-doc', 'html')

subdir('xml')

gnome.gtkdoc('gpropz',
    main_xml : 'gpropz-docs.xml',
    src_dir : gpropz_inc,
    content_files : [readme_xml],
    dependencies : [gpropz_dep],
    scan_args : [
        '--rebuild-sections',
    ],
    mkdb_args : [
        '--default-includes=gpropz.h',
    ],
    fixxref_args : [
        '--extra-dir=@0@'.format(join_paths(gtk_docs, 'glib')),
        '--extra-dir=@0@'.format(join_paths(gtk_docs, 'gobject')),
    ],
)
