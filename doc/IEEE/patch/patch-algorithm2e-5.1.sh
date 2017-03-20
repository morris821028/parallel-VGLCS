#!/bin/bash

set -e
sudo mv /usr/share/texlive/texmf-dist/tex/latex/algorithm2e/algorithm2e.sty /usr/share/texlive/texmf-dist/tex/latex/algorithm2e/algorithm2e.sty-bak
set +e
sudo cp ./algorithm2e.sty /usr/share/texlive/texmf-dist/tex/latex/algorithm2e/

