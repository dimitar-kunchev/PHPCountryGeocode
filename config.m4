dnl $Id$
dnl config.m4 for extension countrygeocode

PHP_ARG_WITH(countrygeocode, for countrygeocode support,
[  --with-countrygeocode             Include countrygeocode support])

PHP_ARG_WITH(gdal-dev, custom gdal.h path,
[ --with-gdal-dev					 Specify custom path to include/gdal.h], , false)

if test "$PHP_COUNTRYGEOCODE" != "no"; then
	dnl Check if gdal is installed

	LIBNAME=gdal
	LIBSYMBOL=GDALAllRegister 

	PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
	[
		PHP_REQUIRE_CXX()
		PHP_ADD_LIBRARY_WITH_PATH($LIBNAME,,EXTRA_LDFLAGS)
		PHP_SUBST(GDAL_SHARED_LIBADD)
	
		INCLINE=`pkg-config --cflags gdal`
		if test "$INCLINE" == ""; then
			AC_MSG_ERROR([gdal-dev probably missing])
		else
			PHP_EVAL_INCLINE($INCLINE)
	
			PHP_NEW_EXTENSION(countrygeocode, countrygeocode.cpp, $ext_shared)
  			AC_DEFINE(HAVE_GDALLIB,1,[ ])
  		fi
	],[
		AC_MSG_ERROR([wrong gdal lib version or lib not found])
	])
fi

