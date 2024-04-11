# InsightDebugger

** Experimental WIP**

# The Plan

## Objective

InsightDebugger is a windows local and remote based BinaryNinja debugger.<br>

It blurs the lines between static and dynamic analysis by introducing a new paradigm where the static analysis takes precedence over the dynamic one.

The main objective is to enhance the work flow of large scale static analysis projects, where the executable interacts with many .dll files and memory blobs, but the static analysis is the ground truth where you spend all your time creating a marked up database.<br>

it should provide a way to have all your .dll and memory blobs in seperate binary views and and be able to launch the debugger fast to have the debugger jump between each binary view masking base address changes inorder to provide dynamic analysis information for you to update your static analysis and therefore keeping a static reproducible overview of your RE project.<br>

The debugger backend should be based on x64 debug's TitanEngine and built into a network remote service application that has compatibility with windows XP and possibly 9.x. A linux backend debugger might be considered in the future.<br>

This is a personal project with a niche workflow and open sourced incase others find it interesting or want to reference some additional plugins that use binary ninja's c++ ui api.

## Wanted Features

-   static precedence debugging
-   network based
-   memory based type viewer, view your structs and local variables easily
-   code/data snapshots for hot swapping in binary views
-   eventual support for binary ninja "projects" with features
    such as promoting memory blobs and/or loaded dlls into the project
-   make use of the upcoming new binary ninja memory map features,
    to insert BVs and other memory segments as part of the database project.
-   consider expanding to handle packed files using the above features to generate
    a good base static overview that is in the database only and not changed on disk.
