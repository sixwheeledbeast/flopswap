#!/bin/sh

source /opt/flopswap/script/setvari.sh

echo $swap
#Print to file
echo $swap > "$locatetempfile"
banner "$swap"
 if [ "$swap" = "$swap1" ] ;
  then
    if [ -e $inttempfile ];
     then
      rm "$inttempfile"
    fi
 elif [ "$swap" = "$swap2" ] ;
   then
    if [ -e $inttempfile ];
     then
      rm "$inttempfile"
    fi
 elif [ "$swap" = "$swap0" ] ;
  then
   touch "$inttempfile"
 else
  echo "You shouldn't have got here"
   exit 1
 fi
exit 0
