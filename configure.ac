
m4_define([angela-typing-tutor], [aott])
m4_define([aott_released], [0])
m4_define([ibus_major_version], [1])
m4_define([ibus_minor_version], [2])
m4_define([ibus_micro_version], [99])

AC_INIT(angela-typing-tutor, version-0.1)
AM_INIT_AUTOMAKE([1.10])

AM_PROG_CC_C_O
AC_PROG_CC
AC_GNU_SOURCE
AM_SANITY_CHECK


AC_CHECK_LIB([espeak],[espeak_Cancel],,[AC_MSG_ERROR([Espeak development Library not found!])])

PKG_CHECK_MODULES([GTK], [ gtk+-3.0 ])
PKG_CHECK_MODULES([CANBERRA], [ libcanberra-gtk3 ])


AC_CONFIG_FILES(Makefile
data/Makefile
data/data/Makefile
data/faces/Makefile
data/hands/Makefile
data/number/Makefile
data/sounds/Makefile
data/ui/Makefile
src/Makefile
)

AC_OUTPUT
