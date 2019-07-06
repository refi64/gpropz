#!/usr/bin/bash

set -e
ninja -C _build gpropz-doc
gtkdoc-rebase --html-dir _build/docs/html --online
firebase deploy --only hosting
