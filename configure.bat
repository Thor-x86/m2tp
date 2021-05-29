where cmake
if %ERRORLEVEL% neq 0 (
  echo Please install CMAKE first
  echo .
  exit 2
)

where make
if %ERRORLEVEL% neq 0 (
  echo Please install MAKE first
  echo .
  exit 2
)

where gcc
if %ERRORLEVEL% neq 0 (
  echo Please install GNU C++ Compiler first
  echo .
  exit 2
)

if not exist ".cmake" mkdir .cmake
if not exist "out" mkdir out

cd .cmake
cmake ..
