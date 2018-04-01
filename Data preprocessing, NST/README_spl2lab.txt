README_spl2lab.c

In the project, we trained a Swedish forced aligner with data from the NST data base (you can find it here: https://www.nb.no/sprakbanken/show?serial=oai%3Anb.no%3Asbr-16&lang=en). We used NST Acoustic database for Swedish (16 kHz) part 1 (of 3).
There are 312 utterances recorded with each speaker in the data base. For every speaker, there is an spl-file with meta data and the script for the recordings. You can see an (anonymized) example of an spl-file, named r4670001.spl.
The program spl2lab.c takes an spl-file and creates 312 lab files from it (one for every recording). The lab files are given names which relate to the corresponding wav files, i.e. if the name of the wav file is u0001001.wav, the name of the lab file will be u0001001.lab.
To execute the program, type spl2lab <source_file>. If you have many spl files (we had more than 300), place them in one directory and type for %a in (*.spl) do spl2lab "%~a"
Note: The program only works with ANSI files (and NSTs spl files are in ANSI format), but MFA only takes UTF8 so the lab files need to be converted to UTF8 before used for training the MFA.