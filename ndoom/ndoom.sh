#!/bin/zsh
readonly PI=3.1415926535897932384626
sw=10
sh=5
w=0
h=0
x=4
y=4
dx=0
dy=0
a=2
quit=0
stepX=0
stepY=0
sa=$(echo "($PI / 4) * -1" | bc -l)
ba=$(echo "$PI / 4" | bc -l)

mapSize=8
map=(1 1 1 1 1 1 1 1 1 0 0 0 0 0 0 1 1 0 0 0 0 0 0 1 1 0 0 0 0 0 0 1 1 0 0 0 0 0 0 1 1 0 0 0 0 0 0 1 1 0 0 0 0 0 0 1 1 0 0 0 0 0 0 1 1 1 1 1 1 1 1 1)

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
	ab=$1
	if [ $(echo "$1 > 0" | bc) -eq 1 ] && [ $(echo "$1 < ($PI * 0.5)" | bc) -eq 1 ]
	then
		stepX=1
		stepY=-1
		ax=$ab
		ay=$(echo "($PI * 0.5) - $ab" | bc)
	elif [ $(echo "$1 > ($PI * 0.5)" | bc -l) -eq 1 ] && [ $(echo "$1 < $PI" | bc -l) -eq 1 ]
	then
		stepX=-1
		stepY=-1
		ax=$(echo "$PI - $ab" | bc)
		ay=$(echo "$ab - ($PI * 0.5)" | bc)
	elif [ $(echo "$1 > $PI" | bc -l) -eq 1 ] && [ $(echo "$1 < ($PI * 1.5)" | bc -l) -eq 1 ]
	then
		stepX=-1
		stepY=1
		ax=$(echo "$ab - $PI" | bc)
		ay=$(echo "($PI * 1.5) - $ab" | bc)
	else
		stepX=1
		stepY=1
		ax=$(echo "($PI * 2) - $ab" | bc)
		ay=$(echo "$ab - ($PI * 1.5)" | bc)
	fi
}

#while [ !$quit ]
#do
echo -n > distances
rc=0
while [ $rc -lt 91 ]
do
	rx=$(rr 0 $x)
	ry=$(rr 1 $y)
	hit=0
	ra=$(echo "a=($a + $sa); if (a<0) a + ($PI * 2) else a" | bc)
	dir $ra
	dx=$(echo "a=(1 / (s($ax) / c($ax))); if (a>1) $stepX else a" | bc -l)
	dy=$(echo "a=(1 / (s($ay) / c($ay))); if (a>1) $stepY else a" | bc -l)
	rlc=0
	rx=$(echo "if ($stepX==1) ($x / 1) + 1 else $x / 1" | bc)
	ry=$(echo "if ($stepY==-1) ($y / 1) + 1 else $y / 1" | bc)
	while [ $hit -eq 0 ]
	do
		if [ ${map[$(echo "$rx + ($mapSize * $ry)" | bc)]} -eq 1 ]
		then
			hit=1
		else
			rx=$(echo "$rx + $dx" | bc)
			ry=$(echo "$ry + $dy" | bc)
		fi
	done
	sideX=$(echo "if($rx>$x) $rx - $x else $x - $rx" | bc)
	sideY=$(echo "if($ry>$y) $ry - $y else $y - $ry" | bc)
	echo "$(echo "sqrt(($sideX * $sideX) + ($sideY * $sideY))" | bc) " >> distances
	sa=$(echo "$sa + ($PI / 180)" | bc -l)
	(( rc++ ))
done
#./draw.sh
#	break
#done
