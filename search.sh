#!/bin/sh                                                                                                                                 

storydir=/var/public/discuss/story
woc "$@" -- $storydir/* | sed "s|[0-9]*\\t$storydir/||"
