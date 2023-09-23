Write-Output "**************************************************"
Write-Output "Build MiniLaska - Introduzione alla Programmazione"
Write-Output "**************************************************"
Write-Output ""

$buildDir = "./build"
if (-not (Test-Path -Path $buildDir)) {
    Write-Output "Cartella ${buildDir} non trovata. Creo..."
    mkdir $buildDir

    if (-not (Test-Path -Path $buildDir)) {
        "Errore nel creare ${BUILD_DIR}. Compilazione fallita."
        Exit 1
    }
}

$sources = @(
    "bigint"
    "board"
    "engine"
    "move"
    
    "main"
)

$objectFiles = @()

$sources | ForEach-Object {
    Write-Output "Compilo src/$PSItem.c ..."
    gcc -c -ansi -pedantic -O2 -o "$buildDir/$PSItem.o" "src/$PSItem.c"
   
    $objectFiles += "$buildDir/$PSItem.o"
    
    Write-Output "Compilato: src/$PSItem.c"
    Write-Output ""
}


Write-Output "Linking..."
gcc -ansi -pedantic -O2 -o MiniLaska $objectFiles

Write-Output "Prodotto eseguibile nella root di progetto. Invocare: ./MiniLaska"