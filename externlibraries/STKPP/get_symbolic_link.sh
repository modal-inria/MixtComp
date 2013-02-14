#!/bin/bash

function make_symbolic_link_for_projects
{
# loop over th files
for file in $1/*
do 
link=./projects/$2/$(basename $1)/$(basename $file)
ln -s ../../../$file $link
done
}

function make_symbolic_link_for_include
{
# loop over header files
for file in $1/*
do 
link=./include/$(basename $file)
ln -s ../$file $link
done
}
#create symbolic link of all the projects
projects=( $(find ../../../stkpp/projects -maxdepth 1 -type d -printf '%P\n') )
for project in ${projects[@]}
do
make_symbolic_link_for_projects ../../../stkpp/projects/$project/include $project
make_symbolic_link_for_projects ../../../stkpp/projects/$project/src $project
done

#create symbolic link for headers
make_symbolic_link_for_include ../../../stkpp/include