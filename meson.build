# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.

project('gpropz', 'c', version: '0.1.0')

conf_data = configuration_data()
conf_data.set_quoted('GPROPZ_VERSION', meson.project_version())
configure_file(output : 'gpropz-config.h',
               configuration : conf_data)
conf_inc = include_directories('.')

dependencies = [
    dependency('glib-2.0'),
    dependency('gobject-2.0'),
]

subdir('src')
subdir('demo')
