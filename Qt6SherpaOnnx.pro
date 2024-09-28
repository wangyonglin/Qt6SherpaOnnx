TEMPLATE = subdirs

 SUBDIRS = \
    Examples \
    libQt6SherpaOnnx

 # where to find the sub projects - give the folders
 libQt6SherpaOnnx.subdir = libQt6SherpaOnnx
 Examples.subdir  = examples/test

 # what subproject depends on others
 Examples.depends = libQt6SherpaOnnx
