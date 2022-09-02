#include <gdal.h>
#include <gdal_priv.h>



#ifndef PHP_COUNTRYGEOCODE_H
#define PHP_COUNTRYGEOCODE_H

extern zend_module_entry countrygeocode_module_entry;
#define phpext_countrygeocode_ptr &countrygeocode_module_entry

#ifdef PHP_WIN32
#	define PHP_COUNTRYGEOCODE_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_COUNTRYGEOCODE_API __attribute__ ((visibility("default")))
#else
#	define PHP_COUNTRYGEOCODE_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

PHP_MINIT_FUNCTION(countrygeocode);
PHP_MSHUTDOWN_FUNCTION(countrygeocode);
PHP_RINIT_FUNCTION(countrygeocode);
PHP_RSHUTDOWN_FUNCTION(countrygeocode);
PHP_MINFO_FUNCTION(countrygeocode);

PHP_FUNCTION(country_geocode);

ZEND_BEGIN_MODULE_GLOBALS(countrygeocode)
	char * borders_file_name;

	GDALDataset * dataset;
ZEND_END_MODULE_GLOBALS(countrygeocode)

/* In every utility function you add that needs to use variables 
   in php_countrygeocode_globals, call TSRMLS_FETCH(); after declaring other 
   variables used by that function, or better yet, pass in TSRMLS_CC
   after the last function argument and declare your utility function
   with TSRMLS_DC after the last declared argument.  Always refer to
   the globals in your function as COUNTRYGEOCODE_G(variable).  You are 
   encouraged to rename these macros something shorter, see
   examples in any other php module directory.
*/

#ifdef ZTS
#define COUNTRYGEOCODE_G(v) TSRMG(countrygeocode_globals_id, zend_countrygeocode_globals *, v)
#else
#define COUNTRYGEOCODE_G(v) (countrygeocode_globals.v)
#endif

#endif	/* PHP_COUNTRYGEOCODE_H */

