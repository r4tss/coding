#!/bin/zsh
W=0
H=0
height=0

echo ${distance[1]}

function a { #input format: distance, height, invert
	if [ $(echo "$(sed -n $(echo $((W + 1))p) distances) < ($height * 0.75)" | bc) -eq 1 ]
	then
		b="#"
	elif [ $H -gt 20 ]
	then
		b="/"
	else
		b=" "
	fi
}

while [ $H -lt 41 ]
do
	echo
	while [ $W -lt 91 ]
	do
		a
		if [ $W -eq 90 ]
		then
			echo $b
		else
			echo -n $b
		fi
		echo -n " "
		((W++))
	done
	((H++))
	if [ $H -lt 21 ]
	then
		((height++))
	else
		((height--))
	fi
	W=0
done
