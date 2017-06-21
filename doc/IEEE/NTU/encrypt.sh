#!/bin/bash

IFS= read -s  -p "Please Enter Password of the Change Permissions: " pwd

pdftk thesis.pdf output thesis-security.pdf \
	owner_pw $(pwd)  \
	allow printing ScreenReaders \
	encrypt_128bit
