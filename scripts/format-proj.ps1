# Script snippet found on 
# https://stackoverflow.com/questions/28896909/how-to-call-clang-format-over-a-cpp-project-folder
Push-Location .
Get-ChildItem -Path . -Directory -Recurse | foreach {
    Set-Location $_.FullName & clang-format -i -style=file *.cpp
}
Pop-Location