#!/bin/sh

rsync -vr -e ssh --delete www/ annarchy.freedesktop.org:/srv/cogl3d.org/www/
