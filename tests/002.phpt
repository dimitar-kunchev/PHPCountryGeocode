--TEST--
Test coutry geocoding
--INI--
countrygeocode.borders=tmp/TM_WORLD_BORDERS-0.3.shp
--SKIPIF--
<?php  if (!extension_loaded("countrygeocode")) print "skip"; ?>
--FILE--
<?php 
system("unzip -qq -n TM_WORLD_BORDERS-0.3.zip -d tmp");
var_dump(country_geocode(42.6728295, 23.2781873));
var_dump(country_geocode(-5.937671, -68.323475));
var_dump(country_geocode(12313213, 12313123));
system("rm -rf tmp");
?>
--EXPECT--
string(2) "BG"
string(2) "BR"
string(0) ""