#!/bin/bash

make
if [[ $? -ne 0 ]]
then
  exit 1
fi

PROG=$1
shift

echo "./$PROG $*"
echo ""
./$PROG $*
