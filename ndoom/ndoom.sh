#!/bin/ksh93
readonly PI=3.1415926535897932384626
sw=10
sh=5
w=0
h=0
x=3
y=3
dx=0
dy=0
a=2
quit=0
stepX=0
stepY=0
sa=$(echo "($PI / 6) * -1" | bc -l)
ba=$(echo "$PI / 6" | bc -l)

mapSize=8
map=(	1 1 1 1 1 1 1 1 \ 
	1 0 0 0 0 0 0 1 \ 
	1 0 0 0 0 0 0 1 \ 
	1 0 0 0 0 0 0 1 \ 
	1 0 0 0 0 0 0 1 \ 
	1 0 0 0 0 0 0 1 \ 
	1 0 0 0 0 0 0 1 \ 
	1 0 0 0 0 0 0 1 \ 
	1 1 1 1 1 1 1 1 )

function dd {
	if [ $1 -eq 0 ]
	then
		ox=$2
		ddx=$(echo "$ox / 1" | bc)
		rx=$(echo "a=($ox - $ddx); b=(1 - a); if(a > 0.5) a=($ox + b) else a=$ddx; a/1" | bc)
		echo $rx
	elif [ $1 -eq 1 ]
	then
		oy=$2
		ddy=$(echo "$oy / 1" | bc)
		ry=$(echo "a=($oy - $ddy); b=(1 - a); if(a > 0.5) a=($oy + b) else a=$ddy; a/1" | bc)
		echo $ry
	fi
}

function dir {
	if [ $(echo "$1 < ($PI * 0.5)" | bc) -eq 1 ]
	then
		stepX=1
		stepY=1
	elif [ $(echo "$1 > ($PI * 0.5)" | bc -l) -eq 1 ] && [ $(echo "$1 < $PI" | bc -l) -eq 1 ]
	then
		stepX=-1
		stepY=1
	elif [ $(echo "$1 > $PI" | bc -l) -eq 1 ] && [ $(echo "$1 < ($PI * 1.5)" | bc -l) -eq 1 ]
	then
		stepX=-1
		stepY=-1
	else
		stepX=1
		stepY=-1
	fi
}

#while [ $h -lt $sh ]
#do
#	while [ $w -lt $sw ]
#	do
#		echo -n '#'
#		((w++))
#	done
#	echo -ne '\n'
#	((h++))
#	w=0
#done

while [ !$quit ]
do
#	while [ !$(echo "$sa == $ba" | bc) ]
#	do
	#	hit=0
	#	ra=$(echo "$a + $sa" | bc)
	#	dir $ra
	#	dx=$(dd 0 $x)
	#	dy=$(dd 1 $y)
	#	sideX=$(echo "if ($x > $dx) $x - $dx else $dx - $x" | bc)
	#	sideY=$(echo "if ($y > $dy) $y - $dy else $dy - $y" | bc)
		sa=$(echo "$sD + ($PI / 180)" | bc -l)
	#	echo $sideX
#	done
	break
done
