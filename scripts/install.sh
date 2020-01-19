#!/bin/bash
echo "Thank you for choosing to install this beautiful piece of software!"
if [[ $EUID -ne 0 ]]; then
   echo "This script must be run as root" 1>&2
   exit 1
else
    build=true
    while [ "$1" != "" ]; do
        case $1 in
            --no-build )            shift
                                    build=false
                                    ;;
        esac
        shift
    done
    arch=$(uname -m)
    kernel=$(uname -r)
    declare -A osInfo;
    osInfo[/etc/redhat-release]=yum
    osInfo[/etc/arch-release]=pacman
    osInfo[/etc/gentoo-release]=emerge
    osInfo[/etc/SuSE-release]=zypp
    osInfo[/etc/debian_version]=apt
    for f in ${!osInfo[@]}; do
        if [[ -f $f ]]; then
            echo "Found package manager: ${osInfo[$f]}."
            pkgman=${osInfo[$f]}
        fi
    done
    if [[ -n "$(command -v lsb_release)" ]]; then
        distroname=$(lsb_release -s -d)
    elif [[ -f "/etc/os-release" ]]; then
        distroname=$(grep PRETTY_NAME /etc/os-release | sed 's/PRETTY_NAME=//g' | tr -d '="')
    elif [[ -f "/etc/debian_version" ]]; then
        distroname="Debian $(cat /etc/debian_version)"
    elif [[ -f "/etc/redhat-release" ]]; then
        distroname=$(cat /etc/redhat-release)
    else
        distroname="$(uname -s) $(uname -r)"
    fi
    echo "Trying to install dependencies for ${distroname} using ${pkgman}."
    if [[ ${pkgman} == yum ]]; then
        if yum -y update && yum -y install cmake make pkgconf-pkg-config gcc g++ sfml-devel mesa-libGL-devel; then
            echo "Successfully installed dependencies for your system." 
        else
            echo "Failed to install dependencies!"
            exit 1
        fi
    elif [[ ${pkgman} == pacman ]]; then
        if pacman -Syu --noconfirm && pacman -Sy --noconfirm cmake make pkg-config gcc sfml; then
            echo "Successfully installed dependencies for your system." 
        else
            echo "Failed to install dependencies!"
            exit 1
        fi
    elif [[ ${pkgman} == apt ]]; then
        if apt-get -y update && apt-get -y --fix-missing install make pkg-config gcc g++ libsfml-dev libegl1-mesa-dev; then
            echo "Successfully installed dependencies for your system."
        else
            echo "Failed to install dependencies!"
            exit 1
        fi
    else
        echo "No supported package manager found!"
        exit 1
    fi
    if [[ "$build" = true ]]; then 
       echo "Building project..."
       if sh scripts/build.sh; then
           echo "Built the project. Execute it by running 'sh scripts/run.sh'. Enjoy!"
       else
           echo "Failed to build the project!"
           exit 1
       fi
    fi
    exit 0
fi
