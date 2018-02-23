# PicoBlaze 6 Hardware Accelerator Assembler
In order to run the tool the regex2.dll is needed which can be obtained from [here](https://sourceforge.net/projects/gnuwin32/files/regex/2.7/regex-2.7-bin.zip/download?use_mirror=kent&download=) 

To use the tool, 5 arguments are needed (in this order):
- The assembly file location you would like to convert
- The name for the entity you would like for the VHDL file
- Whether you would like the PicoBlaze to go to sleep when running the Accelerator (Use ON or OFF)
- How many custom inputs you would like for the Accelerator
- How many custom outputs you would like for the Accelerator

Do note that you only need to supply the file location for the tool to work and the rest will default (works similarly when less than 5 arguments are given).
