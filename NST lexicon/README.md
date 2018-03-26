# General information
Created by _miastra_ in March 2018  
Belongs to the respository: https://github.com/miastra/SwedishForcedAligner

# Scripts

## convert_NST_lexicon.py

This script converts the NST lexicon given in the file _swe030224NST.pron_ found at https://www.nb.no/sprakbanken/show?serial=oai%3Anb.no%3Asbr-22&lang=en (retrieved 2018-03-26) to a text file called _NST_lexicon.txt_ containing only orthography and one phonemic transcription in the format needed by the Montréal Forced Aligner. It will create the text file if it does not exist and otherwise overwrite it. The script must be run from the folder where _swe030224NST.pron_ is situated.

After running the script, search and replace needs to be performed in the text file to manually capitalize the letters å, ä, and ö.
