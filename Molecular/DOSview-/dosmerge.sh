#!/bin/bash

#find first 99 dens files
str=`ls | grep dens_0`

> dens_map.dat
#first 2 colums from desn_TOT.dat
 awk 'BEGIN{FS=" "}{print $1}' dens_TOT.dat > dens_map.dat
 awk 'BEGIN{FS=" "}{print $2}' dens_TOT.dat > temp
 paste dens_map.dat temp > temp2 #add column
 mv -f temp2 dens_map.dat

#last-but-one colums from dens_0XX.dat
for i in $str;
do
 awk 'BEGIN{FS=" "}{print $(NF-1)}' $i > temp #take last word/number of line
 paste dens_map.dat temp > temp2 #add column
 mv -f temp2 dens_map.dat
done
rm -f temp
rm -f temp2