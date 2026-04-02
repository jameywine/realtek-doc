@ECHO OFF

IF EXIST test_util.exe DEL /F test_util.exe
IF EXIST *.o DEL /F *.o
IF EXIST gpy2xx_api.lib DEL /F gpy2xx_api.lib
IF "%~1"=="clean" GOTO EXIT

:ECHO "     Building GPY211 Utility"
:ECHO "--------------------------------"
ECHO ON
@SET LOG_CFLAGS=
@IF "%~1"=="lib" GOTO SKIP_LOG_CFLAGS
SET LOG_CFLAGS=-DLOG_SEVERITY_MAX=2 -DLOGM_IN_FILE=0
:SKIP_LOG_CFLAGS
set UART_FLAG=-DP31G_UART=1 -DRAPID_UART=1 -DPC_UTILITY=1 -DP31G_OPT_SPEED=0
set EN_API_FLAG=-DSUPPORT_MACSEC_VLAN=0 -DEN_GMAC_DEBUG_ACCESS=0 -DEN_MSEC_DEBUG_ACCESS=0 -DEN_BACKWARD_COMP_API=0
set OPTINAL_TFLAGS=%LOG_CFLAGS% %EN_API_FLAG% -DP31G_B0=1 -DP34X_A0=1 -DSAFE_C_LIB=1 -DREG_RD_PRINT=0 -DREG_WR_PRINT=0 -DEN_SMDIO_RW=0
cd ..\..
set PC_TOOL=%CD%
set GPY211_INC=%PC_TOOL%\src\inc
set PHY_SRC=%PC_TOOL%\src\api\phy
set MACSEC_SRC=%PC_TOOL%\src\api\macsec
set DDKAES_SRC=%PC_TOOL%\ISC-DDK-160-BSD_v2.3\SafeXcel-IP-160_DDK_v2.3\Kit\SABuilder_MACsec\Example_AES\src
set DDKAES_INC=%PC_TOOL%\ISC-DDK-160-BSD_v2.3\SafeXcel-IP-160_DDK_v2.3\Kit\SABuilder_MACsec\Example_AES\incl
set LINUX_INC=%PC_TOOL%\src\os\linux

@IF "%~1"=="lib" GOTO SKIP_ASTYLE
:--max-code-length=80
set astyle_param=--style=linux --indent=tab=8  --break-blocks --indent=force-tab=8  --pad-oper --pad-header --unpad-paren --indent-preprocessor --indent-preproc-define  --align-pointer=name
set astyle_path=%PC_TOOL%\tools\astyle\win\astyle.exe

cd "%GPY211_INC%"
"%astyle_path%" %astyle_param% *.h
del *.orig

cd "%PHY_SRC%"
"%astyle_path%" %astyle_param% *.c
del *.orig

cd "%PHY_SRC%"
"%astyle_path%" %astyle_param% *.h
del *.orig

cd "%MACSEC_SRC%"
"%astyle_path%" %astyle_param% *.c
del *.orig

cd "%MACSEC_SRC%"
"%astyle_path%" %astyle_param% *.h
del *.orig

echo
:SKIP_ASTYLE

cd "%PC_TOOL%"
cd utility\pc
"%astyle_path%" %astyle_param% *.c
"%astyle_path%" %astyle_param% *.h
del *.orig

set ALL_INC=-I"%DDKAES_INC%"

gcc -m32 %UART_FLAG% %OPTINAL_TFLAGS% -Wall -fstack-protector-all -I%LINUX_INC% -I%GPY211_INC% -c "%PHY_SRC%\gpy211_phy.c"
gcc -m32 %UART_FLAG% %OPTINAL_TFLAGS% -Wall -fstack-protector-all -I%LINUX_INC% -I%GPY211_INC% -c "%PHY_SRC%\gpy211_chip.c"
gcc -m32 %UART_FLAG% %OPTINAL_TFLAGS% -Wall -fstack-protector-all -I%LINUX_INC% -I%GPY211_INC% -c "%MACSEC_SRC%\gpy211_macsec.c"
gcc -m32 %UART_FLAG% %OPTINAL_TFLAGS% %ALL_INC% -Wall -fstack-protector-all -I%LINUX_INC% -I%GPY211_INC% -c "%DDKAES_SRC%\aes.c"
ar rcs gpy2xx_api.lib gpy211_phy.o gpy211_chip.o gpy211_macsec.o aes.o

@IF "%~1"=="lib" GOTO SKIP_EXE
gcc -m32 %UART_FLAG% %OPTINAL_TFLAGS% -Wall -fstack-protector-all -I%LINUX_INC% -I%GPY211_INC% user_uart_interface.c gpy211_utility.c -L. -lgpy2xx_api -o test_util
:SKIP_EXE

IF EXIST *.o DEL /F *.o

@ECHO OFF
:ECHO "-------------- X ---------------"
pause
:EXIT
