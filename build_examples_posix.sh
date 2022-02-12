#!/bin/sh

set -e

CC=cc
CFLAGS="-Wall -pedantic -O2 -Isrc/include"
verbose=0

build() {
	for file in $files; do
		source_file="src/$(echo $file | sed -e 's/_example//')"
		out_file="out/$(echo $file | sed -e 's/\.c/.exe/')"
		if [ -e "$source_file" ]; then
			command="$CC $CFLAGS -o $out_file examples/$file $source_file"
		else
			command="$CC $CFLAGS -o $out_file examples/$file"
		fi
		if [ $verbose -eq 1 ]; then
			echo $command
		fi
		$command
	done
}

if [ ! -d out ]; then
	mkdir out
fi

files=""
if [ -z "${1+x}" ]; then
	for file in examples/*; do
		files="$(basename $file) $files"
	done
	build
	exit
fi

case $1 in
	help)
		echo "Usage: $0 [help] [examples]"
		echo "Builds given examples. Defaults to all."
		exit
		;;

	all)
		for f in examples/*; do
			files="$files $f"
		done
		build
		exit
		;;

	-v)
		verbose=1
		shift
		;;
esac

if [ ! -z "${files+x}" ]; then
	while [[ $# -gt 0 ]]; do
		files="$files $1_example.c"
		shift
	done
fi

build
