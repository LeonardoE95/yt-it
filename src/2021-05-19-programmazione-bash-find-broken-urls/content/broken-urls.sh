#!/usr/bin/env sh

# Made by Leonardo Tamiano on 2021-05-19
# Related video: 

# dependencies:
# - pup (go)
# - xmlstarlet
# - curl

# ---------------------------------------------
# Globals

DATA_DIR="/home/leo/repos/ppa/"

ROOT_URL="https://leonardotamiano.xyz"

EXAMPLE_FILE="/home/leo/repos/ppa/education/university/master/SABD/lecture_notes/sabd_03.html"
FULL_URLS_FILE="/tmp/full_list_urls.txt"
PARTIAL_URLS_FILE="/tmp/partial_list_urls.txt"
BROKEN_URLS_FILE="/tmp/broken_urls.txt"

PUP_PATH="/home/leo/go/bin/pup"
XMLSTARLET_PATH=$(which "xml" 2> /dev/null)

# ---------------------------------------------

main () {
    # -- requirements
    [ ! -f "$PUP_PATH" ] && [ ! -f "$XMLSTARLET_PATH" ] && echo "Missing requirements: xmlstarlet, pup" && exit
    
    generate_urls_file
    generate_broken_urls_file
}

# ---------------------------------------------
# Extract url list

generate_urls_file() {
    rm -f $FULL_URLS_FILE
    rm -f $PARTIAL_URLS_FILE
    
    pushd $DATA_DIR > /dev/null
    # -- extract all urls 
    recursive_iterate
    popd > /dev/null
    # -- force C locale to have byte-wise sorting
    LC_ALL=C sort -u $PARTIAL_URLS_FILE -o $PARTIAL_URLS_FILE
}

recursive_iterate() {
    for f in *; do
	# NOTE: maybe support more file extensions?
	if [ -f "$f" ] && [ "${f: -5}" == ".html" ]; then
	    # -- file
	    extract_all_urls $f $FULL_URLS_FILE $PARTIAL_URLS_FILE
	elif [ -d "$f" ]; then
	    # -- dir
	    cd "$f"
	    recursive_iterate
	    cd ..
	fi
    done
}

extract_all_urls() {
    local in_file=$1
    local full_out_file=$2
    local partial_out_file=$3
    
    while IFS= read -r url
    do
	if [[ $url == /* ]]; then
	    # -- local link
	    echo "$ROOT_URL$url" >> $partial_out_file
	    echo "$ROOT_URL$url $(pwd)/$in_file" >> $full_out_file
	elif [[ $url == https://* ]] || [[ $url == http://* ]] ; then
	    # -- absolute link
	    echo "$url" >> $partial_out_file
	    echo "$url $(pwd)/$in_file" >> $full_out_file
	else
	    : # -- no-op
	fi 

	# requires pup
    done < <($PUP_PATH 'a[href] attr{href}' < $in_file)    
}

# ---------------------------------------------
# Iterate over all urls and check if they exist

generate_broken_urls_file() {
    rm -f $BROKEN_URLS_FILE
    check_for_broken_urls $FULL_URLS_FILE $PARTIAL_URLS_FILE $BROKEN_URLS_FILE
}

check_for_broken_urls() {
    local full_file=$1
    local partial_file=$2
    local err_file=$3
    
    while IFS= read -r url
    do
	# unescape HTML characters such as "&#39;" > "'"
	# requires xmlstarlet
	unescaped_url=$($XMLSTARLET_PATH unesc <<< "$url")
	if curl --output /dev/null --silent --head --fail "$unescaped_url"; then
	    : # no-op
	else
	    # -- url not found
	    echo $url >> $err_file
	    urlEscaped=$(sed 's/[^^]/[&]/g; s/\^/\\^/g' <<<"$url") # escape it.
	    while IFS= read -r line
	    do
		stringarray=($line)
		echo "    ${stringarray[1]}" >> $err_file
	    done < <(grep -E "^$urlEscaped( | $)" "$full_file")
	fi
    done < $partial_file
}

main
