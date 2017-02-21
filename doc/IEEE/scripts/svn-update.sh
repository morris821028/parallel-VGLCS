#!/bin/bash

pushd `dirname $0` > /dev/null
SCRIPTPATH=`pwd -P`
popd > /dev/null

echo "${SCRIPTPATH}"
cd ${SCRIPTPATH}/parallel-VGLCS && svn update
cd ${SCRIPTPATH}/svnroot && svn update

