#!/bin/bash

sudo apt-get install xzdec
sudo tlmgr init-usertree
sudo tlmgr option repository ftp://tug.org/historic/systems/texlive/2015/tlnet-final 
sudo tlmgr update --all
