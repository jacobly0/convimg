# convimg [![Windows/Linux/MacOS](https://github.com/mateoconlechuga/convimg/actions/workflows/make.yml/badge.svg)](https://github.com/mateoconlechuga/convimg/actions/workflows/make.yml) [![Coverity Scan](https://scan.coverity.com/projects/23542/badge.svg)](https://scan.coverity.com/projects/mateoconlechuga-convimg) [![Language Grade](https://img.shields.io/lgtm/grade/cpp/g/mateoconlechuga/convimg.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/mateoconlechuga/convimg/context:cpp)

This program is used to convert images to other formats, specifically for the TI84+CE and related calculators.

## Command Line Help

    Usage:
        convimg [options] -i <yaml file>

    Required options:
        -i, --input <yaml file>  Input file, format is described below.

    Optional options:
        --icon <file>            Create an icon for use by shell.
        --icon-description <txt> Specify icon/program description.
        --icon-format <fmt>      Specify icon format, 'ice' or 'asm'.
        --icon-output <output>   Specify icon output filename.
        -n, --new                Create a new template YAML file.
        -h, --help               Show this screen.
        -v, --version            Show program version.
        -l, --log-level <level>  Set program logging level.
                                 0=none, 1=error, 2=warning, 3=normal

    YAML File Format:

        A YAML file is a human-readable text file that is used by this program
        to create palettes, convert images, and output the results to a supported
        format. A palette is a list of colors that an image or group of images uses.
        This saves memory by having a fixed number of colors rather than storing
        a color for every pixel in the image.

        The YAML file consists of three key components: palette generation, image
        conversion, and output formatting. These components are broken down into
        different sections of a YAML file, shown below:

            palettes:
              - name: mypalette           : Palette and name of palette
                images: automatic         : Automatically get list of images

            converts:
              - name: myimages            : Convert images section
                palette: mypalette        : Use the above generated palette
                images:                   : List of input images
                  - image1.png
                  - image2.png
                  - image3.png

            outputs:                      : List of outputs
              - type: c                   : Sets the output format to C
                include-file: gfx.h       : File name containing all images/palettes
                palettes:                 : Start of palettes list in the output
                  - mypalette             : Name of each palette, separated by line
                converts:                 : Converted sections, based on name
                  - myimages              : Name of each convert section

    To create the above template, use the '--new' command line option.
    The following describes each section of the YAML file in more detail, and
    the available options for conversion.

    ----------------------------------------------------------------------------

    palettes:
        The YAML palettes section constists of a list of different palettes to be
        generated. List entires start with the name of the palette to create.

         - name: <palette name>       : Name of palette. This is used by the
                                      : convert blocks for detecting which
                                      : palette to use, as well as the name
                                      : used in the output code files.
                                      : Required parameter.

           max-entries: <num>         : Number of maximum colors available
                                      : in the palette. Default is 256,
                                      : range is 1-256.

           fixed-entries:             : Adds fixed color entries to the
                                      : palette. The format is:
                                      :
                                      :  fixed-entries:
                                      :    - color: {index: 0, r: 9, g: 10, b: 0}
                                      :    - color: {index: 1, r: 2, g: 83, b: 5}
                                      :
                                      : Note the spaces between key/value pairs.
                                      : 'index' represents the palette entry
                                      : used for the color, r is the red amount,
                                      : g is the green amound, and b is the blue
                                      : amount of the color. This is often
                                      : called the "rgb" value of a color.
                                      : 'exact' is used to match colors exactly
                                      : to the input color before quantization.
                                      : It is an optional parameter, defaults to
                                      : 'false' but can be 'true'.
                                      : Fixed entries can also be supplied via
                                      : an image, 1 pixel in height (and a
                                      : width <= 256) where each pixel in the
                                      : image represents an entry:
                                      :
                                      :   fixed-entries:
                                      :     - image: palette.png
                                      :
                                      : There can be up to 256 fixed entries.
                                      : With the image method, the starting
                                      : index is 0 and increments by 1 moving
                                      : from left to right.

           speed: <speed>             : Speed controls (somewhat, not really)
                                      : the quality of the palette generated.
                                      : Value range is 1-10, where 1 is best
                                      : and 10 is worst. Default is 4.

           images: <option>           : A list of images separated by a newline
                                      : and indented with a leading '-'
                                      : character. These images are quantized
                                      : in order to produce a palette that
                                      : best matches with these images.
                                      : <option> can be 'automatic' to
                                      : automatically get the images from the
                                      : convert blocks that are using them,
                                      : without having to specify the images
                                      : twice. Note that common "globbing"
                                      : patterns can be used, such as '*'.

    ----------------------------------------------------------------------------

    converts:
        The YAML converts section constists of a list of different "groups" of
        images to be converted that share similar properties, such as the palette
        used, or the compression mode. The available options are:

         - name: <convert name>       : Name of convert. This is used by the
                                      : outputs for detecting which converted
                                      : images to output.
                                      : Required parameter.

           palette: <palette name>    : Name of palette used to quantize images
                                      : against. This is the name of a palette
                                      : block. Built-in palettes also exist,
                                      : namely 'xlibc' and 'rgb332'.

           palette-offset: <index>    : Converts assuming a palette has
                                      : a different base index.
                                      : Defaults to '0'.

           images:                    : A list of images separated by a newline
                                      : and indented with a leading '-'
                                      : character. These images are quantized
                                      : against the palette.
                                      : Note that common "globbing"
                                      : patterns can be used, such as '*'.

           tilesets:                  : A group of tilesets. There can be only one
                                      : tileset group per conversion block.
                                      : Tilesets are special images consisting of
                                      : multiple "tiles" of fixed width and
                                      : height. The format for this option is:
                                      :
                                      : tilesets:
                                      :   tile-width: 16
                                      :   tile-height: 16
                                      :   images:
                                      :     - tileset1.png
                                      :     - tileset2.png
                                      :
                                      : Above, each tileset in the list is
                                      : composed of tiles of a width and height
                                      : of 16 pixels. Tile numbers are determined
                                      : starting from the top left, moving right
                                      : to the bottom-right.
                                      : Another optional boolean field is
                                      : 'pointer-table', which will output
                                      : pointers to each tile.
                                      : The default is 'true'.

           transparent-color-index:   : Transparent color index in the palette
                                      : (probably determined using fixed-color),
                                      : that represents a transparent color.
                                      : Note that the color itself is not
                                      : transparent, but rather a placeholder
                                      : in the palette.
                                      : The format of this option is:
                                      : transparent-color-index: <index>
                                      : where <index> is the index/entry in the
                                      : palette.

           style: <mode>              : Style controls the converted format of
                                      : images. In 'normal' mode, converted
                                      : images map 1-1 where each pixel byte
                                      : represents a palette index.
                                      : In 'rlet' mode, the transparent color
                                      : index is used to compress runs of
                                      : transparent colors, essentially reducing
                                      : the output size if there are many
                                      : transparent pixels.

           compress: zx7              : After quantization, images can then
                                      : optionally be compressed in zx7 format.
                                      : The images will then be required to be
                                      : decompressed before use.

           width-and-height: <bool>   : Optionally control if the width and
                                      : height should be placed in the converted
                                      : image; the first two bytes respectively.

           flip-x: <bool>             : Flip input images vertically across the
                                      : x-axis

           flip-y: <bool>             : Flip input images horizontally across the
                                      : y-axis

           rotate: <degrees>          : Rotate input images 0,90,180,270 degrees

           bpp: <bits-per-pixel>      : Control how many bits per pixel are used
                                      : This also affects the size of the
                                      : generated palette; use with caution.
                                      : Available options are 1,2,4,8.
                                      : The default is 8.

           omit-indices: [<list>]     : Omits the specified palette indices
                                      : from the converted image. May be useful
                                      : by a custom drawing routine. A comma
                                      : separated list in [] should be provided.

    ----------------------------------------------------------------------------

    outputs:
        The YAML file outputs section is a list of different "groups" of outputs,
        differing in types or images. The currently available formats are:

         - type: c                    : C source and header files
         - type: asm                  : Assembly and include files
         - type: bin                  : Binary and listing files
         - type: ice                  : Single ICE compiler formatted file
         - type: appvar               : TI archived AppVar format

        The different options available for outputs are:

           include-file: <file>       : Output file containing all the graphics
                                      : Defaults to "gfx.h", "gfx.inc",
                                      : "bin.txt", and "ice.txt" for C, Asm,
                                      : Binary, and ICE formats respectively.

           palettes:                  : A list of generated palettes, separated
                                      : by a newline and indented with the '-'
                                      : character.

           converts:                  : A list of convert sections, separated
                                      : by a newline and indented with the '-'
                                      : character.

           directory: <directory>     : Supply the name of the output directory.
                                      : Defaults to the current directory.

           prepend-palette-sizes:     : Prepend the palette with a 2-byte
               <bool>                 : entry containing the total size in
                                      : bytes of the palette.
                                      : Default is 'false'.
                                      : Optional parameter.

       AppVars are a special type of output and require a few more options.
       The below options are only available for AppVars, however the above
       options can also be used.

           name: <appvar name>        : Name of AppVar, maximum 8 characters.
                                      : Required parameter.

           source-format: <format>    : Source files to create to access
                                      : image and palette data. format can be
                                      : 'c', 'ice', 'asm', or 'none'.
                                      : Default is 'none'.
                                      : Optional parameter.

           source-init: <bool>        : Whether to output AppVar initialization
                                      : code for loading and setting up image
                                      : and palette pointers. Default is 'true'
                                      : Only compatible with source-format: 'c'.
                                      : Optional parameter.

           lut-entries: <bool>        : Embeddeds a lookup table (LUT) for
                                      : determining the image offsets in the
                                      : appvar. Format consists of an entry
                                      : containing the table size, followed by
                                      : the LUT entries. Entry sizes are
                                      : controlled via the parameter
                                      : 'lut-entry-size'. Default is 'false'.
                                      : Optional parameter.

           lut-entry-size: <int>      : Controls the size of LUT entries. Can be
                                      : '2' for 2-byte or '3' for 3-byte
                                      : entries. Default is '3'.
                                      : Optional parameter.

           compress: <mode>           : Compress AppVar data.
                                      : Current 'mode' parameters are 'zx7'.
                                      : The AppVar then needs to be decompressed
                                      : to access image and palette data.
                                      : Optional parameter.

           header-string: <string>    : Prepends <string> to the start of the
                                      : AppVar's data.
                                      : Use double quotes to properly interpret
                                      : escape sequences.

           archived: <bool>           : 'true' makes the AppVar archived, while
                                      : 'false' leaves it unarchived.
                                      : Optional parameter.

    ----------------------------------------------------------------------------

    Credits:
        (c) 2017-2022 by Matt "MateoConLechuga" Waltz.

        This program utilizes the following libraries:
            libimagequant: (c) 2009-2022 by Kornel Lesiński.
            libyaml: (c) 2006-2022 by Ingy döt Net & Kirill Simonov.
            stb: (c) 2017 by Sean Barrett.
            zx7: (c) 2012-2013 by Einar Saukas.


