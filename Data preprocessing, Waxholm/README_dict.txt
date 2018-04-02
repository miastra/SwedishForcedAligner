README_dict.c

In the project, we had to build a dictionary for the Waxholm data base. To do this, we created text files at the same time we created lab files (see README_mix2lab).
The program dict.c opens two instances of the same text file, and takes pairs of orthographic text and phonetic text from them, translating the phonetic text from Waxholms TTS to NSTs SAMPA, and inserting the pair in the dictionary if it not already exists there.
Since the Waxholm data base is annotated by several different annotators, there are inconsistencies in the format of the smp.mix files. Therefore, we executed the mix2lab two times; one time we removed the X-words (words in the orthographic text which mark non-words sounds, like inhaling), and one time we kept the X-words. In this directory, you can see the resulting directories when executing dict on the text files without X-words (waxdict.dic) and on the text files with X-words (wax-xdict.dic).
Finally, we did manual checks and fixes on the two dictionaries, removing errors (there are lots of them, if you check the two dictionaries mentioned above). The result is in this directory, finalwaxdict.dic, and in the text file waxdict_missing you can see the words from the Waxholm data base which had no correct phonetic transcription in any of the two created dictionaries.
To run the program, type: dict <dictionary-filename> <input-filename>
Since you will build a dictionary from many text files, collect them in a directory and type:
for %a in (*.txt) do dict <dictionary-filename> "%~a"