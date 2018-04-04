README_converter.c

In the project, we worked with different phonetic alphabets. The NST data base and the data from KI were annotated in sampa, and the Waxholm data in STA. In README_dict.txt (subdirectory Data preprocessing Waxholm), you can read about how we translated Waxholms phonetic transcriptions from STA to sampa.
MFAs pretrained Swedish model uses another phonetic alphabet, IPA. To align the Waxholm data and the KI data with the pretrained model, we had to translate the dictionaries from sampa to IPA. The program converter.c makes this translation. To execute the program, type:
converter <input-file> <output-file>
In this directory, you can see how the Waxholm dictionary (finalwaxdict.dic, from subdirectory Data preprocessing Waxholm) looks after translation (the file waxmfadict.txt).
Note: MFAs phonetic alphabet IPA has 49 phones, while NSTs version of sampa only has 41 phones. Therefore, the translator from sampa to IPA does not use all of IPAs phones.