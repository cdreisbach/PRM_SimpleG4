#!/bin/bash

[[ $_ != $0 ]] && REALPATH=`dirname $(readlink -f ${BASH_SOURCE[0]})` || REALPATH=`dirname $(readlink -f $0)`

if [[ ! -z "$1" || ! -d "$REALPATH/build" || -z "$(ls -A $REALPATH/build)" ]]; then
  mkdir -p $REALPATH/build
  cd $REALPATH/build
  cmake3 $REALPATH
  [ $? -eq 1 ] && exit 1
fi

cd $REALPATH/build

make -j$(echo "$(nproc --all)/2+1" | bc)
[ $? -eq 1 ] && exit 1
cd - > /dev/null 2> /dev/null
