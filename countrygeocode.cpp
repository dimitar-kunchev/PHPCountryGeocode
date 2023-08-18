
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_countrygeocode.h"

/* If you declare any globals in php_countrygeocode.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(countrygeocode)
*/

ZEND_DECLARE_MODULE_GLOBALS(countrygeocode)

static PHP_INI_MH(OnUpdateBordersFile);
static PHP_GINIT_FUNCTION(countrygeocode);

PHP_INI_BEGIN()
	//STD_PHP_INI_ENTRY("countrygeocode.borders", "", PHP_INI_ALL, OnUpdateBordersFile, borders_file_name, zend_countrygeocode_globals, countrygeocode_globals)
	PHP_INI_ENTRY("countrygeocode.borders", "", PHP_INI_ALL, OnUpdateBordersFile)
PHP_INI_END()

/* True global resources - no need for thread safety here */
static int le_countrygeocode;

/* {{{ arginfo
 */
ZEND_BEGIN_ARG_INFO_EX(arginfo_countrygeocode, 0, 0, 2)
	ZEND_ARG_INFO(0, lat)
	ZEND_ARG_INFO(0, lng)
ZEND_END_ARG_INFO()

/* }}} */


/* {{{ countrygeocode_functions[]
 *
 * Every user visible function must have an entry in countrygeocode_functions[].
 */
const zend_function_entry countrygeocode_functions[] = {
	PHP_FE(country_geocode, arginfo_countrygeocode)
	PHP_FE_END	/* Must be the last line in countrygeocode_functions[] */
};
/* }}} */

/* {{{ countrygeocode_module_entry
 */
zend_module_entry countrygeocode_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"countrygeocode",
	countrygeocode_functions,
	PHP_MINIT(countrygeocode),
	PHP_MSHUTDOWN(countrygeocode),
	PHP_RINIT(countrygeocode),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(countrygeocode),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(countrygeocode),
#if ZEND_MODULE_API_NO >= 20010901
	"0.3", /* Replace with version number for your extension */
#endif
	PHP_MODULE_GLOBALS(countrygeocode),
	PHP_GINIT(countrygeocode),
	NULL, //PHP_GSHUTDOWN(countrygeocode),
	NULL,
	STANDARD_MODULE_PROPERTIES_EX
};
/* }}} */

#ifdef COMPILE_DL_COUNTRYGEOCODE
ZEND_GET_MODULE(countrygeocode)
#endif


/* {{{ php_countrygeocode_init_globals
 */
/* Uncomment this function if you have INI entries */
//static void php_countrygeocode_init_globals(zend_countrygeocode_globals *countrygeocode_globals)
PHP_GINIT_FUNCTION(countrygeocode)
{
	countrygeocode_globals->borders_file_name = NULL;
}

/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(countrygeocode)
{
	/* If you have INI entries, uncomment these lines 
	*/
	REGISTER_INI_ENTRIES();
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(countrygeocode)
{
	/* uncomment this line if you have INI entries
	*/
	UNREGISTER_INI_ENTRIES();
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(countrygeocode)
{
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(countrygeocode)
{
	/*GDALDataset * dataset = COUNTRYGEOCODE_G(dataset);
	if (dataset != NULL) {
		GDALClose(dataset);
	}*/
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(countrygeocode)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "countrygeocode support", "enabled");
	php_info_print_table_end();

	DISPLAY_INI_ENTRIES();
}
/* }}} */

/* {{{ proto string country_geocode(double lat, double lng)
   Return a string with the ISO code of the country where the coordinates lie */
PHP_FUNCTION(country_geocode)
{
	GDALDataset * dataset = NULL;
	/*GDALDataset * dataset = COUNTRYGEOCODE_G(dataset);
	if (dataset == NULL) {*/
		// Attempt to load the dataset
		GDALAllRegister();
		char * bfn = COUNTRYGEOCODE_G(borders_file_name);
		if (bfn != NULL) {
			dataset = (GDALDataset *)GDALOpenEx(bfn, GDAL_OF_RASTER | GDAL_OF_VECTOR | GDAL_OF_READONLY | GDAL_OF_SHARED, NULL, NULL, NULL);
			if (dataset == NULL) {
				php_error_docref(NULL, E_WARNING, "Could not open borders file %s", bfn);
				RETURN_NULL();
			}
			//COUNTRYGEOCODE_G(dataset) = dataset;
		}
	//}
	if (dataset == NULL) {
		php_error_docref(NULL, E_ERROR, "No borders file configured or could not be open");
		RETURN_NULL();
	}


	double lat, lng;

	ZEND_PARSE_PARAMETERS_START(2,2);
	Z_PARAM_DOUBLE(lat);
	Z_PARAM_DOUBLE(lng);
	ZEND_PARSE_PARAMETERS_END();

	int layersCount = GDALDatasetGetLayerCount(dataset);
	if (layersCount == 0) {
		//fprintf( stderr, "No layers in dataset\n");
		php_error_docref(NULL, E_ERROR, "Invalid borders file - no layers found");
		RETURN_NULL();
	}

	OGRLayer * layer = (OGRLayer *)GDALDatasetGetLayer(dataset, 0);
	if (layer == NULL) {
		php_error_docref(NULL, E_ERROR, "Invalid borders file - could not get layer");
		RETURN_NULL();
	}

	GIntBig featureCount = OGR_L_GetFeatureCount(layer, 1);

	OGRGeometry * searchPoint = (OGRGeometry *)OGR_G_CreateGeometry(wkbPoint);
	OGR_G_AddPoint_2D(searchPoint, lng, lat);

	char returnISOCode[3];
	memset(returnISOCode, 0, 3);

	for (int i = 0; i < featureCount; i ++) {
		OGRFeature * feature = (OGRFeature *)OGR_L_GetFeature(layer, i);
		if (feature == NULL) {
			php_error_docref(NULL, E_ERROR, "Error getting layer feature %d\n", i);
			RETURN_NULL();
		}

		OGRGeometry * geometry = (OGRGeometry *)OGR_F_GetGeometryRef(feature);
		if (geometry == NULL) {
			php_error_docref(NULL, E_ERROR, "Error getting layer feature geometry %d\n", i);
			RETURN_NULL();
		}

		if (OGR_G_Contains(geometry, searchPoint)) {
			int fieldIndex = OGR_F_GetFieldIndex(feature, "ISO2");
			if (fieldIndex < 0) {
				php_error_docref(NULL, E_ERROR, "Error getting ISO2 code for matched country");
				RETURN_NULL();
			}
			const char * tmp = OGR_F_GetFieldAsString(feature, fieldIndex);
			strcpy(returnISOCode, tmp);


			OGR_F_Destroy(feature);

			break;
		}

		OGR_F_Destroy(feature);
	}

	OGR_G_DestroyGeometry(searchPoint);

	GDALClose(dataset);
	RETURN_STRING(returnISOCode);

}
/* }}} */

static PHP_INI_MH(OnUpdateBordersFile)
{
	COUNTRYGEOCODE_G(borders_file_name) = ZSTR_VAL(new_value);

	return SUCCESS;
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
