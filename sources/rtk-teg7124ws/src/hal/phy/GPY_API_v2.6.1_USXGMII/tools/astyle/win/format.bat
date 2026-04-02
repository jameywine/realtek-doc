:@ECHO OFF

SET CUR_DIR="%CD%"

cd ..\..\..
SET TOP=%CD%

:--max-code-length=80
SET astyle_param=--style=linux --indent=tab=8  --break-blocks --indent=force-tab=8  --pad-oper --pad-header --unpad-paren --indent-preprocessor --indent-preproc-define  --align-pointer=name
SET astyle_path=%TOP%\tools\astyle\win\astyle.exe

CD %TOP%\src\inc
"%astyle_path%" %astyle_param% *.h
IF EXIST *.orig DEL /F *.orig

CD %TOP%\src\api
"%astyle_path%" %astyle_param% *.h
"%astyle_path%" %astyle_param% *.c
IF EXIST *.orig DEL /F *.orig

CD %CUR_DIR%
