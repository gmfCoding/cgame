# Converts all PNG images inside assets/** into XPM files
# using:
# python xpm saver 0.3.0
# python Pillow

UPDATE=0
ALL=0
HELP=0
PRINT_ONLY=0
VERBOSE=0

while [[ $#>0 ]]; do
	case "$1" in 
		'-u'|'--update')
			UPDATE=1
			shift 1
			continue
	esac
	case "$1" in 
		'-a'|'--all')
			ALL=1
			shift 1
			continue
	esac
	case "$1" in 
		'-h'|'--help')
			HELP=1
			shift 1
			continue
	esac
	case "$1" in 
		'-p'|'--print')
			PRINT_ONLY=1
			shift 1
			continue
	esac
	case "$1" in 
		'-v'|'--verbose')
			VERBOSE=1
			shift 1
			continue
	esac
	break
done

if [[ $HELP -eq 1 ]]
then
	(echo -e "Asset refresh:
	./refresh.sh [-hu]

	Converts all PNG files found in ../assets/source to XPM files in ../assets
	If the XPM already exists it will only convert if it the PNG is newer than the XPM.

	Dependencies: (auto fetcjed)
		Python3.8+
		pip3
		Python.PIL: pip install Pillow
		XPM: pip install git+https://github.com/gmfCoding/xpm_argb.git@master
		 ^ This is a fork of PyPI:xpm that supports more features

	-h --help
		displays this help message.

	-u --update 
		Updates the python xpm module (no automatic way)
		Use with -p / --print if you don't want to run the conversion.

	-a --all
		Converts all pngs regardless if they're older or newer than the equivelent XPMs.

	-p --print
		Don't convert anything only print.

	-l --VERBOSE
		Also print files tha were passed (due to them being older)
	")
	exit;
fi

python3 -c "import PIL"
if [[ $? -ne 0 ]]; then
	pip3 install Pillow
fi

python3 -c "import xpm"
if [[ $? -ne 0 ]] || [[ $UPDATE == 1 ]]; then
	pip3 install --upgrade --force-reinstall git+https://github.com/gmfCoding/xpm_argb.git@master
fi

OWD=$(pwd)
cd $(cd $(dirname $0); pwd -P)

BGREEN="\033[1;32m"
GREEN="\033[0;32m"
BRED="\033[1;31m"
RED="\033[0;31m"
BYELLOW="\033[1;33m"
YELLOW="\033[0;33m"
BBLUE="\033[1;34m"
BLUE="\033[0;34m"
BMAG="\033[1;35m"
MAG="\033[0;35m"
BCYAN="\033[1;36m"
CYAN="\033[0;36m"
NC="\033[0m"

function realpath()
{
	echo "$(cd $(dirname "$1");pwd)/$(basename "$1")"
}

FILES=$(find ../assets/source -type f -name "*.png" -not -path "*/_*")
printf "Converting files:\n"

for filename in $FILES; do
    IN=$(realpath $filename) # png path
	OUT=$(realpath "$(echo $filename | sed s,/assets/source/,/assets/,g | sed s,.png,.xpm,g)") # converts png path to xpm path
	mkdir -p $(dirname $OUT)
	if [[ $ALL -eq 1 || "$IN" -nt "$OUT" ]]; then
		printf ""$GREEN"converting $BLUE$IN$NC to$RED $OUT\n$NC"
		if [ $PRINT_ONLY -eq 0 ]; then
   			python3 -m xpm "$IN" --inverse-alpha > "$OUT"
		fi
	elif [ $VERBOSE -eq 1 ]; then
		printf ""$MAG"passing $BLUE$IN$NC to$RED $OUT\n$NC"
	fi
done
printf "Done.\n"

cd $OWD

# TODO automatic asset pipeline refresh?
# BLENDER=/mnt/c/Program Files/Blender Foundation/Blender 3.6/blender-launcher.exe
# $BLENDER -b assets/source/models/cube.blend -x 1 -o //render -f 0 # automatically rerender blend file