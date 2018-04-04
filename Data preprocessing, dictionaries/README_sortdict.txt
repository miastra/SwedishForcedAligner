README_sortdict

When aligning data with our different trained models, MFA did not accept our dictionary (which was the NST, the KI and the Waxholm dictionaries merged together, just pasted one after another). We assumed the MFA required an alphabetically sorted dictionary, and created sortdict.c.
If you have pasted some dictionaries (each of them alphabetically ordered) after each other, sortdict rearranges them to one alphabetcally ordered dictionary. During the process, sortdict also removes duplicates so there is only one dictionary line per orthographic word. In this subdirectory you can see NST_WAX_sorteddict.txt, which is the NST and the Waxholm dictionaries merged and alphabetically sorted. The unsorted version was 925839 lines, and the sorted version is 818147 lines, so there were some duplicates.
Later in the project, we realized there is actually no reason to merge several dictionaries, the dedicated dictionary is enough, but then the program was already written.
To execute sortdict, type:
sortdict <unsorted-dictionary-filename> <sorted-dictionary-filename>