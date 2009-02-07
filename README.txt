



.%% Building ABCp
=================

  Makefiles are provided for gcc. They have been tested on Windows with 
MSYS+MINGW and on Linux.

  1) Download clibutl (http://sites.google.com/site/clibutl/)
  2) Build it in a directory called c-libutl:
  
.v         
         x
         |__ abcp
         |__ c-libutl
..
 
  3) From the abcp directory type:
  
.v 
        make tools
        make release
..
     you now have the libabcp.a library and the abcp.h header
     in the abcp/dist directory.
     
  4) from the abcp/examples/abcdump directory type:
.v
        make release
..
     to build the abcdump example that give the full list of tokens
     for an abc file.
