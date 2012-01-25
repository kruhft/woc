#!/bin/ksh
#
# search.ksh - prototype for woc
#
# Author: Burton Samograd <burton.samograd@gmail.com>
# Year: 2012

typeset -A counts
for word in "${@}"; do
    [[ "$word" = -* ]] && invert=-v
    for file in *; do
        echo $file:$(grep $invert -i -o -- "${word/-/}" $file | wc -l)
    done
done | sort -n -k1 | while IFS=: read n count; do
    counts[$n]=$((${counts[$n]}+count));
done
for i in ${!counts[*]}; do
    echo ${counts[$i]} $i
done | sort -r -V -k1,2 | sed /^0/d
