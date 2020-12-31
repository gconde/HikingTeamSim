SET PATH=%PATH%;x64\Release

if exist x64\Release\test.exe (
    x64\Release\test.exe test\resources\trip1.yaml test\resources\trip2.yaml
) else (
    test\build\install\main\release\windows\test.bat test\resources\trip1.yaml test\resources\trip2.yaml
)
