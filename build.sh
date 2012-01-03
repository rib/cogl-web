#!/bin/sh

# template.html file can have any number of "mustache" markers like {{foo}} and
# {{bar}}
#
# This script simply greps for all the markers in template.html and then for
# each sub directory under pages/ we make a www/DIR.html by copying
# template.html and substituting any pages/DIR/MUSTACHE_MARKER_NAME.html files
# we find that matches the names we found in the template. Markers without a
# corresponding file are simply stripped.

for page in `find ./pages -maxdepth 1 -mindepth 1 -type d`
do
    echo "Generating www/`basename $page`.html"
    cp template.html www/`basename $page`.html

    for marker in `grep '{{' template.html|sed -e 's/{{\([a-zA-Z_-]\+\)}}/\1/'`
    do
	if test -f $page/$marker.html; then
	    echo "  substituting {{$marker}} with $page/$marker.html"
	    sed -i "/{{$marker}}/ r $page/$marker.html" www/`basename $page`.html
	fi
    done

    # Remove any remaining un-substituted mustache markers.
    sed -i 's/{{\([a-zA-Z_-]\+\)}}//' www/`basename $page`.html
done

