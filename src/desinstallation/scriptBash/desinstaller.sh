#!/bin/bash

apt remove $1

apt purge $1 

apt clean 

apt autoremove

apt -f install

