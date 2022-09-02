dnl $Id$
dnl config.m4 for extension countrygeocode

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.


dnl If your extension references something external, use with:

PHP_ARG_WITH(countrygeocode, for countrygeocode support,
dnl Make sure that the comment is aligned:
[  --with-countrygeocode             Include countrygeocode support])

dnl Otherwise use enable:

dnl PHP_ARG_ENABLE(countrygeocode, whether to enable countrygeocode support,
dnl Make sure that the comment is aligned:
dnl [  --enable-countrygeocode           Enable countrygeocode support])

if test "$PHP_COUNTRYGEOCODE" != "no"; then

LIBNAME=gdal # you may want to change this
LIBSYMBOL=GDALAllRegister # you most likely want to change this 

PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
[
	PHP_REQUIRE_CXX()
	PHP_ADD_LIBRARY_WITH_PATH($LIBNAME,,EXTRA_LDFLAGS)
	dnl PHP_ADD_LIBRARY($LIBNAME,1,EXTNAME_SHARED_LIBADD)
	
	dnl PHP_SUBST(COUNTRYGEOCODE_SHARED_LIBADD)
  	
	PHP_EVAL_LIBLINE(`pkg-config --libs gdal`)
	PHP_EVAL_INCLINE(`pkg-config --cflags gdal`)
	
	PHP_NEW_EXTENSION(countrygeocode, countrygeocode.cpp, $ext_shared)
  	AC_DEFINE(HAVE_GDALLIB,1,[ ])
],[
	AC_MSG_ERROR([wrong gdal lib version or lib not found])
])

PHP_SUBST(GDAL_SHARED_LIBADD)


  dnl Write more examples of tests here...
    dnl # --with-countrygeocode -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/countrygeocode.h"  # you most likely want to change this
  dnl if test -r $PHP_COUNTRYGEOCODE/$SEARCH_FOR; then # path given as parameter
  dnl   COUNTRYGEOCODE_DIR=$PHP_COUNTRYGEOCODE
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for countrygeocode files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       COUNTRYGEOCODE_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$COUNTRYGEOCODE_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the countrygeocode distribution])
  dnl fi

  dnl # --with-countrygeocode -> check for lib and symbol presence
  
  
  
fi

