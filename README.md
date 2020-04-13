# TacoTranscribe

## About
This project aims to help in editing LJSpeech-like datasets. It was created to edit/review the normalized version of the transcription of many audio files.
You can load LJSpeech-like audio scripts and listen to the audio in order to edit/review the normalized transcription.

## User Manual

### Dropdown Menu
Save Savepoint -> Write all reviewed/edited dataset-lines into a file of your liking. (Your "savepoint")
Save Script -> Saving the script/dataset (into a new file) you are working on in case you had to mark lines as unusable/missing.

### HowTo start/resume working
To start/resume working, you'd have to first load the audio, then load the script/dataset (remember to load the new version of your dataset in case you had to throw out lines using this tool before), and then (if continuing previous work) load the savepoint file.

### Saving/Loading files: File content syntax
The current version allows to load them as LJSpeech-like files. It however has to use a modified version.
While LJSpeech scripts are structured like
`Audiofilename (without file extension)|Transcription|Normalized Transcription`
my editor expects the LJSpeech scripts to be structured like
`Audiofilename.wav (or any other extension)|Transcription|Normalized Transcription`

The "old way" (from a previous version of this program) of reading the files still works. 
The old file content syntax (non-compliant) was:
`Transcription|Normalized Transcription|Audiofile.wav`
When saving or loading files, you have to choose the script syntax correctly using the file type dropdown menu. The standard is now the LJSpeech-like format. If you wanna keep using the old not-compliant format, choose the non-compliant in the file type menu.

## Copyright
This code uses QCustomPlot, which is licensed under the GPLv3 license.
The AudioGraph class uses [code written by StackOverflow user thibsc](https://stackoverflow.com/a/50294040), which is licensed under the MIT license and has been significantly changed by me.

Copyright (C) 2018 KaitoCross, thibsc

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU General Public License for more details.
