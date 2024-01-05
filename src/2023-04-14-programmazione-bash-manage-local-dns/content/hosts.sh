#!/usr/bin/env sh

# Interface:
#
#  ./hosts add <IP_ADDRESS> <DOMAIN_1> <DOMAIN_2> ... <DOMAIN_N> ...
#
#  ./hosts modify <DOMAIN> <DOMAIN_1> <DOMAIN_2> ... <DOMAIN_N> ...
# 
#  ./hosts modify <IP_ADDRESS_1> <IP_ADDRESS_2>

# --------------------------------
# GLOBAL VARIABLES

OFFICIAL_CONF="/etc/hosts"
TMP_CONF="/tmp/hosts"
FILLER="<FILLER>"

# --------------------------------
# FUNCTIONS

ip_valid() {
    ip=$1
    
    if [[ "$ip" =~ ^(([1-9]?[0-9]|1[0-9][0-9]|2([0-4][0-9]|5[0-5]))\.){3}([1-9]?[0-9]|1[0-9][0-9]|2([0-4][0-9]|5[0-5]))$ ]]; then
	echo "true"
    else
	echo "false"
    fi
}

# --------------------------------
# STEP 1: create tmp copy

cp $OFFICIAL_CONF $TMP_CONF

# --------------------------------
# STEP 2: understand mode of operation

ARG1=$1
shift

if [ "$ARG1" = "add" ]; then
    ADD=true
elif [ "$ARG1" = "modify" ]; then
    ADD=false
else
    echo "Usage: hosts [add|modify] <args>"
    echo "Examples:"
    echo -e "   ./hosts add 192.192.192.192 bashed1 bashed2"
    echo -e "   ./hosts modify bashed1 bashed.htb"
    echo -e "   ./hosts modify 192.192.192.192 1.1.1.1"
    exit 0
fi

# --------------------------------
# STEP 3: execute action

ARG2=$1
REM_ARGS=$(( $# - 1 ))

# add action 1
if [ "$ADD" = "true" ] && [ $(ip_valid $ARG2) = "true" ] && [ $REM_ARGS -ge 1 ]; then
    # echo "add action 1"

    # check if IP already exists in conf
    new_line=$@
    old_line=$(grep $ARG2 $TMP_CONF)

    if [ $? -eq 0 ]; then
	# if it exists, asks for change
	echo "IP (${ARG2}) already exists with conf"
	echo ""
	echo "   '${old_line}'"
	echo ""

	echo "Replace current line with new one?"
	select yn in "Yes" "No"; do
	    case $yn in
		Yes ) break;;
		No ) exit;;
	    esac
	done
    else
	old_line=$FILLER
	new_line="${new_line}\n${FILLER}"
    fi

    # add new_line it
    sed_arg="s/${old_line}/${new_line}/"
    sed -i "${sed_arg}" $TMP_CONF    
fi

# modify action 1
if [ "$ADD" = "false" ] && [ $(ip_valid $ARG2) = "false" ] && [ $REM_ARGS -ge 1 ]; then
    # echo "modify action 1"

    # discover IP assigned to ARG2
    ip=$(grep $ARG2 $TMP_CONF | cut -d' ' -f1)

    if [ -z $ip ] || [ $(ip_valid $ip) = "false" ]; then
	echo "ERROR: missing domain (${ARG2})"
	exit
    fi

    shift # remove $ARG2 from list of args

    # add remaining domains to the same IP as $ARG2
    old_line=$(grep $ARG2 $TMP_CONF)
    new_line="${old_line} ${@}"
    
    sed_arg="s/${old_line}/${new_line}/"
    sed -i "${sed_arg}" $TMP_CONF

    echo "INFO: updated domains associated to (${ARG2})"
    echo "    ${new_line}"
    
fi

# modify action 2
if [ "$ADD" = "false" ] && [ $(ip_valid $ARG2) = "true" ] && [ $REM_ARGS -eq 1 ] && [ $(ip_valid $2) = "true" ] ; then

    # check if IP1 ($ARG2) is already present in the conf
    old_line=$(grep $ARG2 $TMP_CONF)

    if [ $? -eq 1 ]; then
	echo "ERROR: missing ip (${ARG2})"
	exit 1
    fi

    # otherwise, replace the old IP ($ARG2) with the new IP ($2)
    old_ip=$(echo $old_line | cut -d' ' -f1)
    new_ip=$2

    sed_arg="s/${old_ip}/${new_ip}/"
    sed -i "${sed_arg}" $TMP_CONF
fi

# --------------------------------
# STEP 4: copy back modified version

cp $TMP_CONF $OFFICIAL_CONF
