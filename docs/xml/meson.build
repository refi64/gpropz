# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.

pandoc = find_program('pandoc')

readme_xml = custom_target('gen-readme',
    output : 'readme.xml',
    input : ['pandoc.sh', '../../README.md'],
    command : ['@INPUT0@', pandoc, '@INPUT1@', '@OUTPUT@'],
)
