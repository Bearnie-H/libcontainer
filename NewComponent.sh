#!/usr/bin/env bash

#   Author: Joseph Sadden
#   Date:   6th March 2021

#   Purpose:
#
#       This script will generate a new, empty component for the current C/C++ project.

#   Terminal Colour Codes
FATAL="\e[7m\e[31m"
RED="\e[91m"
GREEN="\e[92m"
YELLOW="\e[93m"
AMBER="\e[33m"
BLUE="\e[96m"
WHITE="\e[97m"
CLEAR="\e[0m"

# Global Logging variables
Quiet=
OutputFile="-"  #    "-" indicates STDOUT
ColourLog=1  #  Flag for adding colours to the log, empty means no colouring
ColourFlag=
LogPrefix="$(basename $0)"
LOG_INFO=0
LOG_NOTICE=1
LOG_ATTENTION=2
LOG_WARNING=3
LOG_ERROR=4
LOG_FATAL=5
LogLevels=( "INFO:  " "NOTE:  " "ATTN:  " "WARN:  " "ERROR: " "FATAL: ")
LogColours=("$WHITE"  "$BLUE"   "$YELLOW" "$AMBER"  "$RED"    "$FATAL")
TimeFormat="+%F %H:%M:%S"

#   Catch errors, interrupts, and more to allow a safe shutdown
trap 'cleanup 1' 1 2 3 6 8 9 15

HelpFlag=

ComponentPath=
ComponentName=
#   If adding additional files to a component, this represents the name to associate with the new files.
ComponentSubFilename=

#   This isn't a great solution to getting the name of the directory containing this script,
#   but it should be fairly portable and get the job done, even if it doesn't look very good.
ProjectBaseFolder="$(cd "$(dirname "$0")" && pwd -P)"

ComponentSourceBase="$ProjectBaseFolder/"
RootMakefile="$ProjectBaseFolder/Makefile"
LanguageExtension=".c"

ComponentDirectory=
ComponentIncludeDirectory=

#   Command-line / Global variables


#   Function to display a help/usage menu to the user in a standardized format.
function helpMenu() {

    #   Get the name of the executable
    local scriptName=$(basename "$0")

    #   Print the current help menu/usage information to the user
    echo -e "
    $BLUE$scriptName   -   A bash tool to generate a new source code component for the current C/C++ project.$WHITE

    $GREEN$scriptName$YELLOW -n <Component Name> [-h] [-o Output-File] [-q] [-z]$WHITE

    "$YELLOW"Component Options:$WHITE
        $BLUE-n$WHITE  -    Component name. The name to use as the new component. This must be the full
                    path of the component, starting from the [ $ProjectBaseFolder/ ] directory.
        $BLUE-s$WHITE  -    Component Sub-file name. To add sub-component files to an existing component,
                    pass the name here. This simplifies adding more files to an existing component to extend
                    the behaviour and split up logic across more smaller files.

    "$YELLOW"Output Options:$WHITE
        $BLUE-o$WHITE  -    Log File. Redirect STDOUT to the given file, creating it if it doesn't exist yet.
        $BLUE-q$WHITE  -    Quiet mode. Only print out fatal errors and suppress all other output.
        $BLUE-z$WHITE  -    Raw Mode. Disables colouring, useful when the ANSI escape codes would be problematic.

    "$YELLOW"Miscellaneous Options:$WHITE
        $BLUE-h$WHITE  -    Help Menu. Display this help menu and exit.
    "$CLEAR
}

function cleanup() {

    #   Implement whatever cleanup logic is needed for the specific script, followed by resetting the terminal and exiting.

    # if [ $1 -eq 0 ]; then
    #     log $LOG_INFO "Successfully executed and beginning cleanup..."
    # else
    #     log $LOG_ATTENTION "Unsuccessfully executed and beginning cleanup..."
    # fi

    stop $1
}

function stop() {
    exit $1
}

function SetLogPrefix() {
    LogPrefix="$1"
}

