CountryGeocode
==============

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

Brought to you by Dimitar Kunchev with the simple purpose of having a function that returns country ISO2 code for given world coordinates. Without having to call external APIs. 

The extension is built around `libgdal` (which you need to install) and requires a borders shape file configured. The repository contains one, downloaded from http://thematicmapping.org/downloads/world_borders.php

## Requirements
- PHP
- libgdal
- gdal-dev

## Build/install steps

1. Make sure you have `libgdal` and `libgdal-dev` installed
2. `phpize`
3. `./configure`
4. `make`
5. `make install`
6. add to your `php.ini` file the extension like this:
	`extension=countrygeocode.so`
7. Configure path to a borders file in your php.init: `countrygeocode.borders=path-to-the-shp-file`. Make sure it is accompanied with its respective `shx` and etc files. This repository contains all the needed files in a ZIP archive you can extract to a location of your choice

## Usage

Call `country_geocode(float latitude, float longitude)` - return value is NULL on error, empty string if coordinates do not match any country or 2-letter ISO code on success

## Example

```php
country_geocode(-5.937671, -68.323475);
\\ returns "BR" for Brazil

country_geocode(42.804594, 24.795333);
\\ returns "BG" for Bulgaria

```

## Copyright

- The provided world borders data set comes from http://thematicmapping.org/downloads/world_borders.php under the Creative Commons Attribution-Share Alike License.
- The source code is inspired by https://github.com/che0/countries
- Implementation as PHP extension is by Dimitar Kunchev

## Author

Dimitar Kunchev

## License

This extension is licensed under the MIT License - see the `LICENSE` file for details

## Contributing

Pull requests and issues are more than welcome.