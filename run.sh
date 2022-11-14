#!/bin/bash

CMD="$1"

case "$CMD" in 
	GOOD)
	_cmd="./x64/Debug/Project1.exe ./shakespeare ./tmp ./out ./x64/Debug/MapDll.dll ./x64/Debug/ReduceDll.dll"
	$_cmd
	;;

	BAD)
	_cmd="./x64/Debug/Project1.exe ./shakespeare ./tmp ./out ./x64/Debug/MapDll.lol ./x64/Debug/ReduceDll.dll"
	echo $_cmd
	$_cmd
	;;

	*)
	;;

esac
