#!/usr/bin/env sh

# This script is used to test my own simple and incomplete
# implementation of the strings binary.

# -- list of binaries used for testing
BINARIES=(
    "sh"
    "python3.9"
    "sudo"
    "imcopy"
    "ppdi"
    "zzcat"
    "img2txt"
    "ppdmerge"
    "zzdir"
    "img2webp"
    "ppdpo"
    "zzxorcat"
    "img_cat"
    "pphs"
    "zzxorcopy"
    "imgcmp"
    "ppltotf"
    "zzxordir"
    "imginfo"
    "ppm2tiff"
    "img_stat"
    "ppm3d"
)

main () {
    # -- compile
    gcc strings.c -o strings

    cd ./testing

    # for each binary to test...
    for i in "${BINARIES[@]}";
    do
	full_path=$(which $i)

	# run official strings and mine
	../strings $full_path > my_strings_output
	strings $full_path > official_strings_output

	diff my_strings_output official_strings_output > diff

	if [ $? -eq 1 ]; then
	    echo "TEST: $full_path failed :("
	fi
	
    done
}

main $@
