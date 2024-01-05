#!/usr/bin/env sh

# This script computes the percentage of people who received the first
# vaccine dosage in Lazio (Italy) by using the official github repo
# for covid19-vaccination-data.

# Related to the following youtube video: https://youtu.be/rXFRtZvU1Qw
# Made by Leonardo Tamiano

# ------------------

LOCAL_FILE_NAME="/tmp/vaccini.txt"
REMOTE_URL="https://raw.githubusercontent.com/italia/covid19-opendata-vaccini/master/dati/somministrazioni-vaccini-latest.csv"

# Possible values: ABR, BAS, CAL, CAM, EMR, FVG, LAZ, LIG, LOM, MAR,
# MOL, PAT, PIE, PUG, SAR, SIC, TOS, UMB, VDA, VEN.
REGION="LAZ"

# Taken from google, 18/04/2021
# -- note also: http://demo.istat.it/bilmens/index.php?anno=2021&lingua=ita
LAZIO_POPULATION=5719324

# -- download file
curl --silent $REMOTE_URL -o /tmp/vaccini.txt

# -- compute percentage
LAZIO_FIRST_VACCINE=$(grep $REGION $LOCAL_FILE_NAME | cut -d ',' -f 7 | paste -sd+ | bc)
PERCENTAGE_FIRST=$(echo "scale=2; $LAZIO_FIRST_VACCINE / $LAZIO_POPULATION" | bc)
INTEGER_FIRST="${PERCENTAGE_FIRST:1}"

LAZIO_SECOND_VACCINE=$(grep $REGION $LOCAL_FILE_NAME | cut -d ',' -f 8 | paste -sd+ | bc)
PERCENTAGE_SECOND=$(echo "scale=2; $LAZIO_SECOND_VACCINE / $LAZIO_POPULATION" | bc)
INTEGER_SECOND="${PERCENTAGE_SECOND:1}"

LAZIO_THIRD_VACCINE=$(grep $REGION $LOCAL_FILE_NAME | cut -d ',' -f 10 | paste -sd+ | bc)
PERCENTAGE_THIRD=$(echo "scale=2; $LAZIO_THIRD_VACCINE / $LAZIO_POPULATION" | bc)
INTEGER_THIRD="${PERCENTAGE_THIRD:1}"

# -- output
echo "$INTEGER_FIRST/$INTEGER_SECOND/$INTEGER_THIRD"
