#!/bin/bash

ARGS=( "$@" )

IFS=';'

#echo $#

if [ $# -lt 2 ]
then
	#echo "za mało parametrów"
	exit 1
fi

if ! [ -f $1 ]
then
	#echo "plik nie istnieje"
	exit 1
fi

for(( i=1; i<$#; i++)); do
	#echo "${ARGS[i]}"
	grep "^${ARGS[i]};" $1 > grep.txt
#	cat grep.txt
	read -a NAMES < grep.txt
	LEN=0

	#echo "${ARGS[i]}"
	if ! [[ "${ARGS[i]}" =~ ^[0-9]{1,3}$ ]]
	then
		#echo "id spoza zakresu"
		exit 1
	fi

	if ! [ "${#NAMES[@]}" == 0 ]
	then
		for((j=2; j<"${#NAMES[@]}"; j+=3)); do
			LEN=$(( LEN + "${NAMES[j]}" ))
			#echo "${NAMES}"
		done
		echo "${NAMES[0]};$LEN"
	fi
done

if [ -f grep.txt ]
then
	rm grep.txt
fi
