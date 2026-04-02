To apply Linux Coding Style to files
====================================

set astyle_param=--style=linux --indent=tab=8  --break-blocks --indent=force-tab=8  --pad-oper --pad-header --unpad-paren --indent-preprocessor --indent-preproc-define  --align-pointer=name
set astyle_path=<DIR>/AStyle.exe

eg: usage in Windows:
cd <DIR> // where we want to format
%astyle_path% %astyle_param% *.c

