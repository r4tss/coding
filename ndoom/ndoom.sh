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
a=9
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

function rr {
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
	if [ $(echo "$a < 0" | bc) -eq 1 ]
	then
		ab=$(echo "$a - ($PI * 2)" | bc)
	else
		ab=$a
	fi
	ax=$a
	ay=$(echo "($PI * 0.5) - $a" | bc)
	if [ $(echo "$1 > 0" | bc) -eq 1 ] && [ $(echo "$1 < ($PI * 0.5)" | bc) -eq 1 ]
	then
		stepX=1
		stepY=1
		ax=$ab
		ay=$(echo "(PI * 0.5) - ab" | bc)
	elif [ $(echo "$1 > ($PI * 0.5)" | bc -l) -eq 1 ] && [ $(echo "$1 < $PI" | bc -l) -eq 1 ]
	then
		stepX=-1
		stepY=1
		ax=$(echo "$PI - $ab" | bc)
		oy=$(echo "$ab - ($PI * 0.5)" | bc)
	elif [ $(echo "$1 > $PI" | bc -l) -eq 1 ] && [ $(echo "$1 < ($PI * 1.5)" | bc -l) -eq 1 ]
	then
		stepX=-1
		stepY=-1
		ax=$(echo "$ab - $PI" | bc)
		ay=$(echo "($PI * 1.5) - $ab" | bc)
	else
		stepX=1
		stepY=-1
		ax=$(echo "($PI * 2) - $ab" | bc)
		ay=$(echo "$ab - ($PI * 1.5)" | bc)
	fi
}

function check_arr {

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
	rc=0
	rx=$(rr 0 $x)
	ry=$(rr 1 $y)
	sideX=$(echo "$x - $rx" | bc)
	sideY=$(echo "$y - $ry" | bc)
	while [ $rc -lt 61 ]
	do
		hit=0
		rlc=0
		ra=$(echo "$a + $sa" | bc)
		dir $ra
		dx=$(echo "1 / (s($ax) / c($ax))" | bc -l)
		dy=$(echo "1 / (s($ay) / c($ay))" | bc -l)
		#echo $stepX $stepY
		while [ $rlc -lt 2 ]
		do
			a=$(echo "$rx + ($mapSize * $ry)" | bc)
			echo $a $rx $ry
			if [ $rc -eq 0 ]
			then
				rx=$(echo "if ($stepX==1) ($x / 1) + 1 else $x / 1" | bc)
				ry=$(echo "if ($stepY==1) ($y / 1) + 1 else $y / 1" | bc)
				((rlc++))
			elif [ ${map[@]} -eq 1 ]
			then
				hit=1
				((rlc++))
			else
				rx=$(echo "")
			fi
		done
		sa=$(echo "$sa + ($PI / 180)" | bc -l)
		(( rc++ ))
	done
	break
done
