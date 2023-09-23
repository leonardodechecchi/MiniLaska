#!/bin/bash

echo "**************************************************"
echo "Build MiniLaska - Introduzione alla Programmazione"
echo "**************************************************"
echo ""

BUILD_DIR="./build"
if [ ! -d "$BUILD_DIR" ];
then
    echo "Cartella ${BUILD_DIR} non trovata. Creo..."
    mkdir $BUILD_DIR
    
    if [ ! -d "$BUILD_DIR" ];
    then
        echo "Errore nel creare ${BUILD_DIR}. Compilazione fallita."
        exit 1
    fi
fi

SOURCES=(
    "bigint"
    "board"
    "engine"
    "move"
    
    "main"
)

OBJECT_FILES=()
for NAME in ${SOURCES[*]};
do
    echo "Compilo src/${NAME}.c..."
    # gcc -c -ansi -pedantic -O2 -o ${BUILD_DIR}/${NAME}.o src/${NAME}.c
    gcc -c -ansi -pedantic -g -O0 -o ${BUILD_DIR}/${NAME}.o src/${NAME}.c
    
    OBJECT_FILES+=(${BUILD_DIR}/${NAME}.o)
    
    echo "Compilato: src/${NAME}.c."
    echo ""
done

echo "Linking..."
# gcc -ansi -pedantic -O2 -o MiniLaska ${OBJECT_FILES[*]}
gcc -ansi -pedantic -g -O0 -o MiniLaska ${OBJECT_FILES[*]}

echo "Prodotto eseguibile nella root di progetto. Invocare: ./MiniLaska"