#   $1 -> Log Level
#   $2 -> Log Message
function log() {

    local Level=$1

    #   Only log if not in quiet mode, or it's a fatal error
    if [[ -z "$Quiet" ]] || [[ $Level -eq $LOG_FATAL ]]; then

        local Message="$2"
        local Timestamp="[$(date "$TimeFormat")]"

        local ToWrite=

        if [ -z "$LogPrefix" ]; then
            ToWrite="$Timestamp ${LogLevels[$Level]} $Message"
        else
            ToWrite="$Timestamp [ $LogPrefix ] ${LogLevels[$Level]} $Message"
        fi

        #   If log colouring is on, check if it's writing to an output file
        if [ ! -z "$ColourLog" ] && [[ "$OutputFile" == "-" ]]; then
            ToWrite="${LogColours[$Level]}""$ToWrite""$CLEAR"
        fi

        #   Attention and higher should be logged to STDERR, Info and Notice to STDOUT
        if [ $Level -ge $LOG_ATTENTION ]; then
            echo -e "$ToWrite" >&2
        else
            if [[ "$OutputFile" == "-" ]]; then
                echo -e "$ToWrite" >&1
            else
                echo -e "$ToWrite" >> "$OutputFile"
            fi
        fi

        #   If it's a fatal error, full exit
        if [ $Level -eq $LOG_FATAL ]; then
            cleanup 1
        fi
    fi
}

#   Helper function to allow asserting that required arguments are set.
function argSet() {
    local argToCheck="$1"
    local argName="$2"

    if [ -z "$argToCheck" ]; then
        log $LOG_FATAL "Required argument [ $argName ] not set!"
    fi
}

#   Helper function to allow checking for the existence of files on disk.
function fileExists() {
    local FilenameToCheck="$1"

    if [ ! -f "$FilenameToCheck" ]; then
        log $LOG_ATTENTION "File [ $FilenameToCheck ] does not exist."
        return 1
    fi

    return 0
}

#   Helper function to allow checking for the existence of directories on disk.
function directoryExists() {
    local DirectoryToCheck="$1"

    if [ ! -d "$DirectoryToCheck" ]; then
        log $LOG_ATTENTION "Directory [ $DirectoryToCheck ] does not exist."
        return 1
    fi

    return 0
}

#   Helper function to either assert that a given directory does exist (creating it if necessary) or exiting if it cannot.
function assertDirectoryExists() {

    local DirectoryToCheck="$1"

    if ! directoryExists "$DirectoryToCheck"; then
        if ! mkdir -p "$DirectoryToCheck"; then
            log $LOG_FATAL "Failed to create directory [ $DirectoryToCheck ]!"
        fi

        log $LOG_NOTICE "Successfully created directory [ $DirectoryToCheck ]."
    fi
}

#   Main function, this is the entry point of the actual logic of the script, AFTER all of the input validation and top-level script pre-script set up has been completed.
function main() {

    #   Create the directory structure for the component.
    CreateComponentDirectories

    #   Create the default component files (source, source_test, header, header_test)
    CreateComponentSourceFiles

    #   Create the Makefile for the component.
    CreateComponentMakefile

    #   Update the root-level Makefile to add this component to the COMPONENTS variable
    UpdateRootMakefile

    return
}

function CreateComponentDirectories() {

    ComponentDirectory="$ComponentSourceBase/$ComponentPath/$ComponentName"
    ComponentIncludeDirectory="$ComponentSourceBase/$ComponentPath/$ComponentName/include"

    ComponentDirectories=("$ComponentDirectory" "$ComponentIncludeDirectory")

    for Dir in ${ComponentDirectories[@]}; do
        assertDirectoryExists "$Dir"
    done

    return;
}

