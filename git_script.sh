#!/bin/bash
echo Subiendo
git add -A 
git commit -m $#
git push