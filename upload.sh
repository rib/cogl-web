#!/bin/sh

rsync -vr -e ssh --exclude cogl2-reference --delete www/ annarchy.freedesktop.org:/srv/cogl3d.org/www/