function CreateComponentSourceFiles() {

    local RefName="$ComponentName"
    local LicenseFile="LICENSE"
    local LicenseText=

    if [ ! -z "$ComponentSubFilename" ]; then
        RefName="$ComponentSubFilename"
    fi

    if [ -f "$LicenseFile" ]; then
        log $LOG_INFO "Adding License preamble to start of newly created files."
        LicenseText="$(cat "$LicenseFile" | sed 's/^\(.\)/    \1/')"
        LicenseText="/*
$LicenseText
*/"
    fi

    RefName="$(echo "$RefName" | tr '-' '_')"

    ComponentSrc="$ComponentDirectory/$RefName$LanguageExtension"
    ComponentTestSrc="$ComponentDirectory/$RefName""_""test$LanguageExtension"
    ComponentHeader="$ComponentIncludeDirectory/$RefName"".h"
    ComponentTestHeader="$ComponentIncludeDirectory/$RefName""_test.h"

    #   Check to see if any of the files already exist, and if so, fail out
    #   to prevent overwriting existing code.
    ComponentFiles=("$ComponentSrc" "$ComponentTestSrc" "$ComponentHeader" "$ComponentTestHeader")

    for File in ${ComponentFiles[@]}; do
        if fileExists "$File"; then
            log $LOG_ERROR "File [ $File ] already exists!"
            log $LOG_FATAL "Exiting to prevent overwriting an existing component!"
        else
            log $LOG_INFO "File [ $File ] will be created and initialized to a default state."
        fi
    done

    ComponentNameUppercase="$(echo "$RefName" | tr '[:lower:]' '[:upper:]')"
    ProjectName="LIBCONTAINER"

    #   Create the main source file...
    ComponentSrcContents="$LicenseText

#include <stdlib.h>

#include \"include/$RefName.h\""

    #   Create the main test file...
    ComponentTestSrcContents="$LicenseText

#include <stdlib.h>
#include <stdio.h>

#include \"include/$RefName.h\"
#include \"../logging/logging.h\"

int Test_$RefName(void) {

    int FailedTests = 0;

    /* Tests go here... */
    TEST_PRINTF(\"Warning, no tests yet implemented for component [ %s ].\", \"$RefName\");

    return FailedTests;
}"

    #   Create the main header file...
    ComponentHeaderContents="$LicenseText

#ifndef $ProjectName"_"$ComponentNameUppercase""_H
#define $ProjectName"_"$ComponentNameUppercase""_H

/*
    If this header should export C-compatible symbols, rearrange these ifdefs as appropriate
*/
#ifdef __cplusplus
extern \"C\" {
#endif

#if defined(TESTING) || defined(DEBUGGER)

#include \"$RefName""_test.h\"

#endif

#ifdef __cplusplus
}
#endif

#endif"

    #   Create the main header test file...
    ComponentTestHeaderContents="$LicenseText

#ifndef $ProjectName"_"$ComponentNameUppercase""_TEST_H
#define $ProjectName"_"$ComponentNameUppercase""_TEST_H

/*
    If this header should export C-compatible symbols, rearrange these ifdefs as appropriate
*/
#ifdef __cplusplus
extern \"C\" {
#endif

/*
    Top-level testing entry-point for the component.
*/
int Test_$RefName(void);

#ifdef __cplusplus
}
#endif

#endif"

    echo "$ComponentSrcContents" > "$ComponentSrc"
    echo "$ComponentTestSrcContents" > "$ComponentTestSrc"
    echo "$ComponentHeaderContents" > "$ComponentHeader"
    echo "$ComponentTestHeaderContents" > "$ComponentTestHeader"

    return;
}

function CreateComponentMakefile() {

    ComponentMakefile="$ComponentDirectory/Makefile"

    MakefileContents="include \$(ROOTDIR)/DefaultComponent.mk"

    echo "$MakefileContents" > "$ComponentMakefile"

    return;
}

function UpdateRootMakefile() {

    if [ ! -z "$ComponentSubFilename" ]; then
        return;
    fi

    EscapedComponentName="$(echo "$ComponentPath/$ComponentName" | sed 's/\//\\\//g')"

    if [ ! -z "$(grep "$EscapedComponentName" "$RootMakefile")" ]; then
        return;
    fi

    sed -i "s/^COMPONENTS\(.*\)/COMPONENTS\1 $EscapedComponentName/g" "$RootMakefile"

    return;
}

#   Parse the command line arguments.  Add the flag name to the list (in alphabetical order), and add a ":" after if it requires an argument present.
#   The value of the argument will be located in the "$OPTARG" variable
while getopts "hn:o:qs:z" opt; do
    case "$opt" in
    h)  HelpFlag=1
        ;;
    n)  ComponentName="$OPTARG"
        ;;
    o)  OutputFile="$OPTARG"
        ;;
    q)  Quiet="-q"
        ;;
    s)  ComponentSubFilename="$OPTARG"
        ;;
    z)  ColourLog=
        ColourFlag="-z"
        ;;
    \?) HelpFlag=2
        ;;
    esac
done

case $HelpFlag in
    1)  helpMenu
        cleanup 0
        ;;
    2)  helpMenu
        cleanup 1
        ;;
esac

argSet "$OutputFile" "-o"

if [[ ! "$OutputFile" == "-" ]]; then

    #   Only assert this here, in case multiple -o arguments are given.
    #   Only create the file of the final argument.
    assertDirectoryExists "$(dirname "$OutputFile")"

    if ! fileExists "$OutputFile"; then
        #   Create the empty file.
        >"$OutputFile"
    fi
fi

#   Assert all of the required arguments are set here
argSet "$ComponentName" "-n"

#   Ensure the component name will work with Make
ComponentName="$(echo "$ComponentName" | sed 's/[ \t]\+/-/g')"

#   Split up the component name and directory parts, this will be helpful later when
#       using the name for file generation and header include guards.
ComponentPath="$(dirname "$ComponentName")"
ComponentName="$(basename "$ComponentName")"

#   Call main, running the full logic of the script.
main

cleanup 0

