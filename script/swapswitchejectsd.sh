#!/bin/sh

source /opt/flopswap/script/setvari.sh

banner "$swap"
sleep 5

#Logic
if [ "$swap" = "$swap1" ] ;
then
   touch "$swaptempfile"
   $nice $xswapon $swap0
    banner "$prog"
   $nice $xswapoff $swap1
    if [ `awk '/dev/ {print $1}' /proc/swaps` = "$swap0" ] ;
     then
      dialog "$safe"
      touch "$inttempfile"
      rm "$swaptempfile"
    sleep 3
      banner "$swap0"
    fi
sleep 3
elif [ "$swap" = "$swap2" ] ;
then
   touch "$swaptempfile"
   $nice $xswapon $swap0
    banner "$prog"
   $nice $xswapoff $swap2
    if [ `awk '/dev/ {print $1}' /proc/swaps` = "$swap0" ]
     then
      dialog "$safe"
      touch "$inttempfile"
      rm "$swaptempfile"
    sleep 3
      banner "$swap0"
    fi
sleep 3
elif [ `awk '/dev/ {print $1}' /proc/swaps` = "$swap0" ] ;
  then
   sleep 1
    dialog "$safe"
    touch "$inttempfile"
     sleep 3
    banner "$swap0"
else
  dialog "$failed" 
sleep 3
 banner "$bcwarn" 
sleep 5
 banner "$strip" && exit 1
fi

