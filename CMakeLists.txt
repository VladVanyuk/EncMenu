# Make Menu Navigation System Library
# https://github.com/VladVanyuk/MakeMenuLib
# MIT License

cmake_minimum_required(VERSION 3.5)

idf_component_register(SRCS "MakeMenu_priv.cpp." "MakeMenu.cpp" 
                       INCLUDE_DIRS "."
                       REQUIRES arduino)

project(MakeMenu)