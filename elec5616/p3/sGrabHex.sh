#! /bin/sh
i=$1

while [ $i -le $2 ]
do
	echo "x/bx main+$i"
	i=$(($i + 1))

done
