# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.

gpropz_inc = include_directories('.')
gpropz_lib = static_library('gproz', 'gpropz.c',
                            include_directories : gpropz_inc,
                            dependencies : [glib_dep, gobject_dep])
gpropz_dep = declare_dependency(link_with : gpropz_lib,
                                include_directories : gpropz_inc)
