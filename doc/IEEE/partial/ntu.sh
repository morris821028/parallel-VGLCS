#!/bin/bash

#############
#
# ./ntu.sh 
#
#############

set -x

rm -rf ./ntu
mkdir -p ./ntu

PATTERN_RHS=("section" "subsection" "subsubsection" "subsubsubsection")
PATTERN_LHS=("chapter"    "section"    "subsection"    "subsubsection")

for TEXFILE in *.tex;
do
	FILE_PREFIX=${TEXFILE%%.tex}
	echo "Process ${TEXFILE} to save as ./ntu/${FILE_PREFIX}-ntu.tex"
	cp ${TEXFILE} ./ntu/${FILE_PREFIX}-ntu.tex
	for pattern_idx in "${!PATTERN_RHS[@]}"; 
	do
		printf "%s %s\n" "${PATTERN_RHS[$pattern_idx]}" "${PATTERN_LHS[$pattern_idx]}"
		sed -i "s/\\\\${PATTERN_RHS[$pattern_idx]}{\([^}]*\)}/\\\\${PATTERN_LHS[$pattern_idx]}{\1}/g" ./ntu/${FILE_PREFIX}-ntu.tex
	done
	sed -i 's/\\begin{abstract}/\\begin{abstractEN}/g' ./ntu/${FILE_PREFIX}-ntu.tex
	sed -i 's/\\end{abstract}/\\end{abstractEN}/g' ./ntu/${FILE_PREFIX}-ntu.tex
	sed -i '1i %% AUTO-GENERATED FILES %%' ./ntu/${FILE_PREFIX}-ntu.tex
#	sed -e 's/\\section{\([^}]*\)}/\\chapter{\1}/g' ${TEXFILE}  >./ntu/${FILE_PREFIX}-ntu.tex
done

