#!/bin/sh

rsync -vr -e ssh --exclude cogl-reference --exclude cogl-gst-reference --exclude cogl-path-reference --exclude cogl-pango-reference --delete www/ annarchy.freedesktop.org:/srv/cogl3d.org/www/
