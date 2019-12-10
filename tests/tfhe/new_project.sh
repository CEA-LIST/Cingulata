#!/bin/bash

# Script for creating a new tfhe project. The new project is the same as _template project.

SCRIPT_DIR=$0

if [ "$#" -lt 1 ]; then
  echo "Usage: $SCRIPT_DIR <project name>"
  exit
fi

NAME=$1

# goto script directory, as next commands are relative to it
cd `dirname $SCRIPT_DIR`

mkdir $NAME || exit

cp -rn _template/* $NAME/

# rename variables
sed -i "s/@NAME@/$NAME/g" $NAME/CMakeLists.txt
sed -i "s/@NAME@/$NAME/g" $NAME/run.sh.in

touch CMakeLists.txt #do it so that cmake rebuilts
echo "Project $NAME created"