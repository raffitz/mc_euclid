#!/usr/bin/env bash

ORIGIN_X=$1
ORIGIN_Y=$2
ORIGIN_Z=$3

FIRST_X=$4
FIRST_Y=$5
FIRST_Z=$6

SECOND_X=$7
SECOND_Y=$8
SECOND_Z=$9

FIRST_V_X="(${FIRST_X})-(${ORIGIN_X})"
FIRST_V_Y="(${FIRST_Y})-(${ORIGIN_Y})"
FIRST_V_Z="(${FIRST_Z})-(${ORIGIN_Z})"

SECOND_V_X="(${SECOND_X})-(${ORIGIN_X})"
SECOND_V_Y="(${SECOND_Y})-(${ORIGIN_Y})"
SECOND_V_Z="(${SECOND_Z})-(${ORIGIN_Z})"

# Cross Product Cheatsheet
#
# a ⨯ b = | i   j   k   |
#         | ax  ay  az  |
#         | bx  by  bz  |
#
#       = (ay bz - az by, az bx - ax bz, ax by - ay bx)

NORMAL_X="((${FIRST_V_Y})*(${SECOND_V_Z})-(${FIRST_V_Z})*(${SECOND_V_Y}))"
NORMAL_Y="((${FIRST_V_Z})*(${SECOND_V_X})-(${FIRST_V_X})*(${SECOND_V_Z}))"
NORMAL_Z="((${FIRST_V_X})*(${SECOND_V_Y})-(${FIRST_V_Y})*(${SECOND_V_X}))"

echo "(x - (${ORIGIN_X}))*(${NORMAL_X}) + (y - (${ORIGIN_Y}))*(${NORMAL_Y}) + (z - (${ORIGIN_Z}))*(${NORMAL_Z}) <= 0"
