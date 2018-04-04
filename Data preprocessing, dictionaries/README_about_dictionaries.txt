README_about_dictionaries

When training and/or aligning data with MFA, you need a dictionary with phonetic transcriptions of the words used in the data set. Therefore, some of the work involves creating, translating or in other ways dealing with dictionaries. In this subdirectory, you find converter.c which translates dictionaries from sampa to IPA, and sortdict.c which sorts dictionaries alphabetically and removes duplicated words. You also find an example dictionary resulting from converter.c; waxmfadict.txt is the Waxholm dictionary translated from sampa to IPA.
In other subdirectories you find other dictionary related programs:
1) the program convert_NST_lexicon.py in the subdirectory NST lexicon converts the dictionary given from the NST to the format needed by MFA
2) the program dict.c in the subdirectory Data preprocessing Waxholm creates a dictionary from the textfiles created by the program mix2lab.c, also in the subdirectory Data preprocessing Waxholm
3) a program creating a dictionary from the KI data, soon to be uploaded